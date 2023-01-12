#include<bits/stdc++.h>
using namespace std;
const int N = 1e3+10, M = 1e5+10,inf=0x3f3f3f3f;
vector<pair<int, int> >E;
vector<int> Adj[N];
int n, m;

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
int d[N], vis[N], maxflow, s, t;

LOC_CUT(int _s, int _t, Graph _g) : s(_s), t(_t), g(_g){maxflow = 0;}

bool bfs(){
	memset(d, 0, sizeof(d)); 
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

vector<int> find_cut(){
	vector<int> cut;
	dinic();
	if(maxflow >= inf) return cut;
	memset(vis, 0, sizeof(vis));
	dfs_cut(s);
	for(int i = 2; i <= g.tot; i += 2){
		int u = g.edge[i].from, v = g.edge[i].to;
		if(u + n != v) break;
		if(vis[u] && !vis[v]) cut.push_back(u);
	}
	return cut;
}
};

struct GLOBAL_CUT{
Graph g;		
void add(int from, int to, int val){
	g.addedge(from, to, val);
	g.addedge(to, from, 0);
}

void create_graph(int s, int t, const set<int>& V){
	g.init();
	for(auto x : V){
		if(x != s && x != t) add(x, x + n, 1);
		else add(x, x + n, inf);
	}
	for(auto u : V){
		for(auto v : Adj[u])
			if(V.find(v) != V.end()){
				add(u + n, v ,inf);
				add(v + n, u, inf);
			}
	}
}

bool check(int s, int t){
	for(auto x : Adj[s]) if(t == x) return true;
	return false;
}

vector<int> getcut(int s, int t, const set<int>& V){
	if(check(s, t)) return {};//s和t在原图上相连，无最小割 
	create_graph(s, t, V);
	LOC_CUT p(s, t, g);
	return p.find_cut();
}

vector<int> find_global_cut(const set<int>& V){
	int min_deg = inf, s = 0, t = 0;
	for(auto u : V){
		int deg = 0;
		for(auto v : Adj[u]) if(V.find(v) != V.end()) ++deg;
		if(deg < min_deg) 
			min_deg = deg, s = u;//找一个最小度点	
	}
	for(auto x : V){
		if(s == x) continue;
		vector<int> cut = getcut(s, x, V);
		if(cut.size()) return cut;
	}	
	for(int i = 0; i < Adj[s].size(); ++i){
		if(V.find(Adj[s][i]) == V.end()) continue;
		for(int j = i + 1; j < Adj[s].size(); ++j){
			if(V.find(Adj[s][j]) == V.end()) continue;
			vector<int> cut = getcut(Adj[s][i], Adj[s][j], V);
			if(cut.size()) return cut;
		}
	}
	return {};
}
};

inline int read(){
	int x=0,f=1;char ch=getchar();
	while (!isdigit(ch)){if (ch=='-') f=-1;ch=getchar();}
	while (isdigit(ch)){x=x*10+ch-48;ch=getchar();}
	return x*f;
}

int main(){
	n=read(), m=read();
	set<int>V;
	for(int i = 1, x, y; i <= m; ++i) {
		x = read(), y = read();
		Adj[x].push_back(y);
		Adj[y].push_back(x); 
		V.insert(x), V.insert(y);
		E.push_back(make_pair(x, y));
	}
	GLOBAL_CUT gc;
	vector<int> global_cut = gc.find_global_cut(V);
	cout<<global_cut.size()<<endl;
	for(auto x:global_cut) cout<<x<<' ';
	return 0;
}

/*
5 6 
1 2
1 3
2 4
3 4
4 5
3 5

6 9
1 4
1 5
1 6
2 4
2 5
2 6
3 4
3 5
3 6
*/
