#pragma GCC target("avx")
#pragma GCC optimize(2)
#pragma GCC optimize(3,"Ofast","inline")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#include<bits/stdc++.h>
using namespace std;

const int k = 40;
const int N0 = 1e6+10;
const int N = 1e5+10, M = 4e6+10; 
const int inf = 0x3f3f3f3f;

typedef struct{
	int tot = 1, head[N], cur[N];
	struct {int next, from, to, val;} edge[M];
	
	void init(){
		tot = 1;
		memset(head, 0, sizeof(head));
	}
	
	void addedge(int from, int to, int val){
		edge[++tot].next = head[from]; 
		edge[tot].from = from;
		edge[tot].to = to; 
		edge[tot].val = val; 
		head[from] = tot;
	}
}Graph; 

struct LOC_CUT{
Graph g;
int d[N], vis[N], maxflow, s, t, n;

LOC_CUT(int _n, int _s, int _t, Graph _g) : n(_n), s(_s), t(_t), g(_g){maxflow = 0;}

bool bfs(){
	fill(d, d + 2 * n + 1, 0); 
	queue<int> q; q.push(s); 
	d[s] = 1; g.cur[s] = g.head[s];
	while(!q.empty()){
		int u = q.front(); q.pop();
		for(int i = g.head[u]; i; i = g.edge[i].next) {
			int v = g.edge[i].to;
			if(g.edge[i].val && !d[v]) {
				g.cur[v] = g.head[v];
				d[v] = d[u] + 1;
				q.push(v);
			}
		}
	}
	return d[t];
}

int dfs(int u, int in){
	if(u == t || in == 0) return in;
	int out = 0;
	for(int i = g.cur[u]; i && in; i = g.edge[i].next){
		g.cur[u] = i; 
		int v = g.edge[i].to;
		if(g.edge[i].val && d[v] == d[u] + 1){
			int res = dfs(v, min(g.edge[i].val, in));
			g.edge[i].val -= res, in -= res, g.edge[i ^ 1].val += res, out += res;
			if(in == 0) break;
		}
	}
	if(out == 0) d[u] = 0;
	return out;
} 

void dinic(){
	while(bfs()) 
		maxflow += dfs(s, inf);
}

void dfs_cut(int u){
	vis[u] = 1;
	for(int i = g.head[u]; i; i = g.edge[i].next){
		int v = g.edge[i].to;
		if(!vis[v] && g.edge[i].val) dfs_cut(v);
	}
}

set<int> find_cut(){
	set<int> cut;
	dinic();
	if(maxflow >= k) return {};
	memset(vis, 0, sizeof(vis));
	dfs_cut(s);
	for(int i = 2; i <= g.tot; i += 2){
		int u = g.edge[i].from, v = g.edge[i].to;
		if(u + n != v) break;
		if(vis[u] && !vis[v]) cut.insert(u);
	}
	return cut;
}
};

struct GLOBAL_CUT{
Graph g;
int n;
set<int> V;
vector<int>* Adj, deposit, dist;
vector<bool> pru;

GLOBAL_CUT(int _n, const set<int>& _V, vector<int>* &_Adj) : n(_n), V(_V), Adj(_Adj){
	pru.resize(n + 1);
	deposit.resize(n + 1);
	dist.resize(n + 1);
}

void add(int from, int to, int val){
	g.addedge(from, to, val);
	g.addedge(to, from, 0);
}

void create_graph(int s, int t){
	g.init();
	for(auto x : V){
		if(x != s && x != t) add(x, x + n, 1);
		else add(x, x + n, inf);
	}
	for(auto u : V)
		for(auto v : Adj[u]){
			add(u + n, v ,inf);
			add(v + n, u, inf);
		}
}

bool check_neighbor(int s, int t){
	for(auto x : Adj[s]) if(t == x) return true;
	return false;
}

bool check_ssv(int u){
	for(int i = 0; i < Adj[u].size(); ++i){
		for(int j = i + 1; j < Adj[u].size(); ++j){
			int v1 = Adj[u][i], v2 = Adj[u][j];
			if(!check_neighbor(v1, v2)) return false;
		}
	}
	return true;
}

void spfa(int s){
	deque<int> q;
	for(int i = 0; i <= n; ++i) dist[i] = inf, pru[i] = false;
	q.push_back(s); dist[s] = 0, pru[s] = true;
	while(!q.empty()){
		int u = q.front(); q.pop_front();  pru[u] = false;
		for(auto v : Adj[u])
			if(dist[v] > dist[u] + 1){
				dist[v] = dist[u] + 1;
				if(pru[v]) continue;
				pru[v] = true;
				if(!q.empty() && dist[v] < dist[q.front()]) q.push_front(v);
				else q.push_back(v);
			}
	}
	fill(pru.begin(), pru.end(), false);
}

void sweep(int u){
	pru[u] = true;
	for(auto v : Adj[u]){
		if(pru[v]) continue;
		++deposit[v];
		if(check_ssv(u) || deposit[v] >= k) sweep(v);
	}
}

set<int> getcut(int s, int t, const set<int>& V){
	if(check_neighbor(s, t)) return {};//s和t在原图上相连，无最小割 
	create_graph(s, t);
	LOC_CUT p(n, s, t + n, g);
	return p.find_cut();
}

set<int> find_global_cut(){
	int min_deg = inf, s = 0, t = 0;
	for(auto u : V){
		int deg = Adj[u].size();
		if(deg < min_deg)  min_deg = deg, s = u;	
	}
	priority_queue<pair<int, int> >pq;
	spfa(s);
	for(auto x : V)
		if(dist[x] > 1 && dist[x] != inf)
			pq.push(make_pair(dist[x], x));
	sweep(s);
	while(!pq.empty()){
		int v = pq.top().second; pq.pop();
		if(pru[v]) continue;
		set<int> cut = getcut(s, v, V);
		if(!cut.empty()) return cut;
		sweep(v);
	}
	if(!check_ssv(s)){
		for(int i = 0; i < Adj[s].size(); ++i)
			for(int j = i + 1; j < Adj[s].size(); ++j){
				int w1 = Adj[s][i], w2 = Adj[s][j];
				set<int> cut = getcut(w1, w2, V);
				if(!cut.empty()) return cut;
			}
	}
	return {};
}
};

namespace KVCC{
void dfs_connected_subgraph (int u, set<int> &tmp, vector<int>* &Adj, vector<bool>& vis){//找极大联通子图 
	vis[u]=1; tmp.insert(u);
	for(auto v : Adj[u]) 
		if(!vis[v])
			dfs_connected_subgraph(v, tmp, Adj, vis);  
}

void dfs_overlap_partition(int u, const set<int>& sV, vector<int>* &Adj, const set<int>& cut, set<int>& tmp, vector<bool>& vis){
	vis[u] = 1; tmp.insert(u);
	for(auto v : Adj[u])
		if(!vis[v] && sV.find(v) != sV.end() && cut.find(v) == cut.end()) 
			dfs_overlap_partition(v, sV, Adj, cut, tmp, vis);
}

vector<set<int> > kvcc(int n, vector<int>* E){
	vector<set<int> > ans;
	unordered_map<int, int> f1, f2; 
	//提取k-core中的点和边
	vector<int> deg;
	vector<bool> vist(n + 1, false);
	for(int i = 0; i <= n; ++i) deg.push_back(E[i].size());
	queue<int> kcore_q;
	for(int i = 0; i <= n; ++i) {
		if(!deg[i]) vist[i] = true;
		else if(deg[i] < k) kcore_q.push(i), vist[i] = true;
	}
	while(!kcore_q.empty()){
		int u = kcore_q.front(); kcore_q.pop();
		for(auto v : E[u])
			if(!vist[v] && --deg[v] < k) 
				kcore_q.push(v), vist[v]=true;
	}
	//重新标号建图 
	set<int> V;
	int cnt = 0;
	for(int i = 0; i <= n; ++i)
		if(!vist[i]) 
			f1[i] = ++cnt, f2[cnt] = i, V.insert(cnt);	
	vector<int> *Adj = new vector<int>[cnt + 1];
	for(int u = 0; u <= n; ++u){
	 	if(vist[u]) continue;
	 	for(auto v : E[u]){
	 		if(vist[v] || v > u) continue;
	 		int u1 = f1[u], v1 = f1[v];
			Adj[u1].push_back(v1);
			Adj[v1].push_back(u1);
		}
	}
	n = cnt;
	
	vector<bool> vis1(n+1, false);
	for(int i = 1; i <= n; ++i){
		if(vis1[i]) continue;
		set<int> sV;
		dfs_connected_subgraph(i, sV, Adj, vis1);
		if(sV.size() <= k) continue;
		
		//cout<<sV.size()<<' ';
		
		GLOBAL_CUT gc(n, sV, Adj);
		set<int> cut = gc.find_global_cut();
		
		if(cut.empty()){
			set<int>tmp;
			for(auto x : sV) tmp.insert(f2[x]);
			ans.push_back(tmp); 
		}
		
		else{//overlap partition 	
			vector<bool> vis2(n+1, false);
			for(auto x : sV){
				if(cut.find(x) != cut.end() || vis2[x]) continue;
				set<int> pV, pssv;
				dfs_overlap_partition(x, sV, Adj, cut, pV, vis2);
				for(auto y : cut) pV.insert(y);
				vector<int>* pAdj = new vector<int>[n + 1];
				for(auto x : pV)
					for(auto y : Adj[x])
						if(pV.find(y) != pV.end()) 
							pAdj[x].push_back(y);	
				vector<set<int> > res = kvcc(n, pAdj);
				for(auto v : res){
					set<int> tmp;
					for(auto x : v) tmp.insert(f2[x]);
					ans.push_back(tmp);
				}
			}
		}
	}
	return ans;	
}
}

inline int read(){
	int x=0,f=1;char ch=getchar();
	while (!isdigit(ch)){if (ch=='-') f=-1;ch=getchar();}
	while (isdigit(ch)){x=x*10+ch-48;ch=getchar();}
	return x*f;
}

int main(){
	freopen("dblp.txt", "r", stdin);
	int n = read(), m=read();
	vector<int> *E;
	E = new vector<int>[N0];
	for(int i = 1, x, y; i <= m; ++i) {
		x = read(), y = read();
		n = max(n, max(x, y));
		E[x].push_back(y);
		E[y].push_back(x);
	}
	vector<set<int> >  ans = KVCC::kvcc(n, E);
	sort(ans.begin(), ans.end(), [](set<int>x, set<int> y) {return x.size() < y.size();});
	cout<<"k = "<<k<<", KVCC num = "<<ans.size()<<endl;
	for(auto x : ans) {
		cout<<"Node num :"<<x.size()<<endl;
		for(auto y : x) cout << y <<" ";
		cout<<endl<<endl;
	}
}
