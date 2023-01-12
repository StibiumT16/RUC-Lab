#include<bits/stdc++.h>
using namespace std;
const int N=1e5+10;

struct treap{
	int ch[N][2],val[N],pri[N],siz[N],tot,rt,x,y,z;
	treap(){rt=tot=0; srand((unsigned)time(NULL));}
	
	void pushup(int x){siz[x]=siz[ch[x][0]]+siz[ch[x][1]]+1;}
	
	int newnode(int v){siz[++tot]=1,val[tot]=v,pri[tot]=rand(); return tot;}
	
	int merge(int x, int y){
		if(!x||!y) return x|y;
		if(pri[x]<pri[y]){ch[x][1]=merge(ch[x][1],y), pushup(x); return x;}
		else {ch[y][0]=merge(x,ch[y][0]), pushup(y); return y;}
	}
	
	void split(int p,int k,int &x,int &y){
		if(!p) return (void)(x=y=0);
		if(val[p]<=k) x=p,split(ch[p][1],k,ch[p][1],y);
		else y=p,split(ch[p][0],k,x,ch[p][0]);
		pushup(p);
	}
	
	void Insert(int v){split(rt,v,x,y), rt=merge(merge(x,newnode(v)),y);}
	
	void Delete(int v){//É¾1¸ö 
		split(rt,v,x,z), split(x,v-1,x,y);
		y=merge(ch[y][0],ch[y][1]), rt=merge(merge(x,y),z);
	}
	
	void Erase(int v){//É¾È«²¿ 
		split(rt,v,x,z), split(x,v-1,x,y); 
		rt=merge(x,z); 
	}
		
	int val2rank(int v){
		split(rt,v-1,x,y);
		int res=siz[x]+1;
		rt=merge(x,y);
		return res;
 	}
 	
 	int rank2id(int p,int k){
 		while(1){
 			if (k<=siz[ch[p][0]]) p=ch[p][0];
        	else if(k==siz[ch[p][0]]+1) return p;
        	else k-=siz[ch[p][0]]+1,p=ch[p][1];
		}
	}
 	
 	int rank2val(int k){return val[rank2id(rt,k)];}
	
	int id2rank(int k){return val2rank(val[k]);}
	
	int pre(int v){
		split(rt,v-1,x,y);
		int res=val[rank2id(x,siz[x])];
		rt=merge(x,y);
		return res; 
	}
	
	int suf(int v){
		split(rt,v,x,y);
		int res=val[rank2id(y,1)];
		rt=merge(x,y);
		return res; 
	}
};

inline int read(){
	int x=0,f=1;char ch=getchar();
	while (!isdigit(ch)){if (ch=='-') f=-1;ch=getchar();}
	while (isdigit(ch)){x=x*10+ch-48;ch=getchar();}
	return x*f;
}

int main(){
	treap t;
	int n=read();
	while(n--){
		int opt=read(), x=read();
		if(opt == 1) t.Insert(x);
		else if(opt == 2) t.Delete(x);
		else if(opt == 3) printf("%d\n",t.val2rank(x));
		else if(opt == 4) printf("%d\n",t.rank2val(x));
		else if(opt == 5) printf("%d\n",t.pre(x));
		else printf("%d\n",t.suf(x));
	}
}
