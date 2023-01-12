#include <bits/stdc++.h>
using namespace std;

const int M = 50;//M >= 2
const int ch_max = M << 1;
const int ch_min = M;
const int key_max = ch_max - 1;
const int key_min = ch_min - 1;
const int inf = 2147483647;

template <typename T>
class BTree{
#define fi first
#define se second
#define mp make_pair
typedef pair<T, int> Type;

private:
    struct Node{
        Type val[key_max];
        Node *ch[ch_max] = {NULL}, *fa = NULL;
        int keyNum = 0, siz = 0;
        bool isLeaf = true;
        T key(int i) {return val[i].fi;}
        int &cnt(int i) {return val[i].se;}
        Node(Node *fa = NULL) : fa(fa) {}
    };
    Node *rt = NULL;
	
	int calsiz(Node *x){
        if (!x) return 0;
        int res = 0;
        for (int i = 0; i < x->keyNum; ++i) res += getsiz(x->ch[i]) + x->cnt(i);
        res += getsiz(x->ch[x->keyNum]);
        return res;
    }
    
    int getsiz(Node *x){
    	if(!x) return 0;
    	return x->siz;
	}
	
	void split(Node *x, int k){//将x的k儿子y分裂为2个儿子y,z
		Node *z = new Node(x), *y = x->ch[k]; 
        z->isLeaf = y->isLeaf;
        memmove(z->val, y->val + M, key_min * sizeof(Type));
        if(!y->isLeaf){ 
        	memmove(z->ch, y->ch + M, ch_min * sizeof(Node*));
        	for(int i = 0; i < ch_min; ++i) z->ch[i]->fa = z;
		}
		z->keyNum = y->keyNum = key_min;
		memmove(x->ch + k + 2, x->ch + k + 1, (x->keyNum - k) * sizeof(Node*));
		x->ch[k + 1] = z;
		memmove(x->val + k + 1, x->val + k, (x->keyNum - k) * sizeof(Type));
		x->val[k] =  y->val[key_min];
		y->siz = calsiz(y), z->siz = calsiz(z);
		++x->keyNum;
	}
	
	void merge(Node *x, int k){//将x的k儿子与k+1儿子合并，两个孩子都只有BF-1个孩子，合并后有2*BF-1个 
		Node *y = x->ch[k], *z = x->ch[k + 1];
		y->keyNum = key_max;
		y->val[key_min] = x->val[k];
		memmove(y->val + M, z->val, key_min * sizeof(Type));
		if(!y->isLeaf){
			memmove(y->ch + M, z->ch, ch_min * sizeof(Node*));
			for(int i = ch_min; i < ch_max; ++i) y->ch[i]->fa = y;
		}
		memmove(x->val + k, x->val + k + 1, (x->keyNum -k - 1) * sizeof(Type));
		memmove(x->ch + k + 1, x->ch + k + 2, (x->keyNum -k - 1) * sizeof(Node*));
		--x->keyNum, y->siz = calsiz(y);
		free(z);
	}
	
	void insert_Nonfull(Node *x, T val){
		while(1){ 
			int i = lower_bound(x->val, x->val + x->keyNum, val, [](const Type &a, const T &b) {return a.fi < b;}) - x->val;
			if(i != x->keyNum && val == x->val[i].fi){
				++x->cnt(i);
				while(x) ++x->siz, x = x->fa;
				return;
			}
			if(x->isLeaf){
				memmove(x->val + i + 1, x->val + i, (x->keyNum - i) * sizeof(Type));
				x->val[i] = mp(val, 1);
				++x->keyNum;
				while(x) ++x->siz, x = x->fa;
				return;
			}
			if(x->ch[i]->keyNum == key_max){//查找新关键字的位置时就沿途分裂遇到的每个满节点，保证分裂每个满节点时其父亲必不满
				split(x, i);
				if(x->key(i) < val) ++i;
				else if (val == x->key(i)){
					++x->cnt(i);
					while(x) ++x->siz, x = x->fa;
					return;
				}
			}
			x = x->ch[i];
		}
	}
	
	void erase(Node *x, T val){
		int i = lower_bound(x->val, x->val + x->keyNum, val, [](const Type &a, const T &b) {return a.fi < b;}) - x->val; 
		if(i != x->keyNum && val == x->val[i].fi){
			if(x->cnt(i) > 1){
				--x->cnt(i);
				while(x) --x->siz, x = x->fa;
				return;
			}
			if(x->isLeaf){//叶节点，直接删除 
				memmove(x->val + i, x->val + i + 1, (--x->keyNum - i) * sizeof(Type));
				while(x) --x->siz, x = x->fa;
			}
			else{
				if(x->ch[i]->keyNum >= M){//前驱所在儿子有足够多儿子 
					Node *y = x->ch[i];
					while(!y->isLeaf) y = y->ch[y->keyNum];//找前驱
					x->val[i] = y->val[y->keyNum - 1]; 
					if(x->cnt(i) != 1){ //y的对应节点cnt有多个，那么沿路减siz;只有一个单独处理 
						y->cnt(y->keyNum - 1) = 1;
						while(y != x) y->siz -= x->cnt(i) - 1, y = y->fa;
					}
					erase(x->ch[i], x->key(i));
				}
				else if(x->ch[i + 1]->keyNum >= M){//后继所在儿子有足够多儿子 
					Node *y = x->ch[i + 1];
					while(!y->isLeaf) y = y->ch[0];//找后继 
					x->val[i] = y->val[0]; 
					if(x->cnt(i) != 1){ 
						y->cnt(0) = 1;
						while(y != x) y->siz -= x->cnt(i) - 1, y = y->fa;
					}
					erase(x->ch[i + 1], x->key(i));
				}
				else{//否则合并节点 
					merge(x, i); 
					if(!rt->keyNum) rt = x->ch[i], rt->fa =NULL;//没有键，但是还可能有一个儿子，根变成该儿子
					erase(x->ch[i], val); 
				}
			}
		}
		else if(!x->isLeaf){
			if(x->ch[i]->keyNum == key_min){//需要合并的情况 
				Node *y = x->ch[i];
				if(i >= 1 && x->ch[i - 1]->keyNum >= M){//找左兄弟借节点，把x的一个键移入要删的key所在孩子，把它的兄弟的一个key和孩子移入x
					Node *z = x->ch[i - 1]; 
					memmove(y->val + 1, y->val, y->keyNum * sizeof(Type));
					//new(y->val) Type(move(x->val[i-1]));
					//new(x->val + i -1) Type(move(z->val[z->keyNum - 1]));
					y->val[0] = x->val[i-1];
					x->val[i - 1] = z->val[z->keyNum - 1];
					if(!y->isLeaf){
						memmove(y->ch + 1, y->ch, (y->keyNum + 1) * sizeof(Node*));
						y->ch[0] = z->ch[z->keyNum], y->ch[0]->fa = y;
					}
					--z->keyNum, ++y->keyNum;
					y->siz = calsiz(y), z->siz = calsiz(z);
					erase(y, val);
				}
				else if(i < x->keyNum && x->ch[i + 1]->keyNum >= M){
					Node *z = x->ch[i + 1]; 
					//new(y->val + y->keyNum) Type(move(x->val[i]));
					//new(x->val + i) Type(move(z->val[0]));
					y->val[y->keyNum] = x->val[i];
					x->val[i] = z->val[0];
					if(!y->isLeaf){
						y->ch[y->keyNum + 1] = z->ch[0], y->ch[y->keyNum + 1]->fa = y;
						memmove(z->ch , z->ch + 1, z->keyNum * sizeof(Node*));
					}
					memmove(z->val, z->val + 1, (z->keyNum - 1) * sizeof(Type));
					--z->keyNum, ++y->keyNum;
					y->siz = calsiz(y), z->siz = calsiz(z);
					erase(y, val);
				}
				else {//两个兄弟都没有节点借,则与左右任一兄弟合并
					if(i) --i;
					y = x->ch[i];
					merge(x, i);
					if(!rt->keyNum) rt = y, rt->fa = NULL;
					erase(y, val); 
				}
			}
			else erase(x->ch[i], val); 
		}
		//如果是叶节点还没有找对对应的值则树中无该值 
	}
	
public:
	BTree() {rt = new Node;}
	
	void Insert(T val){
		if(rt->keyNum == key_max){
			Node *x = new Node;
			x->isLeaf = false, x->ch[0] = rt, x->siz = rt->siz;
			rt->fa = x; rt = x;
			split(x, 0); 
		}  
		insert_Nonfull(rt, val);//插入到已经存在的叶节点上 
	}

	void Delete(T val){erase(rt, val);}	

	int Rank(T val){
		Node *x = rt;
		int res = 0;
		while(x){
			if(x->key(x->keyNum - 1) < val){
				res += x->siz - getsiz(x->ch[x->keyNum]);
				x = x->ch[x->keyNum];
				continue;
			}
			for(int i = 0; i < x->keyNum; ++i){
				if(x->key(i) < val) res += getsiz(x->ch[i]) + x->cnt(i);
				else if(x->key(i) == val) return res + getsiz(x->ch[i]) + 1;
				else {x = x->ch[i]; break;}			
			}
		}
		return res;
	}
	
	T Kth(int k){
		Node *x = rt;
		while(x){
			bool flag = 0;
			for(int i = 0; i<= x->keyNum; ++i){
				int l = getsiz(x->ch[i]) + 1, r = getsiz(x->ch[i]) + (i == x->keyNum ? 1 : x->cnt(i));
				if(k >= l && k <= r) return x->key(i);
				if(k < l){x = x->ch[i]; flag = 1; break;}
				k -= r;
			}
			if(!flag) return 0;
		}
	}
	
	T Predecessor(T val) {
   	 	T res = -inf; Node* x = rt;
    	while(x){
        	int i = lower_bound(x->val, x->val + x->keyNum, val, [](const Type &a, const T &b) {return a.fi < b;}) - x->val;
        	if(i) res = x->key(i-1);
        	x = x->ch[i];
    	}
		return res;
	}
	
	T Successor(T val) {
   	 	T res = inf; Node* x = rt;
    	while(x){
        	int i = upper_bound(x->val, x->val + x->keyNum, val, [](const T &b, const Type &a) {return a.fi > b;}) - x->val;
        	if(i != x->keyNum) res = x->key(i);
        	x = x->ch[i];
    	}
		return res;
	}

#undef fi
#undef se
#undef mp
};

inline int read(){
	int x=0,f=1;char ch=getchar();
	while (!isdigit(ch)){if (ch=='-') f=-1;ch=getchar();}
	while (isdigit(ch)){x=x*10+ch-48;ch=getchar();}
	return x*f;
}

int main()
{
	BTree<int> T;
	int _ = read();
	while(_--){
		int opt = read(), x = read();
		switch (opt)
        {
        case 1:
            T.Insert(x);
            break;
        case 2:
            T.Delete(x);
            break;
        case 3:
            printf("%d\n",T.Rank(x));
            break;
        case 4:
            printf("%d\n",T.Kth(x));
            break;
        case 5:
            printf("%d\n",T.Predecessor(x));
            break;
        case 6:
            printf("%d\n",T.Successor(x));
            break;
        }
	}
}

