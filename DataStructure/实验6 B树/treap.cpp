#include<bits/stdc++.h>
using namespace std;
const int N=1e5+10;
const int inf = 0x7fffffff;

class Treap{
private:
	typedef struct Node{
		int val, pri, siz, cnt;
		Node *ls, *rs;
		Node(int _val, Node* _ls = NULL, Node* _rs = NULL) : val(_val){this->ls = _ls; this->rs = _rs; this->siz = this->cnt = 1; this->pri = rand();} 
	}Node;
	
	void pushup(Node *x){x->siz = x->cnt + (x->ls == NULL ? 0 : x->ls->siz) + (x->rs == NULL ? 0 : x->rs->siz);}
	
	Node* zig(Node *x){
		Node *y = x->ls;
		x->ls = y->rs, y->rs = x;
		y->siz = x->siz;
		pushup(x);
		x = y;
		return x;
	}
	
	Node* zag(Node *x){
		Node *y = x->rs;
		x->rs = y->ls, y->ls = x;
		y->siz = x->siz;
		pushup(x);
		x = y;
		return x;
	}

public:
	Node *rt;	
	
	Treap(){
		this->rt = NULL;
	}
	
	void insert(Node * &x, int val){
		if(!x){
			x = new Node(val, NULL, NULL);
			return;
		}
		++x->siz;
		if(x->val == val) return (void)(++x->cnt); 
		if(val > x->val){
			insert(x->rs, val);
			if(x->rs->pri > x->pri) x = zag(x);
		}
		else{
			insert(x->ls, val);
			if(x->ls->pri > x->pri) x = zig(x);
		}
	}
	
	void del(Node *&x, int val){
		if(x == NULL) return;
		if(x->val == val){
			Node *tmp;
			if(x->cnt > 1) --x->cnt, --x->siz;
			else if(!x->ls){
				tmp = x->rs;
				free(x);
				x = tmp;
			}
			else if(!x->rs){
				tmp = x->ls;
				free(x);
				x = tmp;
			}
			else if(x->ls->pri > x->rs->pri) x = zig(x), del(x, val);
			else x = zag(x), del(x, val);
			return; 
		}
		--x->siz;
		if(val > x->val) del(x->rs, val);
		else del(x->ls, val);
	}
	
	int val2rank(int val){
		int res = 1;  Node* x = rt;
		while(x){
			if(val < x->val) x = x->ls;
			else {
				res += x->ls ? x->ls->siz : 0;
				if(val == x->val)  return res;
				res += x->cnt, x = x->rs;
			}
		}
		return res;
	}
	
	int rank2val(int k) {
    	Node* x = rt;
   		while(x){
       		int tmp = x->ls ? x->ls->siz : 0;
       		if(tmp < k && tmp + x->cnt >= k) return x->val;
       		if(tmp >= k) x = x->ls;
       		else k -= tmp + x->cnt, x = x->rs; 
    	}
    	return 0;
	}
	
	int pre(int val){
		Node *x = rt; int res = -inf;
			while(x){
        	if(x->val < val) res = x->val, x = x->rs;
        	else x = x->ls;
    	}
		return res; 
	}
	
	int suf(int val){
		Node *x = rt; int res = inf;
			while(x){
        	if(x->val > val) res = x->val, x = x->ls;
        	else x = x->rs;
    	}
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
	Treap t;
	int n=read();
	while(n--){
		int opt = read(), x = read();
		if(opt == 1) t.insert(t.rt, x);
		else if(opt == 2) t.del(t.rt, x);
		else if(opt == 3) printf("%d\n",t.val2rank(x));
		else if(opt == 4) printf("%d\n",t.rank2val(x));
		else if(opt == 5) printf("%d\n",t.pre(x));
		else printf("%d\n",t.suf(x));
	}
}
