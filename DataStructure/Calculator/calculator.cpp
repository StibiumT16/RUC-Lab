#include<bits/stdc++.h>
using namespace std; 

#define MARSIZE 110 
#define MaxSize 100
#define ElemType double
#define STACK_INIT_SIZE 100 
#define STACKINCREMENT 10 
#define Status int
#define LIST_INIT_SIZE 100 
#define STRINGSIZE 1000
#define LISTINCREMENT 10 
#define ERROR 0
#define OK 1
#define INFEASIBLE -1
#define TRUE 1
#define FALSE 0
#define OVERFLOW 2

namespace SQLIST{//用SQLIST命名空间封装顺序表部分 
typedef struct{
    ElemType *elem;
    int length,listsize;
    
	void clear(){length = 0;}//清空线性表 
	 
    Status init(){//初始化线性表 
    	elem=(ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
    	if(!elem) exit(OVERFLOW);                                                 
    	length=0;
    	listsize =  LIST_INIT_SIZE;
    	return OK;
	}
	
	void destory(){//销毁线性表 
		delete(elem);
		elem=NULL;
		length=listsize=0;
	}
	
	ElemType get(int pos){//取线性表上某一位置的元素 
		if(pos <= 0 || pos > length) return 0;
		return elem[pos-1];
	}
	
	Status insert(int i, ElemType e){//在线性表的第i位置插入元素e 
   		if(i < 1 || i > length+1) return ERROR;
    	if(length>=listsize){
    		ElemType *newbase = (ElemType *)realloc(elem, (listsize + LISTINCREMENT) * sizeof(ElemType));
			if(!newbase) exit(OVERFLOW);
			elem = newbase;
			listsize += LISTINCREMENT;
		} 
		ElemType *q = &(elem[i-1]), *p=&elem[length-1];
		for(; p >= q; --p) *(p + 1) = *p;
		*q = e;
		++length;
		return OK; 
	}
	
	Status del(int i, ElemType &e){//删除线性表上i位置的元素e 
		if(i < 1 || i > length) return ERROR;
		ElemType *p = &(elem[i-1]);
		e = *p;
		ElemType *q = elem + length - 1;
		for(++p; p <= q; ++p) *(p - 1) = *p;
		--length;
		return OK;
	}	
}SqList;

Status TransList_Sq(SqList &A, SqList &B){//将线性表B转移到线性表A上并清空B 
	A.clear();
	for(int i = 1; i<= B.length; ++i)
		A.insert(i, B.get(i));
	B.clear();
} 
}
using namespace SQLIST;

namespace OPT{//用OPT命名空间封装运算操作部分 
void Add(SqList A, SqList B, SqList &C, int on){//加法和减法操作，由on控制 
	int len = max(A.length, B.length);
	for(int i = 1; i <= len; ++i) C.insert(i ,A.get(i) + on * B.get(i));
} 

void Mul(SqList A, SqList B, SqList &C){//乘法操作 
	if((A.length == 1 && A.get(1) == 0) || (B.length == 1 && B.get(1) == 0)){
		C.insert(1, 0);
		return;
	}
	for(int i = 1; i <= A.length + B.length - 1 ;++i) C.insert(i,0);
	for(int i = 1; i <= A.length; ++i)
		for(int j = 1; j <= B.length; ++j)
			C.elem[i+j-2] += A.get(i) * B.get(j);
}

void dev(SqList A, SqList &B){//求导操作 
	int len = A.length;
	if(len == 1) {
		B.insert(1, 0);
		return;
	}
	for(int i = 1; i < len ; ++i)
	B.insert(i, A.get(i + 1) * i);
}

void idev(SqList A, SqList &B){//求不定积分操作 
	int len = A.length;
	B.insert(1, 0);
	if(len == 1 && A.get(1) == 0) return;
	for(int i = 1; i <= len; ++i){
		B.insert(i+1, A.get(i)/i);
	}
}

double Cos(SqList A,SqList B){//求夹角余弦操作 
	double a = 0 ,b = 0, c = 0;
	for(int i = 1;i <= A.length; ++i){
		int x = A.elem[i - 1], y = B.elem[i - 1];
		a += 1.0 * x * y;
		b += 1.0 * x * x;
		c += 1.0 * y * y;
	}
	return a / (sqrt(b) * sqrt(c));
}

void Out(int opt,SqList &A, SqList &B,SqList &C){
	A.destory(), B.destory(), C.destory();
	if(opt == 1) puts("退出向量模式");
	else puts("退出多项式模式");
	puts("------------------------------------------------------------");
	puts(""); 
}
}
using namespace OPT;

namespace Vec{//用Vec命名空间封装向量部分 
void print_vec(SqList L){
	ElemType *p = L.elem;
	cout << "(";
	for(int i = 1; i <= L.length; ++i) {
		cout << *p++;
		if(i!=L.length) cout << ", ";
	}
	cout << ")\n";
}	
	
void input_vec(SqList &L,int n){
	ElemType x;
	printf("输入%d维向量Vec:\n",n);
	for(int i = 1; i <= n; ++i){
		cin >> x;
		L.insert(i,x);
	}
	print_vec(L);
}

void vec()
{
	int opt, n;
	SqList a, b, c;
	a.init(), b.init(), c.init();
	puts("输入向量维数:");
	while(1){
		cin>>n;
		if(n > 0) break;
		puts("请输入正确的维数"); 
	}
	input_vec(a,n);
	while(1){
		puts("输入操作 1：加法  2：减法  3：求夹角余弦  0：退出向量模式");
		while(1){
			cin>>opt;
			if(opt>=0&&opt<=3) break;
			puts("请输入正确的操作数"); 
		}
		if(!opt){
			Out(1,a,b,c);
			return; 
		} 
		if(opt == 1){
			input_vec(b,n);
			Add(a,b,c,1);
			puts("加法的结果为：");
			print_vec(c);
			puts("");
			TransList_Sq(a,c);
			b.clear(); 
		}
		else if(opt == 2){
			input_vec(b,n);
			Add(a,b,c,-1);
			puts("减法的结果为：");
			print_vec(c);
			puts("");
			TransList_Sq(a,c);
			b.clear();
		}
		else if(opt == 3) {
			input_vec(b,n);
			puts("求余弦夹角的结果为：");
			printf("%.6lf\n",Cos(a,b));
			Out(1,a,b,c);
			return;
		}
	}
}
}

namespace Poly{//用Poly命名空间封装多项式部分
void print_poly(SqList L){
	int len = L.length;
	if(len == 1){
		cout << L.get(1) <<endl;
		return;
	}
	for(int i = len; i >= 1; --i) 
	{
		ElemType x = L.get(i);
		if(x == 0) continue;
		if(i == len)  {
			if(x != 1 && x != -1) cout << x;
			else if(x == -1) cout<<"-";
			if(i != 2&& i != 1){
				cout << "x^";
				cout << i - 1;
			}
			else if(i == 2) cout << "x";
		}
		else if(i == 1) {
			if(x > 0) cout << "+";
			cout << x;
		}
		else if(i == 2){
			if(x > 0) cout<<"+";
			if(x !=1 && x!= -1) cout << x;
			else if(x == -1) cout << "-";
			cout << "x";
		}
		else{
			if(x > 0) cout << "+";
			if(x != 1 && x != -1) cout << x;
			else if(x == -1) cout << "-";
			cout << "x^" << i - 1;
		}
	}
	puts("");
}

void input_poly(SqList &L){
	ElemType x;
	int n;
	puts("输入多项式的阶deg");
	while(1){
		cin>>n;
		if(n>=0) break;
		puts("请输入正确的阶数"); 
	}
	printf("输入多项式从低到高各项系数(%d项)\n",n+1);
	for(int i = 1;i <= n + 1; ++i) {
		cin >> x;
		L.insert(i,x);
	}
	print_poly(L);
}

void poly()
{
	int opt;
	SqList a, b, c;
	a.init(), b.init(), c.init();
	input_poly(a);
	while(1)
	{
		puts("输入操作 1：加法  2：减法  3：乘法  4：求导  5:求不定积分  0：退出多项式模式");
		while(1){
			cin>>opt;
			if(opt>=0&&opt<=5) break;
			puts("请输入正确的操作数"); 
		}
		if(!opt){
			Out(2,a,b,c);
			return;
		}
		if(opt == 1) input_poly(b),Add(a,b,c,1),puts("加法的结果为：");
		else if(opt == 2) input_poly(b),Add(a,b,c,-1),puts("减法的结果为：");
		else if(opt == 3) input_poly(b),Mul(a,b,c),puts("乘法的结果为："); 
		else if(opt == 4) {
			int num;
			puts("请输入求导的阶数");
			while(1) {
				cin >> num; 
				if(num > 0) break;
				puts("请输入正确的阶数"); 
			}
			for(int i = 1; i <= num; ++i){
				dev(a,c);
				if(i != num) TransList_Sq(a,c);
			} 
			printf("求%d阶导的结果为：\n",num); 
		}
		else {
			int num;
			puts("请输入求不定积分的阶数");
			while(1) {
				cin >> num; 
				if(num > 0) break;
				puts("请输入正确的阶数"); 
			}
			for(int i = 1; i <= num; ++i){
				idev(a,c);
				if(i != num) TransList_Sq(a,c);
			} 
			printf("求%d阶不定积分的结果为：\n",num); 
		}
		print_poly(c);
		puts(""); 
		TransList_Sq(a,c);
		b.clear();
	}
}
}

template<typename T>
class Stack{//封装成STL风格的栈 
public:
	Stack(){
		this->Base = (T *) malloc (STACK_INIT_SIZE * sizeof (T)); 
		if(!this->Base) exit(OVERFLOW);
		this->Top = this->Base;
		this->stacksize = STACK_INIT_SIZE;
	}
	
	~Stack(){
		free(this->Base);
		this->Base = NULL;
		this->Top = NULL ;
		this->stacksize = 0;
	}
	
	bool empty(){ //判断栈是否为空 
		return this->Top == this->Base;
	}
	
	T top(){//取栈顶元素 
		if(this->Top != this->Base) 
		return *(this->Top - 1);
	}
	
	void push(T e){//压栈 
		if(this->Top - this->Base >= this->stacksize){
			this->Base = (T *) realloc (this->Base, (this->stacksize + STACKINCREMENT) * sizeof (T)); 
			if(!this->Base) exit(OVERFLOW);
			this->Top = this->Base + this->stacksize;
			this->stacksize +=  STACKINCREMENT;
		}
		*this->Top++ = e;
	}
	
	void pop(){//出栈 
		if(this->Top != this->Base)  --this->Top;
	}
		
private:
	int stacksize;
	T *Base;
	T *Top; 
};

class String{//封装成STL风格的字符串 
public:
	String(){
		this->ch = NULL;
		this->length = 0;
	} 
	
	String(char *t){
		int i; char *c;
		for(i = 0, c = t; *c; ++i, ++c);
		if(!i) this->ch = NULL, this->length = 0;
		else{
			if(!(this->ch = (char*) malloc (i * sizeof(char)))) exit(OVERFLOW);
			for(int j = 0; j < i; ++j) this->ch[j] = t[j];
			this->length = i;
		}
	} 
	
	Status assign(char *t){//生成一个值等于串常量t的串 
		if(this->ch) free(this->ch);
		int i; char *c;
		for(i = 0, c = t; *c; ++i, ++c);
		if(!i) this->ch = NULL, this->length = 0;
		else{
			if(!(this->ch = (char*) malloc (i * sizeof(char)))) exit(OVERFLOW);
			for(int j = 0; j < i; ++j) this->ch[j] = t[j];
			this->length = i;
		}
	}
	
	Status assign(String t){//生成一个值等于串常量t的串 
		if(this->ch) free(this->ch);
		int i = t.length;
		if(!i) this->ch = NULL, this->length = 0;
		else{
			if(!(this->ch = (char*) malloc (i * sizeof(char)))) exit(OVERFLOW);
			for(int j = 0; j < i; ++j) this->ch[j] = t.ch[j];
			this->length = i;
		}
	}
	
	void clear(){//清空串 
		if(this->ch) free(this->ch), this->ch = NULL;
		this->length = 0;
	}
	
	void insert(int pos, char chr){//重载insert，在S的第pos个字符前插入字符chr
		if(pos < 1 || pos > this->length + 1) return;
		if(!(this->ch = (char*) realloc (this->ch, (this->length + 1) * sizeof(char)))) exit(OVERFLOW);
		for(int i = this->length-1; i >= pos - 1; --i) this->ch[i + 1] = this->ch[i];
		this->ch[pos-1] = chr; 
		this->length++;
	}
	
	void insert(int pos, String T){//重载insert，在S的第pos个字符前插入串T 
		if(pos < 1 || pos > this->length + 1) return;
		if(T.length){
			if(!(this->ch = (char*) realloc (this->ch, (this->length + T.length) * sizeof(char)))) exit(OVERFLOW);
			for(int i = this->length-1; i >= pos - 1; --i) this->ch[i + T.length] = this->ch[i];
			for(int i = pos -1 ,j = 0; j <= T.length - 1; ++i, ++j) this->ch[i] = T.ch[j]; 
			this->length += T.length;
		}
	}
	
	void del(int pos, int len){//删除自pos位置起的len个字符 
		if(pos < 1 || pos + len > this->length + 1) return;
		for(int i = pos - 1, j = pos + len -1; j < this->length; ++i, ++j) this->ch[i] = this->ch[j];
		this->length -= len;
	}
	
	int compare(String T){//比较两字符串的值 
		for(int i = 0; i < this->length && i < T.length; ++i)
			if(this->ch[i] != T.ch[i]) return this->ch[i] - T.ch[i];
		return this->length - T.length;
	}
	
	String substring(int pos ,int len){//第pos个字符起长度为len的子串 
		String Sub; 
		if(pos < 1 || pos > this->length || len < 0 || len + pos > this->length + 1) return Sub;
		if(!len) Sub.ch = NULL, Sub.length = 0;
		else for(int i = 0; i < len; ++i) Sub.insert(i + 1, this->ch[pos - 1 + i]);
		return Sub;
	}
	
	int index(String t, int pos = 1){//字符串匹配 
		String Sub;
		if(pos <= 0 || pos > this->length) return ERROR;
		int len1 = this->length, len2 = t.length, i = pos;
		while(i + len2 <= len1 + 1){
			Sub = substring(i, len2);
			if(Sub.compare(t)) ++i;
			else return i;
		}
		return 0;
	}
	
	void replace(String s1, String s2){//将原串中的所有s1用s2替代 
		if(!s1.length) return;
		int i = index(s1, 1);
		while(i){
			del(i, s1.length);
			insert(i, s2);
			i += s2.length;
			i = index(s1, i); 
		}  
	}
	
	void print(){
		for(int i = 0; i < this->length; ++i) cout<<this->ch[i];
	}
	
	char *ch;
	int length;
}; 

namespace EXP{//用EXP命名空间封装表达式求值部分  
char precede(char a, char b){//比较运算符优先级 
	if(a == '+' || a == '-') {
		if(b == '+' || b == '-' || b == '#' || b == ')') return '>';
        else return '<';
	}
	else if (a == '*' || a == '/'){
		if(b == '(' || b == '^') return '<';
		else return '>'; 
	}
	else if(a == '^') {
		if(b == '(') return '<';
		else return '>'; 
	}
	else if(a == '('){
		if(b == ')') return '=';
        else return '<';
	}
	else if(a == ')') return '>';
	else {
		if(b == '#') return '=';
		else return '<';
	}
}

double Operate(ElemType a, char opt, ElemType b){//求值 
	switch(opt){
		case '+':
			return a + b;
			break;
		case '-':
			return a - b;
			break;
		case '*':
			return a * b;
			break;
		case '/':
			return a / b;
			break;
		case '^':
			return pow(a,b);
			break;
	}
}
 
Status check_bracket(String s){//检查括号匹配 
	int cnt = 0;
	for(int i = 0; i < s.length; ++i){
		if(s.ch[i] == '(') cnt++;
		if(s.ch[i] == ')')  if(!cnt--) return ERROR;
	}
	return !cnt;
}

Status Cal(String S,ElemType &res){//支持浮点数类型的四则运算表达式求值 
	if(!check_bracket(S)) return ERROR;//单独检查括号匹配是否错误 
	for(int i = 0; i < S.length; ++i){
		if(S.ch[i] == '.' || S.ch[i] == '(' || S.ch[i] == ')' || S.ch[i] == '+' || S.ch[i] == '-' || S.ch[i] == '*' || S.ch[i] == '/' || S.ch[i] == '^' ||  S.ch[i] == '#' || isdigit(S.ch[i])) continue;
		return ERROR; 
	}
	Stack<ElemType> OPND;
	Stack<char> OPTR;
	OPTR.push('#');
	int x = 0, f = 1, cnt = 1, i = 0;
	bool isfloat = 0, flag = 1;
	while(1){
		if(OPTR.empty()) return ERROR;
		if(S.ch[i] == '#' && OPTR.top() == '#') break;
		if(S.ch[i] == ' ') ++i;
		if(isdigit(S.ch[i])) {
			x = x * 10 + S.ch[i] - 48;
			if(isfloat) cnt *= 10;
			 ++i, flag = 1;
		}
		else if(S.ch[i]=='-' && ((i == 0 && (isdigit(S.ch[1]) || S.ch[1] == '.')) || ( i!= 0 && S.ch[i - 1]=='('))){
			OPND.push(-1);
			OPTR.push('*');
			i++, flag = 1;//判断负数 ,除了算式开头其他负数应该用括号括起来 
		}
		else if(S.ch[i]=='-' && i==0 && S.ch[1] == '(') OPND.push(0), OPTR.push('-'), ++i;//特判 -(...)的情况 
		else if(S.ch[i]=='.') isfloat = 1, ++i, flag =1;//判断小数
		else{
			if(flag && (isdigit(S.ch[i - 1]) || S.ch[i - 1] == '.')){
				if(isfloat) OPND.push(1.0 * x * f / cnt);
				else OPND.push(x * f);
				x = 0, f = 1, cnt = 1, isfloat = 0, flag = 0;
			}
			if(OPTR.empty()) return ERROR;
			char tmp = precede(OPTR.top(),S.ch[i]);
			if(tmp == '<') OPTR.push(S.ch[i++]), flag = 1;
			else if(tmp == '=') {
				OPTR.pop(), ++i, flag = 1;
			}
			else{
				if(OPTR.empty()) return ERROR;
				char theta = OPTR.top(); OPTR.pop();
				if(OPND.empty()) return ERROR;
				ElemType b = OPND.top(); OPND.pop();
				if(OPND.empty()) return ERROR;
				ElemType a = OPND.top(); OPND.pop();
				OPND.push(Operate(a,theta,b));
			}
		} 
	} 
	if(OPTR.empty() || OPTR.top() != '#') return ERROR;
	if(OPND.empty()) return ERROR;
	res = OPND.top(); OPND.pop();
	if(!OPND.empty()) return ERROR;
	return OK;
} 

String getvar(String s){//从表达式中提取变量名 
	String t; 
	int pos = 0, len = 0;
	while(!((s.ch[pos] >= 'A' && s.ch[pos] <='Z') || (s.ch[pos] >= 'a' && s.ch[pos] <= 'z') || s.ch[pos] =='_' )) ++pos;
	while(s.ch[pos + len] && ((s.ch[pos + len] >= 'A' && s.ch[pos + len] <='Z') ||  (s.ch[pos +len] >= 'a' && s.ch[pos + len] <= 'z') || s.ch[pos + len] =='_' || isdigit(s.ch[pos + len]))) ++len;
	return s.substring(pos + 1, len);
}

void exp(){
	while(1)
	{ 
		int opt;
		puts("输入操作 1：四则运算表达式求值  2：含单变量的表达式求值  0：退出表达式求值模式");
		while(1){
			cin>>opt;
			if(opt>=0&&opt<=2) break;
			puts("请输入正确的操作数"); 
		}
		if(!opt){
			puts("退出表达式求值模式");
			puts("------------------------------------------------------------");
			puts(""); 
			return;
		}
		puts("输入表达式");
		String s;  ElemType e;
		char t[STRINGSIZE], tt[STRINGSIZE];
		cin>>t;
		s.assign(t);
		s.insert(s.length + 1, '#');	
		if(opt==1) {//opt=1，直接求值 
			Status sta = Cal(s,e); 
			if(sta) cout << t << " = " << e << endl;
			else cout << "表达式错误！" << endl; 
		}
		else {//opt=2，将变量名用相应的数替换后求值 
			String Var = getvar(s), Num; 
			puts("输入单变量的数值");
			while(1){
				cin>>tt;
				int len = strlen(tt),flag=0;
				for(int i = 0; i < len; ++i) {
					if(!(isdigit(tt[i])||tt[i]=='.')){
						flag=1; break;
					}
				}
				if(!flag) break;
				puts("请输入正确的变量数值");
			}
			Num.assign(tt);
			s.replace(Var, Num);
			Status sta = Cal(s,e); 
			if(sta){
				Var.print();
				cout<< " = " << tt <<endl;
				cout << t << " = " << e <<endl;
			}
			else cout << "表达式错误！" << endl; 
			puts(""); 
		}
	}	
}
}

namespace FUNCTION{//用FUNCTION命名空间封装函数部分 
struct{String name,var,exp;} func[MaxSize];
int cnt = 0;

void Def(char *t){
	if(!((t[0] >= 'A' && t[0] <='Z') || (t[0] >= 'a' && t[0] <= 'z') || t[0] =='_' )){
		puts("错误的函数名");
		return; 
	}
	int len = 1, i = 0;
	while(t[i + 1] != '(' && t[i]) i++, len++;
	if(len > strlen(t) || t[strlen(t) - 1] != ';'){
		puts("错误的输入");
		return; 
	}
	for(int i = 1; i < len; ++i){
		if((t[i] >= 'A' &&  t[i] <='Z') ||  (t[i] >= 'a' && t[i] <= 'z') || t[i] =='_' || isdigit(t[i])) continue;
		puts("错误的函数名");
		return; 
	}
	String s(t); 
	func[++cnt].name = s.substring(1, len);
	for(i = 1; i < cnt; ++i){
		if(!func[cnt].name.index(func[i].name, 1)) continue;
		puts("函数重名错误"),cnt--;
		return; 
	}
	s.del(1, len+1);
	if(!((s.ch[0] >= 'A' && s.ch[0] <='Z') || (s.ch[0] >= 'a' && s.ch[0] <= 'z') || s.ch[0] =='_' )){
		puts("错误的变量名"),cnt--; 
		return; 
	}
	len = 1, i = 0;
	while(s.ch[i + 1] != ')' && s.ch[i]) i++, len++;
	for(int i = 1; i < len; ++i){
		if((s.ch[i] >= 'A' &&  s.ch[i] <='Z') ||  (s.ch[i] >= 'a' && s.ch[i] <= 'z') || s.ch[i] =='_' || isdigit(s.ch[i])) continue;
		puts("错误的变量名"),cnt--;
		return; 
	}
	func[cnt].var = s.substring(1, len);
	s.del(1, len);
	if(s.ch[0] != ')' || s.ch[1] != '='){
		puts("错误的输入"),cnt--;
		return;
	}
	s.del(1,2); s.del(s.length,1);
	s.insert(1,'('); s.insert(s.length + 1, ')'); s.insert(s.length + 1,'#');
	for(int i = 1; i < cnt; ++i){
		if(!s.index(func[i].name)) continue;
		String s1,s2; 
		s1.assign(func[i].name);
		s1.insert(s1.length + 1, '(');
		s1.insert(s1.length + 1, func[cnt].var);
		s1.insert(s1.length + 1, ')');
		s2.assign(func[i].exp);
		s2.del(s2.length, 1);
		s2.replace(func[i].var, func[cnt].var);
	 	s.replace(s1, s2);
	}
	func[cnt].exp = s;
	String _exp, tmp; 
	_exp.assign(func[cnt].exp);
	tmp.insert(1,'1'); 
	_exp.replace(func[cnt].var,tmp);
	ElemType e;
	Status sta = EXP::Cal(_exp, e); 
	if(!sta) puts("错误的表达式"),cnt--;
}

void Run(char *t){
	String Var(t);
	int len = 1, i = 0;
	while(t[i + 1] != '(' && t[i]) i++, len++;
	if(len > strlen(t)){
		puts("错误的输入");
		return; 
	}
	String Name = Var.substring(1, len);
	int ip = 0;
	for(i = 1; i <= cnt; ++i) if(!Name.compare(func[i].name)) {ip= i; break;}
	if(!ip) {
		puts("没有找到该函数");
		return; 
	}
	Var.del(1, len+1);
	if(Var.ch[Var.length - 1] == ';') Var.del(Var.length,1);
	if(Var.ch[Var.length - 1] == ')') Var.del(Var.length,1);
	for(int i = 0; i < Var.length; ++i) 
		if(!(isdigit(Var.ch[i]) || Var.ch[i]=='.')){
			puts("错误的输入");
			return; 
		}
	String exp;
	exp.assign(func[ip].exp);
	exp.replace(func[ip].var, Var);
	ElemType e;
	EXP::Cal(exp,e); 
	cout << e << endl;	
}

void Func(){
	while(1){
		char opt[MaxSize],t[MaxSize];
		puts("输入操作 DEF：定义函数  RUN：运行函数  OUT：退出函数模式");
		while(1){
			cin>>opt;
			if((opt[0]=='O'&&opt[1]=='U'&&opt[2]=='T'&&!opt[3])){
				puts("退出函数模式");
				puts("------------------------------------------------------------");
				puts(""); 
				return;
			}
			cin>>t;
			if(((opt[0]=='D'&&opt[1]=='E'&&opt[2]=='F'&&!opt[3]) || (opt[0]=='R'&&opt[1]=='U'&&opt[2]=='N'&&!opt[3])))  break;
			puts("输入正确的操作命令");
		}
		if(opt[0] == 'D') Def(t);
		else Run(t);
	}
}
}

class Matrix{//封装矩阵运算 
public: 
	int n, m;
	
	Matrix(int _n,int _m): n(_n), m(_m){
		memset(this->a,0,sizeof(this->a));	
	}
	
	void clear(){//清空矩阵 
		this->n = 0, this->m = 0;
		memset(this->a,0,sizeof(this->a));
	} 
	
	ElemType getval(int x,int y){//取第x行，第y列的值 
		if(x < 1 || x > n || y < 1 || y > m) return 0;
		return this->a[x][y];
	}
	
	void modify(int x, int y, ElemType e){//将第x行第y列的值增加e 
		if(x < 1 || x > n || y < 1 || y > m) return;
		this->a[x][y] += e;
	}
	
	void change(Matrix B){//用矩阵B覆盖该矩阵 
		this->n = B.n, this->m = B.m;
		for(int i = 1; i <= this->n; ++i)
			for(int j = 1;j <= this->m; ++j)
				this->a[i][j] = B.getval(i, j);
	}
	
	void print(){//矩阵输出 
		for(int i = 1 ;i <= this->n; ++i){
			for(int j = 1; j <= this->m; ++j) cout << setw(6) << a[i][j] << ' ';
			puts("");
		}
		puts("");
	}
	
	void transpose(){//矩阵转置 
		Matrix B(this->m ,this->n);
		for(int i = 1; i <= this->n; ++i)
			for(int j = 1;j <= this->m; ++j)
				B.modify(j, i, this->getval(i, j));
		this->change(B);
	}
	
	void nummul(ElemType k){//矩阵数乘 
		for(int i = 1 ; i <= this->n; ++i)
			for(int j = 1; j<= this->m; ++j)
				this->a[i][j] *= k;
	} 
	
	Status add(Matrix B, int on = 1){//矩阵加减法 
		if(this->m != B.m || this->n != B.n) return ERROR;
		for(int i = 1 ; i <= this->n; ++i)
			for(int j = 1; j<= this->m; ++j)
				this->modify(i, j, on * B.getval(i, j));
		return OK;	
	}
	 
	Status mul(Matrix B){//矩阵乘法 
		if(this->m != B.n) return ERROR;
		int x = this->n, y = this->m, z = B.m;
		Matrix C(x,z);
		for(int i = 1;i <= x; i++)
      		for(int j = 1;j <= z; j++)
        		for(int k = 1; k <= y; k++)
        			C.modify(i, j, this->getval(i, k) * B.getval(k, j));
        this->change(C);
		return OK;
	} 
	
	Status qpow(int k){//矩阵快速幂 
		if(this->m != this->n) return ERROR;
		int r = this->n;
		Matrix res(r,r), x(r,r);
		for(int i = 1; i <= r; ++i) res.modify(i, i, 1);
		x.change(*this); 
		while(k){
			if(k&1) res.mul(x);
			x.mul(x); 
			k >>= 1;
		}
		this->change(res);
		return OK;
	}             
	
	Status determinant(double &res){//行列式求值,辗转相除+高斯消元,复杂度O(n^3+n^2logn) 
		if(this->m != this->n) return ERROR;
		res = 1.0;
		int r = this->n, flag = 1;
		ElemType b[MARSIZE][MARSIZE];
		memcpy(b,this->a,sizeof(b));
		for(int i = 1; i <= r; ++i)
			for(int j = i + 1; j <= r; ++j){
				while(fabs(b[i][i]) > 1e-8){
					double div= b[j][i] / b[i][i];
					for(int k = i; k <= r; ++k) b[j][k] -= div * b[i][k];
					swap(b[i], b[j]), flag *= -1;
				}
				swap(b[i], b[j]), flag *= -1;
			}
		for(int i = 1; i <= n ; ++i) res *= b[i][i];
		res *= flag;
		return OK;
	}
	
private:
	ElemType a[MARSIZE][MARSIZE];
};

namespace MATRIX{
void Mar()
{
	while(1)
	{ 
		int opt,n,m;
		puts("输入一个矩阵的行数,列数:");
		while(1){
			cin>>n>>m;
			if(n > 0 && m > 0) break;
			puts("请输入正确的行数,列数"); 
		}
		Matrix a(n,m);
		puts("请输入矩阵"); 
		for(int i = 1; i <= n; ++i)
			for(int j = 1; j <= m; ++j){
				ElemType x; cin>>x;
				a.modify(i,j,x);
			}
		while(1)
		{
			puts("输入操作 1：矩阵转置  2：矩阵数乘  3：矩阵加法 4：矩阵减法 5：矩阵乘法 6：矩阵快速幂  7：行列式求值  0：退出矩阵模式");
			while(1){
				cin>>opt;
				if(opt>=0&&opt<=7) break;
				puts("请输入正确的操作数"); 
			}
			if(!opt){
				puts("退出矩阵模式");
				puts("------------------------------------------------------------");
				puts(""); 
				return;
			}
			else if(opt == 1){
				a.transpose();
				puts("矩阵转置的结果是:");
				a.print();	
			}
			else if(opt == 2){
				ElemType k;
				puts("输入矩阵要乘的数");
				cin>>k;
				a.nummul(k);
				puts("矩阵数乘的结果是:");
				a.print(); 
			}
			else if(opt == 3){
				puts("输入一个矩阵的行数,列数:");
				while(1){
					cin>>n>>m;
					if(n > 0 && m > 0) break;
					puts("请输入正确的行数,列数"); 
				}
				Matrix b(n,m);
				puts("请输入矩阵"); 
				for(int i = 1; i <= n; ++i)
					for(int j = 1; j <= m; ++j){
					ElemType x; cin>>x;
					b.modify(i,j,x);
				}
				Status st = a.add(b,1);
				if(st)  puts("矩阵加法的结果是:"), a.print();
				else puts("错误的运算！"); 
			}
			else if(opt == 4){
				puts("输入一个矩阵的行数,列数:");
				while(1){
					cin>>n>>m;
					if(n > 0 && m > 0) break;
					puts("请输入正确的行数,列数"); 
				}
				Matrix b(n,m);
				puts("请输入矩阵"); 
				for(int i = 1; i <= n; ++i)
					for(int j = 1; j <= m; ++j){
					ElemType x; cin>>x;
					b.modify(i,j,x);
				}
				Status st = a.add(b,-1);
				if(st)  puts("矩阵减法的结果是:"), a.print();
				else puts("错误的运算！");
			}
			else if(opt == 5){
				puts("输入一个矩阵的行数,列数:");
				while(1){
					cin>>n>>m;
					if(n > 0 && m > 0) break;
					puts("请输入正确的行数,列数"); 
				}
				Matrix b(n,m);
				puts("请输入矩阵"); 
				for(int i = 1; i <= n; ++i)
					for(int j = 1; j <= m; ++j){
					ElemType x; cin>>x;
					b.modify(i,j,x);
				}
				Status st = a.mul(b);
				if(st)  puts("矩阵乘法的结果是:"), a.print();
				else puts("错误的运算！");
			} 
			else if(opt == 6){
				int k;
				puts("输入乘幂");
				while(1){
					cin>>k;
					if(k >= 0) break;
					puts("请输入正确的乘幂"); 
				}
				Status st = a.qpow(k);
				if(st)  puts("矩阵乘幂的结果是:"), a.print();
				else puts("错误的运算！");
			} 
			else if(opt == 7){
				double ans;
				Status st = a.determinant(ans);
				if(st)  puts("行列式求值的结果是:"), cout<<ans<<endl<<endl;
				else puts("错误的运算！");
			} 
		}
	}
}
}

int main()
{
	int opt;
	while(1){ 
		
		puts("输入模式 1：向量  2：多项式 3：表达式求值  4：函数运算  5：矩阵运算  0：关闭"); 
		while(1){
			cin>>opt;
			if(opt>=0&&opt<=5) break;
			puts("请输入正确的操作数"); 
		}
		if(opt == 1) {
			puts("------------------------------------------------------------");
			puts("进入向量模式");
			Vec::vec();
		}
		else if(opt == 2){
			puts("------------------------------------------------------------");
			puts("进入多项式模式");
			Poly::poly(); 
		}
		else if(opt == 3){
			puts("------------------------------------------------------------");
			puts("进入表达式求值模式");
			EXP::exp();
		}
		else if(opt == 4){
			puts("------------------------------------------------------------");
			puts("进入函数模式");
			FUNCTION::Func();
		}
		else if(opt==5){
			puts("------------------------------------------------------------");
			puts("进入矩阵模式");
			MATRIX::Mar();
		}
		else break;
	} 
	return 0;
}
