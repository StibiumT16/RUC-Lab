#pragma GCC target("avx")
#pragma GCC optimize(3,"Ofast","inline")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#include<bits/stdc++.h>
using namespace std;
#define pii pair<int,int> 
#define mp make_pair
#define pb push_back
#define fi first
#define se second

const int inf = 0x7f7f7f7f;
const int M = 2346;
const int N = 79;
int NUM = 0;//拆点后总节点数 

namespace Graph{
struct tim{
	int date, h, m;
	
	tim() : date(0), h(0), m(0){}
	tim(int _d, int _h, int _m) : date(_d), h(_h), m(_m) {}
	
	bool operator < ( const tim &x )const{
		if(x.date != date) return date < x.date;
		else if(x.h != h) return h < x.h;
		else return m < x.m;
	}
	
	bool operator == ( const tim &x )const{
		return (date == x.date) && (h == x.h) && (m == x.m);
	}
	
	friend int operator - (const tim y, const tim x){
		return 24 * 60 * (y.date - x.date) + 60 * (y.h - x.h) + y.m - x.m;
	}
};
struct EDGE{int to, id, cost, model; tim st,ed;}; 
vector<EDGE>edge[N + 10];

struct {int next, to, cost, t, transfer, model;}e[M * M];
int tot = 0, head[M<<2]; 
void addedge(int from, int to, int cost, int T, int transfer, int model = 0){
  	e[++tot].next = head[from];
  	e[tot].to = to, e[tot].cost = cost, e[tot].t = T, e[tot].transfer = transfer, e[tot].model = model;
 	head[from]=tot;
}

map<tim, int> f1[N + 10], f2[N + 10];//f1:出点编号    f2:入点编号 
map<int, int> g[M<<1];//编号对应航班 


void init_addedge(){
	memset(head,0,sizeof(head)); tot = 0;
	for(int u = 1; u <= N; ++u){
		for(auto E : edge[u])
		addedge(f1[u][E.st] , f2[E.to][E.ed], E.cost, E.ed - E.st, 0, E.model);
		for(auto i = f2[u].begin(); i != f2[u].end(); i++)  //转机:同一个机场节点的入点连出点 
		 	for(auto j = f1[u].begin(); j != f1[u].end(); j++)
		 		if(i->fi < j->fi || i->fi == j->fi) addedge(i->se, j->se, 0, j->fi - i->fi, 1, 0); 
	}
}
}
using namespace Graph;

namespace Connectivity{//连通性和遍历 
int Matrix[N+10][N+10], ans[N+10][N+10], dx[N+10][N+10];
bool vis[N+10];

void bfs(int s){
	queue<int>q;
	vis[s] = true; q.push(s);
	while(!q.empty()){
		int u=q.front(); q.pop();
		for(auto e:edge[u]) 
			if(!vis[e.to]) vis[e.to] = 1,q.push(e.to);
	}
} 

void dfs(int u){
	vis[u] = 1;
	for(auto e : edge[u])
		if(!vis[e.to]) dfs(e.to);
}	

void Connect(int r){
	memset(ans,0,sizeof(ans));
	for(int i = 1; i <= N; ++i) ans[i][i] = 1; 
	while(r--){
		for(int i = 1;i <= N; i++)
    	  	for(int j = 1; j <= N; j++)
      		   	dx[i][j] = ans[i][j], ans[i][j] = 0; 
		for(int i = 1;i <= N; i++)
      		for(int j = 1;j <= N; j++)
        		for(int k = 1; k <= N; k++)
					ans[i][j] |=  Matrix[i][k] & dx[k][j];
	}
	for(int i = 1;i <= N; ++i){
		for(int j = 1 ; j <= N ; ++j) cout<< ans[i][j] << ' ';
		cout << endl;
	}                                        
}

void Main1(int flag){//flag = 1: 遍历    flag = 2：查询能否到达 
	if(flag == 1){
		puts("输入出发的机场：");
		int s; cin >> s;
		memset(vis,0,sizeof(vis));
		dfs(s);
		for(int i = 1; i <= N; ++i) if(vis[i]) cout << i << ' '; 
	}
	else{
		puts("输入一个>=-1的数： -1：无限制  0：直飞  1：1次中转  2:2次中转 .......");
		int t; cin >> t;
		if(!~t){
			for(int i =1;i <= N; ++i){
				for(int j = 1 ; j <= N ; ++j) cout<< 1 << ' ';
				cout << endl;
			}
		}
		else Connect(t + 1);
	}	
	puts("");
}

void Main2(int s, int t){//仅限直飞或1次中转，求任意两个机场的航线
	vector<pii> ans;
	for(auto e0 : edge[s]) if(e0.to == t) ans.pb(mp(e0.id,0));
	for(auto e1 : edge[s]){
		int k = e1.to;
		for(auto e2 : edge[k])
			if(e2.to == t && (e1.ed < e2.st || e1.ed == e2.st)) ans.pb(mp(e1.id,e2.id));
	} 
	sort(ans.begin(),ans.end());
	for(auto x:ans){
		cout << x.fi;
		if(x.se) cout << ' ' << x.se;
		cout << endl;
	}
}
}

namespace ShortestPath1{//一维dijkstra 
int dis[M<<1], pre[M<<1];
bool vis[M<<1];

struct SegmentTree{//线段树维护dijkstra 
#define ls p<<1
#define rs p<<1|1
	int Min[M << 3], rk[M << 3], pos[M << 3];
	
	void pushup(int p){	
		if(Min[ls] <= Min[rs]) Min[p] = Min[ls], rk[p] = rk[ls];
		else Min[p] = Min[rs], rk[p] = rk[rs];
	}
	void build(int l, int r, int p) {
		if(l == r){
			Min[p] = inf, pos[rk[p] = l] = p;
			return;
		}
		int mid = (l + r) >> 1;
		build(l, mid, ls); build(mid + 1, r, rs);
		pushup(p);
	}
	void update(int p, int val){//单点修改 
		Min[p] = val;
		while(p>>1) pushup(p >> 1),  p >>= 1;
	}
#undef ls
#undef rs 
};

void dijkstra(int s, int flag){//flag = 1: time; flag = 2: cost ; flag = 3: transfer
	memset(vis,0,sizeof(vis)), memset(dis,0x7f,sizeof(dis)), memset(pre,0,sizeof(pre));
	SegmentTree T; dis[s] = 0;
	T.build(1,NUM+2,1); 
	T.update(T.pos[s], 0);
	while(T.Min[1] ^ inf){
		int u = T.rk[1]; T.update(T.pos[u], inf);
		if(vis[u]) continue; vis[u]=1;
		for(int i = head[u]; i; i = e[i].next){
			int v = e[i].to;
			if(flag == 1){
				if(dis[v] > dis[u] + e[i].t) 
					dis[v] = dis[u] + e[i].t, T.update(T.pos[v], dis[v]), pre[v] = u;
			}
			else if(flag == 2){
				if(dis[v] > dis[u] + e[i].cost) 
					dis[v] = dis[u] + e[i].cost,  T.update(T.pos[v], dis[v]), pre[v] = u;
			}
			else{
				if(dis[v] > dis[u] + e[i].transfer) 
					dis[v] = dis[u] + e[i].transfer, T.update(T.pos[v], dis[v]), pre[v] = u;
			}	
		}
	}
}

void dijkstra2(int s, int model, int flag){//考虑机型, flag = 1: cost; flag = 2 :transfer
	memset(vis,0,sizeof(vis)), memset(dis,0x7f,sizeof(dis)), memset(pre,0,sizeof(pre));
	SegmentTree T; dis[s] = 0;
	T.build(1,NUM+2,1); 
	T.update(T.pos[s], 0);
	while(T.Min[1] ^ inf)
	{
		int u = T.rk[1]; T.update(T.pos[u], inf);
		if(vis[u]) continue; vis[u]=1;
		for(int i = head[u]; i; i = e[i].next){
			int v = e[i].to;
			if(e[i].model && e[i].model != model) continue; 
			if(flag == 1){
				if(dis[v] > dis[u] + e[i].cost) 
					dis[v] = dis[u] + e[i].cost,  T.update(T.pos[v], dis[v]), pre[v] = u;
			}
			else{
				if(dis[v] > dis[u] + e[i].transfer) 
					dis[v] = dis[u] + e[i].transfer, T.update(T.pos[v], dis[v]), pre[v] = u;
			}	
		}
	}
}

void Main1(int u, int v){ //求任意两个机场之间的最短时间
	int S = NUM + 1, T = NUM + 2;
	init_addedge();
	for(auto i = f1[u].begin(); i != f1[u].end(); i++) addedge(S, i->se, 0, 0, 0);
	for(auto i = f2[v].begin(); i != f2[v].end(); i++) addedge(i->se, T, 0, 0, 0);
	 dijkstra(S, 1);
	if(dis[T] == inf){
		puts("-1");
		return;
	}
	cout << dis[T] <<endl; 
}

void Main2(int u,int v, tim T1, tim T2){//给定起飞时段求最小费用 
	int S = NUM + 1, T = NUM + 2;
	init_addedge();
	for(auto i = f1[u].begin(); i != f1[u].end(); i++)
		if((T1 < i->fi && i->fi < T2 ) || T1 == i->fi || T2 == i->fi )
			addedge(S, i->se, 0, 0, 0);
	for(auto i = f2[v].begin(); i != f2[v].end(); i++) addedge(i->se, T, 0, 0, 0);
	dijkstra(S, 2);
	if(dis[T] == inf){
		puts("-1");
		return;
	}
	vector<int>ID;
	for(int i = T; i; i = pre[i]) 
		if(g[i][pre[i]]) ID.pb(g[i][pre[i]]);
	reverse(ID.begin(), ID.end());
	for(auto x: ID) cout << x << ' ';
	cout<<endl;
	cout << dis[T] <<endl; 
} 

void Main3(int u,int v,tim T1, tim T2){//给定降落时段求最小费用 
	int S = NUM + 1, T = NUM + 2;
	init_addedge();
	for(auto i = f1[u].begin(); i != f1[u].end(); i++) addedge(S, i->se, 0, 0, 0);
	for(auto i = f2[v].begin(); i != f2[v].end(); i++) 
		if((T1 < i->fi && i->fi < T2 ) || T1 == i->fi || T2 == i->fi )
			addedge(i->se, T, 0, 0, 0);
	dijkstra(S, 2);
	if(dis[T] == inf){
		puts("-1");
		return;
	}
	vector<int>ID;
	for(int i = T; i; i = pre[i]) 
		if(g[i][pre[i]]) ID.pb(g[i][pre[i]]);
	reverse(ID.begin(), ID.end());
	for(auto x: ID) cout << x << ' ';
	cout<<endl;
	cout << dis[T] <<endl; 
} 

void Main4(int u,int v, int model){//给定机型求最小费用 
	int S = NUM + 1, T = NUM + 2;
	init_addedge();
	for(auto i = f1[u].begin(); i != f1[u].end(); i++) addedge(S, i->se, 0, 0, 0);
	for(auto i = f2[v].begin(); i != f2[v].end(); i++) addedge(i->se, T, 0, 0, 0);
	dijkstra2(S, model, 1);
	if(dis[T] == inf){
		puts("-1");
		return;
	}
	vector<int>ID;
	for(int i = T; i; i = pre[i]) 
		if(g[i][pre[i]]) ID.pb(g[i][pre[i]]);
	reverse(ID.begin(), ID.end());
	for(auto x: ID) cout << x << ' ';
	cout<<endl;
	cout << dis[T] <<endl; 
}

void Main5(int u,int v, int k, tim T1, tim T2){//给定起飞时段限制中转次数 
	int S = NUM + 1, T = NUM + 2;
	init_addedge();
	for(auto i = f1[u].begin(); i != f1[u].end(); i++)
		if((T1 < i->fi && i->fi < T2 ) || T1 == i->fi || T2 == i->fi )
			addedge(S, i->se, 0, 0, 0);
	for(auto i = f2[v].begin(); i != f2[v].end(); i++) addedge(i->se, T, 0, 0, 0);
	dijkstra(S, 3);
	if(dis[T] > k) puts("-1");
	else{
		vector<int>ID;
		for(int i = T; i; i = pre[i]) 
			if(g[i][pre[i]]) ID.pb(g[i][pre[i]]);
		reverse(ID.begin(), ID.end());
		for(auto x: ID) cout << x << ' ';
		cout<<endl;
	}
} 

void Main6(int u,int v, int k, tim T1, tim T2){//给定降落时段限制中转次数 
	int S = NUM + 1, T = NUM + 2;
	init_addedge();
	for(auto i = f1[u].begin(); i != f1[u].end(); i++) addedge(S, i->se, 0, 0, 0);
	for(auto i = f2[v].begin(); i != f2[v].end(); i++) 
		if((T1 < i->fi && i->fi < T2 ) || T1 == i->fi || T2 == i->fi )
			addedge(i->se, T, 0, 0, 0);
	dijkstra(S, 3);
	if(dis[T] > k) puts("-1");
	else{
		vector<int>ID;
		for(int i = T; i; i = pre[i]) 
			if(g[i][pre[i]]) ID.pb(g[i][pre[i]]);
		reverse(ID.begin(), ID.end());
		for(auto x: ID) cout << x << ' ';
		cout<<endl;
	}
} 

void Main7(int u,int v, int k, int model){//给定机型限制中转次数 
	int S = NUM + 1, T = NUM + 2;
	init_addedge();
	for(auto i = f1[u].begin(); i != f1[u].end(); i++) addedge(S, i->se, 0, 0, 0);
	for(auto i = f2[v].begin(); i != f2[v].end(); i++) addedge(i->se, T, 0, 0, 0);
	dijkstra2(S, model, 2);
	if(dis[T] > k) puts("-1");
	else{
		vector<int>ID;
		for(int i = T; i; i = pre[i]) 
			if(g[i][pre[i]]) ID.pb(g[i][pre[i]]);
		reverse(ID.begin(), ID.end());
		for(auto x: ID) cout << x << ' ';
		cout<<endl;
	}
}
}

namespace ShortestPath2{//二维dijkstra 
unordered_map<int, bool>vis[M<<1];
unordered_map<int, int>dis[M<<1];
unordered_map<int, pii> pre[M<<1];

struct node{
    int pos, ti, cost;
    bool operator <( const node &x )const{
		if(cost == x.cost) return ti > x.ti;
		return cost > x.cost;
	}
};
priority_queue<node>q;

void dijkstra(int s, int lim, int flag){//flag = 1:限定中转次数求最小时间， flag = 2: 限定中转时间求最小费用 
	for(int i = 1; i <= NUM + 2; ++i) pre[i].clear(), vis[i].clear(), dis[i].clear();
	dis[s][0]=0; q.push((node){s,0,0});
	while(!q.empty()){
		node x=q.top(); q.pop(); int u = x.pos, trans= x.ti;
		if(vis[u][trans]) continue; vis[u][trans] = 1;
		for(int i = head[u]; i; i = e[i].next){
			int v = e[i].to; 
			int tr = trans + (flag == 2 ? e[i].t * e[i].transfer : e[i].transfer);
			int cost = flag ==2 ? e[i].cost : e[i].transfer * e[i].t; 
			if(tr > lim) continue;
			if(!dis[v].count(tr) || (dis[v].count(tr) && dis[v][tr] > dis[u][trans] + cost))
				dis[v][tr] = dis[u][trans] + cost, q.push((node){v, tr, dis[v][tr]}), pre[v][tr] = mp(u, trans);
		}
	}
}

void Main1(int u, int v, int lim_trans, int lim_time){//限制中转次数和中转时间 
	int S = NUM + 1, T = NUM + 2;
	init_addedge();
	for(auto i = f1[u].begin(); i != f1[u].end(); i++) addedge(S, i->se, 0, 0, 0);
	for(auto i = f2[v].begin(); i != f2[v].end(); i++) addedge(i->se, T, 0, 0, 0);
	dijkstra(S, lim_trans, 1);
	for(auto i = dis[T].begin(); i != dis[T].end(); i++)
		if(i->se <= lim_time && i->fi <= lim_trans) {
			vector<int>ID;
			for(int j = T, k = i->fi; j;) {
				if(g[j][pre[j][k].fi]) ID.pb(g[j][pre[j][k].fi]);
				int jj = pre[j][k].fi, kk = pre[j][k].se;
				j = jj, k = kk;
			}
			reverse(ID.begin(), ID.end());
			for(auto x: ID) cout << x << ' ';
			cout<<endl;
			return;
		}
	puts("-1");
}

void Main2(int u, int v, int lim_time){//限制中转时间求最小费用 
	int S = NUM + 1, T = NUM + 2;
	init_addedge();
	for(auto i = f1[u].begin(); i != f1[u].end(); i++) addedge(S, i->se, 0, 0, 0);
	for(auto i = f2[v].begin(); i != f2[v].end(); i++) addedge(i->se, T, 0, 0, 0);
	dijkstra(S, lim_time, 2);
	int Min = inf ,tmp; 
	for(auto i = dis[T].begin(); i != dis[T].end(); i++)
		if(i->se < Min && i->fi <= lim_time) Min = i->se, tmp = i->fi;
	if(Min == inf){
		puts("-1");
		return;
	}
	vector<int>ID;
	for(int j = T, k = tmp; j;) {
		if(g[j][pre[j][k].fi]) ID.pb(g[j][pre[j][k].fi]);
		int jj = pre[j][k].fi, kk = pre[j][k].se;
		j = jj, k = kk;
	}
	reverse(ID.begin(), ID.end());
	for(auto x: ID) cout << x << ' ';
	cout << endl;
	cout << dis[T][tmp] << endl;
}
}

namespace INIT{//read the data
int m = 0;
struct info{
	int num, No, from, to, id, model, val;
	tim st, ed;
}a[M + 10];
	
void Read(){
	vector<string> row;
	ifstream in("data.csv");
	string line;
	getline(in, line);
	while(getline(in, line) && in.good()){
		++m;
		int i = 0;
		while(line[i] != ',') a[m].num = a[m].num * 10 + line[i] - '0', ++i;
		i += 3;
		a[m].st.date = line[i] - '0';
		i += 12;
		while(line[i] != ',') a[m].No = a[m].No * 10 + line[i] - '0', ++i;
		++i;
		while(line[i] != ',') a[m].from = a[m].from * 10 + line[i] - '0', ++i;
		++i;
		while(line[i] != ',') a[m].to = a[m].to * 10 + line[i] - '0', ++i;
		i+=10;
		while(line[i] != ':') a[m].st.h = a[m].st.h * 10 + line[i] - '0', ++i;
		++i;
		while(line[i] != ',') a[m].st.m = a[m].st.m * 10 + line[i] - '0', ++i;
		i += 3;
		a[m].ed.date = line[i] - '0';
		i += 7;
		while(line[i] != ':') a[m].ed.h = a[m].ed.h * 10 + line[i] - '0', ++i;
		++i;
		while(line[i] != ',') a[m].ed.m = a[m].ed.m * 10 + line[i] - '0', ++i;
		++i;
		while(line[i] != ',') a[m].id = a[m].id * 10 + line[i] - '0', ++i;
		++i;
		while(line[i]!=',') a[m].model = a[m].model * 10 + line[i] - '0', ++i;
		++i;
		while(i < line.length()) a[m].val = a[m].val * 10 + line[i] - '0', ++i;
	}
	in.close();
}

void Main(){
	Read();
	for(int i = 1; i <= m; ++i){
		Connectivity::Matrix[a[i].from][a[i].to] = 1;
		edge[a[i].from].pb((EDGE){a[i].to, a[i].num, a[i].val, a[i].model, a[i].st, a[i].ed});
		if(!f1[a[i].from][a[i].st]) f1[a[i].from][a[i].st] = ++NUM;
		if(!f2[a[i].to][a[i].ed]) f2[a[i].to][a[i].ed] = ++NUM;
		g[f2[a[i].to][a[i].ed]][f1[a[i].from][a[i].st]] = a[i].num;
	}
	for(int i = 1; i <= N ; ++i) Connectivity::Matrix[i][i] = 1;
}
}

int main(){
	INIT::Main();
	while(1){ 
		int opt1, opt2;
		while(1){
			puts("***********************************************"); 
			puts("输入操作数:                                   *"); 
			puts("1：任意机场出发的遍历                         *"); 
			puts("2：查询机场连通性                             *"); 
			puts("3：任意两个机场之间的最短飞行时间             *"); 
			puts("4：限直飞或1次中转，求两个机场的航线          *"); 
			puts("5：求任意两个机场中转次数不超过K的路线        *"); 
			puts("6：求任意两个机场之间的航费最低的路径         *"); 
			puts("7：求限制中转时间和中转次数的路径             *");
			puts("8：求限制中转时间，航飞最低的路径             *");
			puts("0：关闭                                       *");
			puts("***********************************************"); 
			cin >> opt1;
			if(opt1 >= 0 && opt1 <= 8) break;
			puts("错误，请输入正确的操作数：");
		}
		if(opt1 == 1) Connectivity::Main1(1);
		else if(opt1 == 2) Connectivity::Main1(2);
		else if(opt1 == 3){
			puts("输入出发和到达机场");
			int s, t; cin >> s >> t;
			ShortestPath1::Main1(s, t);
		}
		else if(opt1 == 4){
			puts("输入出发和到达机场");
			int s, t; cin >> s >> t;
			Connectivity::Main2(s, t);
		}
		else if(opt1 == 5){ 
			while(1){
				puts("***********************"); 
				puts("输入操作数:           *"); 
				puts("1：给定起飞时间段     *"); 
				puts("2：给定降落时间段     *"); 
				puts("3：给定机型           *"); 
				puts("***********************"); 
				cin >> opt2;
				if(opt2 >= 1 && opt2 <= 3) break;
				puts("错误，请输入正确的操作数：");
			}
			if(opt2 == 1){
				puts("输入起飞降落机场，中转次数，起飞时间段");
				int s, t, k;  cin >> s >> t >> k;
				tim T1, T2; string str;
				cin >> str; T1.date = str[2] - '0';
				cin >> str;
				int i = 0; 
				while(str[i] != ':') T1.h = T1.h * 10 + str[i] - '0', ++i;
				++i;
				while(str[i]) T1.m = T1.m * 10 + str[i] - '0', ++i;
				cin >> str; T2.date = str[2] - '0';
				cin >> str; 
				i = 0; 
				while(str[i] != ':') T2.h = T2.h * 10 + str[i] - '0', ++i;
				++i;
				while(str[i]) T2.m = T2.m * 10 + str[i] - '0', ++i;
				ShortestPath1::Main5(s, t, k, T1, T2);
			} 
			else if(opt2 == 2){
				puts("输入起飞降落机场，中转次数，降落时间段");
				int s, t, k;  cin >> s >> t >> k;
				tim T1, T2; string str;
				cin >> str; T1.date = str[2] - '0';
				cin >> str;
				int i = 0; 
				while(str[i] != ':') T1.h = T1.h * 10 + str[i] - '0', ++i;
				++i;
				while(str[i]) T1.m = T1.m * 10 + str[i] - '0', ++i;
				cin >> str; T2.date = str[2] - '0';
				cin >> str; 
				i = 0; 
				while(str[i] != ':') T2.h = T2.h * 10 + str[i] - '0', ++i;
				++i;
				while(str[i]) T2.m = T2.m * 10 + str[i] - '0', ++i;
				ShortestPath1::Main6(s, t, k, T1, T2);
			}
			else {
				puts("输入起飞降落机场，中转次数，机型");
				int s, t, k, model;  cin >> s >> t >> k >> model;
				ShortestPath1::Main7(s, t, k, model);
			}
		}
		else if(opt1 == 6){
			while(1){
				puts("***********************"); 
				puts("输入操作数:           *"); 
				puts("1：给定起飞时间段     *"); 
				puts("2：给定降落时间段     *"); 
				puts("3：给定机型           *"); 
				puts("***********************"); 
				cin >> opt2;
				if(opt2 >= 1 && opt2 <= 3) break;
				puts("错误，请输入正确的操作数：");
			}
			if(opt2 == 1){
				puts("输入起飞降落机场，起飞时间段");
				int s, t;  cin >> s >> t;
				tim T1, T2; string str;
				cin >> str; T1.date = str[2] - '0';
				cin >> str;
				int i = 0; 
				while(str[i] != ':') T1.h = T1.h * 10 + str[i] - '0', ++i;
				++i;
				while(str[i]) T1.m = T1.m * 10 + str[i] - '0', ++i;
				cin >> str; T2.date = str[2] - '0';
				cin >> str; 
				i = 0; 
				while(str[i] != ':') T2.h = T2.h * 10 + str[i] - '0', ++i;
				++i;
				while(str[i]) T2.m = T2.m * 10 + str[i] - '0', ++i;
				ShortestPath1::Main2(s, t, T1, T2);
			} 
			else if(opt2 == 2){
				puts("输入起飞降落机场，降落时间段");
				int s, t;  cin >> s >> t;
				tim T1, T2; string str;
				cin >> str; T1.date = str[2] - '0';
				cin >> str;
				int i = 0; 
				while(str[i] != ':') T1.h = T1.h * 10 + str[i] - '0', ++i;
				++i;
				while(str[i]) T1.m = T1.m * 10 + str[i] - '0', ++i;
				cin >> str; T2.date = str[2] - '0';
				cin >> str; 
				i = 0; 
				while(str[i] != ':') T2.h = T2.h * 10 + str[i] - '0', ++i;
				++i;
				while(str[i]) T2.m = T2.m * 10 + str[i] - '0', ++i;
				ShortestPath1::Main3(s, t, T1, T2);
			}
			else {
				puts("输入起飞降落机场，机型");
				int s, t, model;  cin >> s >> t >> model;
				ShortestPath1::Main4(s, t, model);
			}
		}
		else if(opt1 == 7){
			puts("输入起飞降落机场，中转次数，中转时长");
			int s, t, k1, k2;  cin >> s >> t >> k1 >> k2;
			ShortestPath2::Main1(s, t, k1, k2);			
		}
		else if(opt1 == 8){
			puts("输入起飞降落机场，中转时长");
			int s, t, k; cin >> s >> t >> k;
			ShortestPath2::Main2(s, t, k);
			
		}
		else {
			puts("关闭"); 
			puts("-----------------------------------------------"); 
			break;
		}
		puts("");  
	}
}
 