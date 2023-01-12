#include<bits/stdc++.h>
using namespace std;
const int N = 1e3+10, M = 1e5+10,inf=0x3f3f3f3f;
int n, m;

typedef struct{
	int tot = 1, head[N], cur[N];
	struct {int next, from, to, cap;} edge[M];
	
	void init(){
		tot = 1;
		memset(head, 0, sizeof(head));
	}
	
	void addedge(int from, int to, int cap){
		edge[++tot].next = head[from]; 
		edge[tot].from = from;
		edge[tot].to = to; 
		edge[tot].cap = cap; 
		head[from] = tot;
	}
}Graph; 

struct LOC_CUT{
Graph g;
int d[N], vis[N], maxflow, s, t;
vector<int> cut;

LOC_CUT(int _s, int _t) : s(_s), t(_t){
	init();
}

LOC_CUT(int _s, int _t, Graph _g) : s(_s), t(_t), g(_g){}

void init(){
	maxflow = 0;
	g.init();
}

void add(int from, int to, int cap){
	g.addedge(from, to, cap);
	g.addedge(to, from, 0);
}

bool bfs(){
	memset(d, 0, sizeof(d)); 
	queue<int> q; q.push(s); 
	d[s] = 1; g.cur[s] = g.head[s];
	while(!q.empty()){
		int u = q.front(); q.pop();
		for(int i = g.head[u]; i; i = g.edge[i].next) {
			int v = g.edge[i].to;
			if(g.edge[i].cap && !d[v]) {
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
		if(g.edge[i].cap && d[v] == d[u] + 1){
			int res = dfs(v, min(g.edge[i].cap, in));
			g.edge[i].cap -= res, in -= res, g.edge[i ^ 1].cap += res, out += res;
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
		if(!vis[v] && g.edge[i].cap) dfs_cut(v);
	}
}

bool find_cut(){
	dinic();
	if(maxflow >= inf) return false;
	memset(vis, 0, sizeof(vis));
	dfs_cut(s);
	for(int i = 2; i <= g.tot; i += 2){
		int u = g.edge[i].from, v = g.edge[i].to;
		if(u + n != v) break;
		if(vis[u] && !vis[v]) cut.push_back(u);
	}
	return true;
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
	int s=read(), t=read();
	LOC_CUT p(s ,t + n);
	for(int i = 1; i <= n; ++i){
		if(i != s && i != t) p.add(i, i + n, 1);
		else p.add(i, i + n, inf);
	}
	while(m--){
		int u = read(), v = read();
		p.add(u + n, v, inf); p.add(v + n, u, inf);
	}
	p.find_cut();
	printf("%d\n", p.maxflow);
	for(auto x: p.cut) cout<<x<<' ';
	return 0;
}

/*
5 6 1 5
1 2
1 3
2 4
3 4
4 5
3 5
*/
