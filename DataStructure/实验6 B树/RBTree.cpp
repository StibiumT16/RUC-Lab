#include <bits/stdc++.h>
using namespace std;
const int inf = 2147483647;

template<typename T> 
class RBTree{
#define ls x->ch[0]
#define rs x->ch[1]

private:
	typedef struct Node{
		T val;
		int siz, cnt;
		bool col;
		Node *fa, *ch[2];
		Node(T _val, int _siz, int _cnt, bool _col, Node* _fa, Node* lc, Node* rc) : 
			val(_val), siz(_siz), cnt(_cnt), col(_col){fa = _fa, ch[0] = lc, ch[1] = rc;} 
	}Node;
	Node *rt, *NIL;
	
	void pushup(Node* x) {x->siz = ls->siz + rs->siz + x->cnt;}
	
	bool get(Node* x){return x == x->fa->ch[1];}
	
	void update(Node* x){for(Node* i = x; i != NIL; i = i->fa) i->siz--;}
	
	Node* newnode(T val){ return (new Node(val, 1, 1, 1, NIL, NIL, NIL));}
	
	void rotate(Node* x, bool c){
		Node* y = x->ch[!c], *f = x->fa; 
		x->ch[!c] = y->ch[c];
		if(y->ch[c] != NIL) y->ch[c]->fa = x;
    	y->fa = f;
    	if(f == NIL) rt = y;
    	else f->ch[get(x)] = y;
    	y->ch[c] = x, x->fa = y;
    	pushup(x), pushup(y);
	}
	
	void Insert_fix(Node* x) {
		if(x->fa){
			while(x->fa->col){
        		Node* f = x->fa, *gf = f->fa;
        		bool wh = !get(f);
        		Node* ul = gf->ch[wh];
        		if(ul->col)  f->col = ul->col = 0, gf->col = 1, x = gf;
				else{
					if(x == f->ch[wh]) rotate(x = f, !wh);
					else gf->col = 1, f->col = 0, rotate(gf, wh);
    			}
    		}
    	}
    	rt->col = 0;
	}
	
	void Delete_fix(Node* x) {
	    while(x != rt && !x->col) {
	        bool wh = !get(x);
	        Node* f = x->fa, *ul = f->ch[wh];
        	if(ul->col){
            	ul->col = 0, f->col = 1;
            	rotate(f, !wh);
            	ul = f->ch[wh];
        	}
			else{
				if(!ul->ch[0]->col && !ul->ch[1]->col) ul->col = 1, x = f;
				else{
            		if(!ul->ch[wh]->col){
                		ul->ch[!wh]->col = 0, ul->col = 1;
                		rotate(ul, wh);
                		ul = f->ch[wh];
            		}
            		ul->col = f->col, ul->ch[wh]->col = f->col = 0;
            		rotate(f, !wh);
            		x = this->rt;
            		break;
        		}
    		}
    	}
    	x->col = 0;
	}
	

public:
	RBTree(){
		this->NIL = new Node(0, 0, 0, 0, NULL, NULL, NULL);
		NIL->fa = NIL->ch[0] = NIL->ch[1] = NIL;
		this->rt = NIL;
	}
	
	void insert(T val) {
    	Node* x = rt, *f = NIL;
    	while(x != NIL){
        	++x->siz, f = x;    
        	if(val == x->val){x->cnt++; return; }
        	x = x->ch[val > x->val];
    	}
    	x = newnode(val);
		if(f != NIL) f->ch[val > f->val] = x;
    	else rt = x;
    	x->fa = f, Insert_fix(x);
	}
	
	void del(T val) {
		Node* x = rt;
		while(x != NIL && x->val != val) x = x->ch[x->val < val];
    	if(x == NIL) return;
    	if(x->cnt > 1) {--x->cnt, update(x); return;}
    	Node* p = x, *g = NIL;
    	if(ls != NIL && rs != NIL){
    	    p = rs;
    	    while(p->ch[0] != NIL) p = p->ch[0];
    	}
    	g = p->ch[p->ch[0] == NIL];
	    g->fa = p->fa;
	    if(p->fa == NIL) rt = g;
	    else p->fa->ch[get(p)] = g;
	    if(x != p) x->val = p->val, x->cnt = p->cnt;
	    update(p->fa);
	    for(Node* i = p->fa; i != NIL && p->cnt > 1 && i != x ; i = i->fa)  i->siz -= p->cnt, ++i->siz;
	    if (!p->col) Delete_fix(g);
	    free(p);
	}

	int val2rank(T val){
		int res = 1;  Node* x = rt;
		while(x != NIL){
			if(val < x->val) x = ls;
			else {
				res += ls->siz;
				if(val == x->val)  return res;
				res += x->cnt, x = rs;
			}
		}
		return res;
	}

	T rank2val(int k) {
    	Node* x = rt;
   		while(x != NIL){
       		int tmp = ls->siz;
       		if (tmp + 1 <= k && k <= tmp + x->cnt) break;
        	else{
				if (k <= tmp) x = ls;
				else k -= tmp + x->cnt, x = rs;
			}
    	}
    	return x->val;
	}
	
	T pre(T val) {
   	 	T res = -inf; Node* x = rt;
    	while(x != NIL){
        	if(x->val < val) res = x->val, x = rs;
        	else x = ls;
    	}
		return res;
	}
	
	T suf(T val) {
    	T res = inf; Node* x = rt;
    	while(x != NIL){
        	if(x->val > val) res = x->val, x = ls;
        	else x = rs;
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
	RBTree<int> rbt;
	int n=read();
	while(n--){
		int opt = read(), x = read();
		if(opt == 1) rbt.insert(x);
		else if(opt == 2) rbt.del(x);
		else if(opt == 3) printf("%d\n",rbt.val2rank(x));
		else if(opt == 4) printf("%d\n",rbt.rank2val(x));
		else if(opt == 5) printf("%d\n",rbt.pre(x));
		else printf("%d\n",rbt.suf(x));
	}
}
