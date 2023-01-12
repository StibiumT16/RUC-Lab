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

namespace SQLIST{//��SQLIST�����ռ��װ˳����� 
typedef struct{
    ElemType *elem;
    int length,listsize;
    
	void clear(){length = 0;}//������Ա� 
	 
    Status init(){//��ʼ�����Ա� 
    	elem=(ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
    	if(!elem) exit(OVERFLOW);                                                 
    	length=0;
    	listsize =  LIST_INIT_SIZE;
    	return OK;
	}
	
	void destory(){//�������Ա� 
		delete(elem);
		elem=NULL;
		length=listsize=0;
	}
	
	ElemType get(int pos){//ȡ���Ա���ĳһλ�õ�Ԫ�� 
		if(pos <= 0 || pos > length) return 0;
		return elem[pos-1];
	}
	
	Status insert(int i, ElemType e){//�����Ա�ĵ�iλ�ò���Ԫ��e 
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
	
	Status del(int i, ElemType &e){//ɾ�����Ա���iλ�õ�Ԫ��e 
		if(i < 1 || i > length) return ERROR;
		ElemType *p = &(elem[i-1]);
		e = *p;
		ElemType *q = elem + length - 1;
		for(++p; p <= q; ++p) *(p - 1) = *p;
		--length;
		return OK;
	}	
}SqList;

Status TransList_Sq(SqList &A, SqList &B){//�����Ա�Bת�Ƶ����Ա�A�ϲ����B 
	A.clear();
	for(int i = 1; i<= B.length; ++i)
		A.insert(i, B.get(i));
	B.clear();
} 
}
using namespace SQLIST;

namespace OPT{//��OPT�����ռ��װ����������� 
void Add(SqList A, SqList B, SqList &C, int on){//�ӷ��ͼ�����������on���� 
	int len = max(A.length, B.length);
	for(int i = 1; i <= len; ++i) C.insert(i ,A.get(i) + on * B.get(i));
} 

void Mul(SqList A, SqList B, SqList &C){//�˷����� 
	if((A.length == 1 && A.get(1) == 0) || (B.length == 1 && B.get(1) == 0)){
		C.insert(1, 0);
		return;
	}
	for(int i = 1; i <= A.length + B.length - 1 ;++i) C.insert(i,0);
	for(int i = 1; i <= A.length; ++i)
		for(int j = 1; j <= B.length; ++j)
			C.elem[i+j-2] += A.get(i) * B.get(j);
}

void dev(SqList A, SqList &B){//�󵼲��� 
	int len = A.length;
	if(len == 1) {
		B.insert(1, 0);
		return;
	}
	for(int i = 1; i < len ; ++i)
	B.insert(i, A.get(i + 1) * i);
}

void idev(SqList A, SqList &B){//�󲻶����ֲ��� 
	int len = A.length;
	B.insert(1, 0);
	if(len == 1 && A.get(1) == 0) return;
	for(int i = 1; i <= len; ++i){
		B.insert(i+1, A.get(i)/i);
	}
}

double Cos(SqList A,SqList B){//��н����Ҳ��� 
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
	if(opt == 1) puts("�˳�����ģʽ");
	else puts("�˳�����ʽģʽ");
	puts("------------------------------------------------------------");
	puts(""); 
}
}
using namespace OPT;

namespace Vec{//��Vec�����ռ��װ�������� 
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
	printf("����%dά����Vec:\n",n);
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
	puts("��������ά��:");
	while(1){
		cin>>n;
		if(n > 0) break;
		puts("��������ȷ��ά��"); 
	}
	input_vec(a,n);
	while(1){
		puts("������� 1���ӷ�  2������  3����н�����  0���˳�����ģʽ");
		while(1){
			cin>>opt;
			if(opt>=0&&opt<=3) break;
			puts("��������ȷ�Ĳ�����"); 
		}
		if(!opt){
			Out(1,a,b,c);
			return; 
		} 
		if(opt == 1){
			input_vec(b,n);
			Add(a,b,c,1);
			puts("�ӷ��Ľ��Ϊ��");
			print_vec(c);
			puts("");
			TransList_Sq(a,c);
			b.clear(); 
		}
		else if(opt == 2){
			input_vec(b,n);
			Add(a,b,c,-1);
			puts("�����Ľ��Ϊ��");
			print_vec(c);
			puts("");
			TransList_Sq(a,c);
			b.clear();
		}
		else if(opt == 3) {
			input_vec(b,n);
			puts("�����ҼнǵĽ��Ϊ��");
			printf("%.6lf\n",Cos(a,b));
			Out(1,a,b,c);
			return;
		}
	}
}
}

namespace Poly{//��Poly�����ռ��װ����ʽ����
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
	puts("�������ʽ�Ľ�deg");
	while(1){
		cin>>n;
		if(n>=0) break;
		puts("��������ȷ�Ľ���"); 
	}
	printf("�������ʽ�ӵ͵��߸���ϵ��(%d��)\n",n+1);
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
		puts("������� 1���ӷ�  2������  3���˷�  4����  5:�󲻶�����  0���˳�����ʽģʽ");
		while(1){
			cin>>opt;
			if(opt>=0&&opt<=5) break;
			puts("��������ȷ�Ĳ�����"); 
		}
		if(!opt){
			Out(2,a,b,c);
			return;
		}
		if(opt == 1) input_poly(b),Add(a,b,c,1),puts("�ӷ��Ľ��Ϊ��");
		else if(opt == 2) input_poly(b),Add(a,b,c,-1),puts("�����Ľ��Ϊ��");
		else if(opt == 3) input_poly(b),Mul(a,b,c),puts("�˷��Ľ��Ϊ��"); 
		else if(opt == 4) {
			int num;
			puts("�������󵼵Ľ���");
			while(1) {
				cin >> num; 
				if(num > 0) break;
				puts("��������ȷ�Ľ���"); 
			}
			for(int i = 1; i <= num; ++i){
				dev(a,c);
				if(i != num) TransList_Sq(a,c);
			} 
			printf("��%d�׵��Ľ��Ϊ��\n",num); 
		}
		else {
			int num;
			puts("�������󲻶����ֵĽ���");
			while(1) {
				cin >> num; 
				if(num > 0) break;
				puts("��������ȷ�Ľ���"); 
			}
			for(int i = 1; i <= num; ++i){
				idev(a,c);
				if(i != num) TransList_Sq(a,c);
			} 
			printf("��%d�ײ������ֵĽ��Ϊ��\n",num); 
		}
		print_poly(c);
		puts(""); 
		TransList_Sq(a,c);
		b.clear();
	}
}
}

template<typename T>
class Stack{//��װ��STL����ջ 
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
	
	bool empty(){ //�ж�ջ�Ƿ�Ϊ�� 
		return this->Top == this->Base;
	}
	
	T top(){//ȡջ��Ԫ�� 
		if(this->Top != this->Base) 
		return *(this->Top - 1);
	}
	
	void push(T e){//ѹջ 
		if(this->Top - this->Base >= this->stacksize){
			this->Base = (T *) realloc (this->Base, (this->stacksize + STACKINCREMENT) * sizeof (T)); 
			if(!this->Base) exit(OVERFLOW);
			this->Top = this->Base + this->stacksize;
			this->stacksize +=  STACKINCREMENT;
		}
		*this->Top++ = e;
	}
	
	void pop(){//��ջ 
		if(this->Top != this->Base)  --this->Top;
	}
		
private:
	int stacksize;
	T *Base;
	T *Top; 
};

class String{//��װ��STL�����ַ��� 
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
	
	Status assign(char *t){//����һ��ֵ���ڴ�����t�Ĵ� 
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
	
	Status assign(String t){//����һ��ֵ���ڴ�����t�Ĵ� 
		if(this->ch) free(this->ch);
		int i = t.length;
		if(!i) this->ch = NULL, this->length = 0;
		else{
			if(!(this->ch = (char*) malloc (i * sizeof(char)))) exit(OVERFLOW);
			for(int j = 0; j < i; ++j) this->ch[j] = t.ch[j];
			this->length = i;
		}
	}
	
	void clear(){//��մ� 
		if(this->ch) free(this->ch), this->ch = NULL;
		this->length = 0;
	}
	
	void insert(int pos, char chr){//����insert����S�ĵ�pos���ַ�ǰ�����ַ�chr
		if(pos < 1 || pos > this->length + 1) return;
		if(!(this->ch = (char*) realloc (this->ch, (this->length + 1) * sizeof(char)))) exit(OVERFLOW);
		for(int i = this->length-1; i >= pos - 1; --i) this->ch[i + 1] = this->ch[i];
		this->ch[pos-1] = chr; 
		this->length++;
	}
	
	void insert(int pos, String T){//����insert����S�ĵ�pos���ַ�ǰ���봮T 
		if(pos < 1 || pos > this->length + 1) return;
		if(T.length){
			if(!(this->ch = (char*) realloc (this->ch, (this->length + T.length) * sizeof(char)))) exit(OVERFLOW);
			for(int i = this->length-1; i >= pos - 1; --i) this->ch[i + T.length] = this->ch[i];
			for(int i = pos -1 ,j = 0; j <= T.length - 1; ++i, ++j) this->ch[i] = T.ch[j]; 
			this->length += T.length;
		}
	}
	
	void del(int pos, int len){//ɾ����posλ�����len���ַ� 
		if(pos < 1 || pos + len > this->length + 1) return;
		for(int i = pos - 1, j = pos + len -1; j < this->length; ++i, ++j) this->ch[i] = this->ch[j];
		this->length -= len;
	}
	
	int compare(String T){//�Ƚ����ַ�����ֵ 
		for(int i = 0; i < this->length && i < T.length; ++i)
			if(this->ch[i] != T.ch[i]) return this->ch[i] - T.ch[i];
		return this->length - T.length;
	}
	
	String substring(int pos ,int len){//��pos���ַ��𳤶�Ϊlen���Ӵ� 
		String Sub; 
		if(pos < 1 || pos > this->length || len < 0 || len + pos > this->length + 1) return Sub;
		if(!len) Sub.ch = NULL, Sub.length = 0;
		else for(int i = 0; i < len; ++i) Sub.insert(i + 1, this->ch[pos - 1 + i]);
		return Sub;
	}
	
	int index(String t, int pos = 1){//�ַ���ƥ�� 
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
	
	void replace(String s1, String s2){//��ԭ���е�����s1��s2��� 
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

namespace EXP{//��EXP�����ռ��װ���ʽ��ֵ����  
char precede(char a, char b){//�Ƚ���������ȼ� 
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

double Operate(ElemType a, char opt, ElemType b){//��ֵ 
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
 
Status check_bracket(String s){//�������ƥ�� 
	int cnt = 0;
	for(int i = 0; i < s.length; ++i){
		if(s.ch[i] == '(') cnt++;
		if(s.ch[i] == ')')  if(!cnt--) return ERROR;
	}
	return !cnt;
}

Status Cal(String S,ElemType &res){//֧�ָ��������͵�����������ʽ��ֵ 
	if(!check_bracket(S)) return ERROR;//�����������ƥ���Ƿ���� 
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
			i++, flag = 1;//�жϸ��� ,������ʽ��ͷ��������Ӧ�������������� 
		}
		else if(S.ch[i]=='-' && i==0 && S.ch[1] == '(') OPND.push(0), OPTR.push('-'), ++i;//���� -(...)����� 
		else if(S.ch[i]=='.') isfloat = 1, ++i, flag =1;//�ж�С��
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

String getvar(String s){//�ӱ��ʽ����ȡ������ 
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
		puts("������� 1������������ʽ��ֵ  2�����������ı��ʽ��ֵ  0���˳����ʽ��ֵģʽ");
		while(1){
			cin>>opt;
			if(opt>=0&&opt<=2) break;
			puts("��������ȷ�Ĳ�����"); 
		}
		if(!opt){
			puts("�˳����ʽ��ֵģʽ");
			puts("------------------------------------------------------------");
			puts(""); 
			return;
		}
		puts("������ʽ");
		String s;  ElemType e;
		char t[STRINGSIZE], tt[STRINGSIZE];
		cin>>t;
		s.assign(t);
		s.insert(s.length + 1, '#');	
		if(opt==1) {//opt=1��ֱ����ֵ 
			Status sta = Cal(s,e); 
			if(sta) cout << t << " = " << e << endl;
			else cout << "���ʽ����" << endl; 
		}
		else {//opt=2��������������Ӧ�����滻����ֵ 
			String Var = getvar(s), Num; 
			puts("���뵥��������ֵ");
			while(1){
				cin>>tt;
				int len = strlen(tt),flag=0;
				for(int i = 0; i < len; ++i) {
					if(!(isdigit(tt[i])||tt[i]=='.')){
						flag=1; break;
					}
				}
				if(!flag) break;
				puts("��������ȷ�ı�����ֵ");
			}
			Num.assign(tt);
			s.replace(Var, Num);
			Status sta = Cal(s,e); 
			if(sta){
				Var.print();
				cout<< " = " << tt <<endl;
				cout << t << " = " << e <<endl;
			}
			else cout << "���ʽ����" << endl; 
			puts(""); 
		}
	}	
}
}

namespace FUNCTION{//��FUNCTION�����ռ��װ�������� 
struct{String name,var,exp;} func[MaxSize];
int cnt = 0;

void Def(char *t){
	if(!((t[0] >= 'A' && t[0] <='Z') || (t[0] >= 'a' && t[0] <= 'z') || t[0] =='_' )){
		puts("����ĺ�����");
		return; 
	}
	int len = 1, i = 0;
	while(t[i + 1] != '(' && t[i]) i++, len++;
	if(len > strlen(t) || t[strlen(t) - 1] != ';'){
		puts("���������");
		return; 
	}
	for(int i = 1; i < len; ++i){
		if((t[i] >= 'A' &&  t[i] <='Z') ||  (t[i] >= 'a' && t[i] <= 'z') || t[i] =='_' || isdigit(t[i])) continue;
		puts("����ĺ�����");
		return; 
	}
	String s(t); 
	func[++cnt].name = s.substring(1, len);
	for(i = 1; i < cnt; ++i){
		if(!func[cnt].name.index(func[i].name, 1)) continue;
		puts("������������"),cnt--;
		return; 
	}
	s.del(1, len+1);
	if(!((s.ch[0] >= 'A' && s.ch[0] <='Z') || (s.ch[0] >= 'a' && s.ch[0] <= 'z') || s.ch[0] =='_' )){
		puts("����ı�����"),cnt--; 
		return; 
	}
	len = 1, i = 0;
	while(s.ch[i + 1] != ')' && s.ch[i]) i++, len++;
	for(int i = 1; i < len; ++i){
		if((s.ch[i] >= 'A' &&  s.ch[i] <='Z') ||  (s.ch[i] >= 'a' && s.ch[i] <= 'z') || s.ch[i] =='_' || isdigit(s.ch[i])) continue;
		puts("����ı�����"),cnt--;
		return; 
	}
	func[cnt].var = s.substring(1, len);
	s.del(1, len);
	if(s.ch[0] != ')' || s.ch[1] != '='){
		puts("���������"),cnt--;
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
	if(!sta) puts("����ı��ʽ"),cnt--;
}

void Run(char *t){
	String Var(t);
	int len = 1, i = 0;
	while(t[i + 1] != '(' && t[i]) i++, len++;
	if(len > strlen(t)){
		puts("���������");
		return; 
	}
	String Name = Var.substring(1, len);
	int ip = 0;
	for(i = 1; i <= cnt; ++i) if(!Name.compare(func[i].name)) {ip= i; break;}
	if(!ip) {
		puts("û���ҵ��ú���");
		return; 
	}
	Var.del(1, len+1);
	if(Var.ch[Var.length - 1] == ';') Var.del(Var.length,1);
	if(Var.ch[Var.length - 1] == ')') Var.del(Var.length,1);
	for(int i = 0; i < Var.length; ++i) 
		if(!(isdigit(Var.ch[i]) || Var.ch[i]=='.')){
			puts("���������");
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
		puts("������� DEF�����庯��  RUN�����к���  OUT���˳�����ģʽ");
		while(1){
			cin>>opt;
			if((opt[0]=='O'&&opt[1]=='U'&&opt[2]=='T'&&!opt[3])){
				puts("�˳�����ģʽ");
				puts("------------------------------------------------------------");
				puts(""); 
				return;
			}
			cin>>t;
			if(((opt[0]=='D'&&opt[1]=='E'&&opt[2]=='F'&&!opt[3]) || (opt[0]=='R'&&opt[1]=='U'&&opt[2]=='N'&&!opt[3])))  break;
			puts("������ȷ�Ĳ�������");
		}
		if(opt[0] == 'D') Def(t);
		else Run(t);
	}
}
}

class Matrix{//��װ�������� 
public: 
	int n, m;
	
	Matrix(int _n,int _m): n(_n), m(_m){
		memset(this->a,0,sizeof(this->a));	
	}
	
	void clear(){//��վ��� 
		this->n = 0, this->m = 0;
		memset(this->a,0,sizeof(this->a));
	} 
	
	ElemType getval(int x,int y){//ȡ��x�У���y�е�ֵ 
		if(x < 1 || x > n || y < 1 || y > m) return 0;
		return this->a[x][y];
	}
	
	void modify(int x, int y, ElemType e){//����x�е�y�е�ֵ����e 
		if(x < 1 || x > n || y < 1 || y > m) return;
		this->a[x][y] += e;
	}
	
	void change(Matrix B){//�þ���B���Ǹþ��� 
		this->n = B.n, this->m = B.m;
		for(int i = 1; i <= this->n; ++i)
			for(int j = 1;j <= this->m; ++j)
				this->a[i][j] = B.getval(i, j);
	}
	
	void print(){//������� 
		for(int i = 1 ;i <= this->n; ++i){
			for(int j = 1; j <= this->m; ++j) cout << setw(6) << a[i][j] << ' ';
			puts("");
		}
		puts("");
	}
	
	void transpose(){//����ת�� 
		Matrix B(this->m ,this->n);
		for(int i = 1; i <= this->n; ++i)
			for(int j = 1;j <= this->m; ++j)
				B.modify(j, i, this->getval(i, j));
		this->change(B);
	}
	
	void nummul(ElemType k){//�������� 
		for(int i = 1 ; i <= this->n; ++i)
			for(int j = 1; j<= this->m; ++j)
				this->a[i][j] *= k;
	} 
	
	Status add(Matrix B, int on = 1){//����Ӽ��� 
		if(this->m != B.m || this->n != B.n) return ERROR;
		for(int i = 1 ; i <= this->n; ++i)
			for(int j = 1; j<= this->m; ++j)
				this->modify(i, j, on * B.getval(i, j));
		return OK;	
	}
	 
	Status mul(Matrix B){//����˷� 
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
	
	Status qpow(int k){//��������� 
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
	
	Status determinant(double &res){//����ʽ��ֵ,շת���+��˹��Ԫ,���Ӷ�O(n^3+n^2logn) 
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
		puts("����һ�����������,����:");
		while(1){
			cin>>n>>m;
			if(n > 0 && m > 0) break;
			puts("��������ȷ������,����"); 
		}
		Matrix a(n,m);
		puts("���������"); 
		for(int i = 1; i <= n; ++i)
			for(int j = 1; j <= m; ++j){
				ElemType x; cin>>x;
				a.modify(i,j,x);
			}
		while(1)
		{
			puts("������� 1������ת��  2����������  3������ӷ� 4��������� 5������˷� 6�����������  7������ʽ��ֵ  0���˳�����ģʽ");
			while(1){
				cin>>opt;
				if(opt>=0&&opt<=7) break;
				puts("��������ȷ�Ĳ�����"); 
			}
			if(!opt){
				puts("�˳�����ģʽ");
				puts("------------------------------------------------------------");
				puts(""); 
				return;
			}
			else if(opt == 1){
				a.transpose();
				puts("����ת�õĽ����:");
				a.print();	
			}
			else if(opt == 2){
				ElemType k;
				puts("�������Ҫ�˵���");
				cin>>k;
				a.nummul(k);
				puts("�������˵Ľ����:");
				a.print(); 
			}
			else if(opt == 3){
				puts("����һ�����������,����:");
				while(1){
					cin>>n>>m;
					if(n > 0 && m > 0) break;
					puts("��������ȷ������,����"); 
				}
				Matrix b(n,m);
				puts("���������"); 
				for(int i = 1; i <= n; ++i)
					for(int j = 1; j <= m; ++j){
					ElemType x; cin>>x;
					b.modify(i,j,x);
				}
				Status st = a.add(b,1);
				if(st)  puts("����ӷ��Ľ����:"), a.print();
				else puts("��������㣡"); 
			}
			else if(opt == 4){
				puts("����һ�����������,����:");
				while(1){
					cin>>n>>m;
					if(n > 0 && m > 0) break;
					puts("��������ȷ������,����"); 
				}
				Matrix b(n,m);
				puts("���������"); 
				for(int i = 1; i <= n; ++i)
					for(int j = 1; j <= m; ++j){
					ElemType x; cin>>x;
					b.modify(i,j,x);
				}
				Status st = a.add(b,-1);
				if(st)  puts("��������Ľ����:"), a.print();
				else puts("��������㣡");
			}
			else if(opt == 5){
				puts("����һ�����������,����:");
				while(1){
					cin>>n>>m;
					if(n > 0 && m > 0) break;
					puts("��������ȷ������,����"); 
				}
				Matrix b(n,m);
				puts("���������"); 
				for(int i = 1; i <= n; ++i)
					for(int j = 1; j <= m; ++j){
					ElemType x; cin>>x;
					b.modify(i,j,x);
				}
				Status st = a.mul(b);
				if(st)  puts("����˷��Ľ����:"), a.print();
				else puts("��������㣡");
			} 
			else if(opt == 6){
				int k;
				puts("�������");
				while(1){
					cin>>k;
					if(k >= 0) break;
					puts("��������ȷ�ĳ���"); 
				}
				Status st = a.qpow(k);
				if(st)  puts("������ݵĽ����:"), a.print();
				else puts("��������㣡");
			} 
			else if(opt == 7){
				double ans;
				Status st = a.determinant(ans);
				if(st)  puts("����ʽ��ֵ�Ľ����:"), cout<<ans<<endl<<endl;
				else puts("��������㣡");
			} 
		}
	}
}
}

int main()
{
	int opt;
	while(1){ 
		
		puts("����ģʽ 1������  2������ʽ 3�����ʽ��ֵ  4����������  5����������  0���ر�"); 
		while(1){
			cin>>opt;
			if(opt>=0&&opt<=5) break;
			puts("��������ȷ�Ĳ�����"); 
		}
		if(opt == 1) {
			puts("------------------------------------------------------------");
			puts("��������ģʽ");
			Vec::vec();
		}
		else if(opt == 2){
			puts("------------------------------------------------------------");
			puts("�������ʽģʽ");
			Poly::poly(); 
		}
		else if(opt == 3){
			puts("------------------------------------------------------------");
			puts("������ʽ��ֵģʽ");
			EXP::exp();
		}
		else if(opt == 4){
			puts("------------------------------------------------------------");
			puts("���뺯��ģʽ");
			FUNCTION::Func();
		}
		else if(opt==5){
			puts("------------------------------------------------------------");
			puts("�������ģʽ");
			MATRIX::Mar();
		}
		else break;
	} 
	return 0;
}
