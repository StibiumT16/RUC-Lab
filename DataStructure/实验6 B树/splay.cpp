#include<bits/stdc++.h>
using namespace std;
const int N=1e5+10;
const int inf=0x7fffffff;

class Splay{
private:
int tot,rt,fa[N],ch[N][2],val[N],num[N],siz[N];
void pushup(int x){siz[x]=siz[ch[x][0]]+siz[ch[x][1]]+num[x];}
bool get(int x){return x==ch[fa[x]][1];}
void clear(int x){ch[x][0]=ch[x][1]=fa[x]=siz[x]=val[x]=num[x]=0;}

void rotate(int x){
	int y=fa[x],z=fa[y],chk=get(x);
	ch[y][chk]=ch[x][chk^1];
	if(ch[x][chk^1]) fa[ch[x][chk^1]]=y;
	ch[x][chk^1]=y; fa[y]=x; 
	fa[x]=z; if(z) ch[z][y==ch[z][1]]=x;
	pushup(y),pushup(x);
}

void splay(int x,int goal){
	for(register int f;(f=fa[x])!=goal;rotate(x)) 
		if(fa[f]!=goal) rotate(get(x)==get(f)?f:x);
	if(!goal) rt=x;
}

public:
Splay(){
	this->tot=this->rt=0;
	this->insert(-inf);
	this->insert(inf);
}	

void insert(int k){
	if(!rt){val[++tot]=k,++num[tot],rt=tot,pushup(rt); return;} 
	int cur=rt,f=0;
	while(cur&&val[cur]!=k) f=cur,cur=ch[cur][val[cur]<k];
	if(cur) ++num[cur];
	else{
		cur=++tot,fa[cur]=f,val[cur]=k,++num[cur];
		if(f) ch[f][val[f]<k]=cur;
	}
	splay(cur,0);
}

int val2rank(int k){
	int res=1,cur=rt;
	while(cur){
		if(k<val[cur]) cur=ch[cur][0];
		else {
			res+=siz[ch[cur][0]];
			if(k==val[cur]){splay(cur,0); return res;}
			res+=num[cur],cur=ch[cur][1];
		}
	}
	return res;
}

int val2id(int k){
	int cur=rt;
	while(1){
		if(k==val[cur]) {splay(cur,0); return cur;}
		else cur= k<val[cur] ? ch[cur][0] : ch[cur][1];
	}
}

int rank2id(int k){
	int cur=rt;
	if(k>siz[rt]) return 0;
	while(1){
		if(ch[cur][0]&&k<=siz[ch[cur][0]]) cur=ch[cur][0];
		else{
			k-=num[cur]+siz[ch[cur][0]];
			if(k<=0){splay(cur,0); return cur;}
			cur=ch[cur][1];
		}
	}
}

int rank2val(int k){return val[rank2id(k)];}

int id2rank(int k){return val2rank(val[k]);}

void del(int k){
	int pre=rank2id(val2rank(k)-1);
	int suf=rank2id(val2rank(k+1));
	splay(pre,0),splay(suf,pre);
	int del=ch[suf][0];
	if(num[del]>1) num[del]--,splay(del,0);
	else ch[suf][0]=0;
}

int pre(int x){return rank2val(val2rank(x)-1);}
int suf(int x){return rank2val(val2rank(x+1));}
}; 

inline int read(){
	int x=0,f=1;char ch=getchar();
	while (!isdigit(ch)){if (ch=='-') f=-1;ch=getchar();}
	while (isdigit(ch)){x=x*10+ch-48;ch=getchar();}
	return x*f;
}

int main()
{
	int n=read();
	Splay d;
	while(n--){
		int opt=read(),x=read();
		if(opt==1) d.insert(x);
		else if(opt==2) d.del(x);
		else if(opt==3) printf("%d\n",d.val2rank(x)-1);
		else if(opt==4) printf("%d\n",d.rank2val(x+1));
		else if(opt==5) printf("%d\n",d.pre(x));
		else printf("%d\n",d.suf(x));
	}
}
