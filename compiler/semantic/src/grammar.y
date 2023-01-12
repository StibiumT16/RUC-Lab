%{
#include <bits/stdc++.h>
using namespace std;

extern int yylex(void); 
extern int yyparse(void); 
extern int row, col, yyleng;
extern char *yytext;

FILE *f;

void yyerror(const char *s){
	printf("%s:\t(%d, %d)\t %s\n", s, row, col - yyleng, yytext);
}

enum Type {Int, Constint, Arr, Fint, Fvoid};
struct Var{
	Type type;
	int val, offset;
	vector<int> dim;
	Var() {}
	Var(Type _type, int _val) : type(_type), val(_val){}
	Var(Type _type, int _val, int _offset) : type(_type), val(_val), offset(_offset){}
	Var(Type _type, int _val, int _offset, vector<int> _dim) : type(_type), val(_val), offset(_offset), dim(_dim){}
};

int tot = 0, num = 0, offset = 0, level = 0;
bool is_func_void = false;
char tag[114514][100], funcname[114];

vector<int> p_offset, ArrInitVal;
vector<string> Assemble, Para;
vector<map<string, Var> > symbol;
vector<vector<pair <int, int> > > breaklist, continuelist; 


struct State{
	int val, offset, offset_of_arr, quad;
	bool is_const, is_arr;
	string name;
	vector<bool> para_is_const, para_is_arr;
	vector<int> truelist, falselist, dim, para_val, para_offset, para_offset_of_arr;
	vector<string> para_name;
}sta[114514];


vector<int> merge(const vector<int> &a, const vector<int> &b){
	vector<int> res;
	for(auto x : a) res.push_back(x);
	for(auto x : b) res.push_back(x);
	return res;
}

void backpatch(vector<int> &a, int pos){
	for(auto x : a) Assemble[x] += ".L" + to_string(pos) + "\n";
}

void var2reg(int x, const char* reg){
	char tmp[114];
	if(sta[x].is_const){
		sprintf(tmp, "\tmovl\t$%d, %%%s\n", sta[x].val, reg); Assemble.push_back(tmp);
	}
	else if(sta[x].offset == 1){
		if(sta[x].is_arr){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].offset_of_arr); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			Assemble.push_back("\tleaq\t0(, %rbx, 4), %rdx\n");
			sprintf(tmp, "\tleaq\t%s(%rip), %rbx\n", sta[x].name.c_str()); Assemble.push_back(tmp);
			sprintf(tmp, "\tmovl\t(%rdx, %rbx), %%%s\n", reg); Assemble.push_back(tmp);
		}
		else{
			sprintf(tmp, "\tmovl\t%s(%rip), %%%s\n", sta[x].name.c_str(), reg); Assemble.push_back(tmp);
		}
	}
	else{
		if(sta[x].is_arr){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].offset_of_arr); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			sprintf(tmp, "\tmovl\t%d(%rbp, %rbx, 4), %%%s\n", sta[x].offset, reg); Assemble.push_back(tmp);
			
		}
		else{
			sprintf(tmp, "\tmovl\t%d(%rbp), %%%s\n", sta[x].offset, reg); Assemble.push_back(tmp);
		}
	}
}

void reg2var(const char* reg, int x){
	char tmp[114];
	if(sta[x].is_const){
		yyerror("Const Error");
		exit(0);
	}
	if(sta[x].offset == 1){
		if(sta[x].is_arr){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].offset_of_arr); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			Assemble.push_back("\tleaq\t0(, %rbx, 4), %rdx\n");
			sprintf(tmp, "\tleaq\t%s(%rip), %rbx\n", sta[x].name.c_str()); Assemble.push_back(tmp);
			sprintf(tmp, "\tmovl\t%%%s, (%rdx, %rbx)\n", reg); Assemble.push_back(tmp);
		}
		else{
			sprintf(tmp, "\tmovl\t%%%s, %s(%rip)\n",  reg, sta[x].name.c_str()); Assemble.push_back(tmp);
		}
	}
	else{
		if(sta[x].is_arr){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].offset_of_arr); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			sprintf(tmp, "\tmovl\t%%%s, %d(%rbp, %rbx, 4)\n", reg, sta[x].offset); Assemble.push_back(tmp);
		}
		else{
			sprintf(tmp, "\tmovl\t%%%s, %d(%rbp)\n", reg, sta[x].offset); Assemble.push_back(tmp);
		}
	}
}

void para2reg(int x, int pos, const char *reg){
	char tmp[114];
	if(sta[x].para_is_const[pos]){
		sprintf(tmp, "\tmovl\t$%d, %%%s\n", sta[x].para_val[pos], reg); Assemble.push_back(tmp);
	}
	else if(sta[x].para_offset[pos] == 1){
		if(sta[x].para_is_arr[pos]){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].para_offset_of_arr[pos]); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			Assemble.push_back("\tleaq\t0(, %rbx, 4), %rdx\n");
			sprintf(tmp, "\tleaq\t%s(%rip), %rbx\n", sta[x].para_name[pos].c_str()); Assemble.push_back(tmp);
			sprintf(tmp, "\tmovl\t(%rdx, %rbx), %%%s\n", reg); Assemble.push_back(tmp);
		}
		else{
			sprintf(tmp, "\tmovl\t%s(%rip), %%%s\n", sta[x].para_name[pos].c_str(), reg); Assemble.push_back(tmp);
		}
	}
	else{
		if(sta[x].para_is_arr[pos]){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].para_offset_of_arr[pos]); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			sprintf(tmp, "\tmovl\t%d(%rbp, %rbx, 4), %%%s\n", sta[x].para_offset[pos], reg); Assemble.push_back(tmp);
			
		}
		else{
			sprintf(tmp, "\tmovl\t%d(%rbp), %%%s\n", sta[x].para_offset[pos], reg); Assemble.push_back(tmp);
		}
	}
}

void lea_para2reg(int x, int pos, const char *reg){
	char tmp[114];
	if(sta[x].para_is_const[pos]){
		yyerror("Const Error");
		exit(0);
	}
	if(sta[x].para_offset[pos] == 1){
		if(sta[x].para_is_arr[pos]){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].para_offset_of_arr[pos]); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			Assemble.push_back("\tleaq\t0(, %rbx, 4), %rdx\n");
			sprintf(tmp, "\tleaq\t%s(%rip), %rbx\n", sta[x].para_name[pos].c_str()); Assemble.push_back(tmp);
			sprintf(tmp, "\tleaq\t(%rdx, %rbx), %%%s\n", reg); Assemble.push_back(tmp);
		}
		else{
			sprintf(tmp, "\tleaq\t%s(%rip), %%%s\n", sta[x].para_name[pos].c_str(), reg); Assemble.push_back(tmp);
		}
	}
	else{
		if(sta[x].para_is_arr[pos]){
			sprintf(tmp, "\tmovl\t%d(%rbp), %%ebx\n", sta[x].para_offset_of_arr[pos]); Assemble.push_back(tmp);
			Assemble.push_back("\tcltq\n");
			sprintf(tmp, "\tleaq\t%d(%rbp, %rbx, 4), %%%s\n", sta[x].para_offset[pos], reg); Assemble.push_back(tmp);
			
		}
		else{
			sprintf(tmp, "\tleaq\t%d(%rbp), %%%s\n", sta[x].para_offset[pos], reg); Assemble.push_back(tmp);
		}
	}
}

void call_func_push(){
	Assemble.push_back("\tpushq\t%rbp\n");
	Assemble.push_back("\tpushq\t%r8\n");
	Assemble.push_back("\tpushq\t%r9\n");
	Assemble.push_back("\tmovq\t%rsp, %rbp\n");
}

void ret_func_pop(){
	Assemble.push_back("\tpopq\t%r9\n");
	Assemble.push_back("\tpopq\t%r8\n");
	Assemble.push_back("\tpopq\t%rbp\n");
	Assemble.push_back("\tret\n");
}
%}

%token IF
%token ELSE WITHOUTELSE
%token WHILE
%token CONTINUE
%token BREAK
%token RETURN
%token CONST
%token INT
%token VOID
%token MAIN
%token LEQ
%token GEQ
%token EQ
%token UEQ
%token AND
%token OR
%token ID
%token NUMBER
%token ERROR 

%nonassoc WITHOUTELSE
%nonassoc ELSE

%%

CompUnits: CompUnit{}
	| CompUnits CompUnit{}
	;

CompUnit: Decl{}
	| FuncDef{}
	;

Decl:	ConstDecl{}
	| VarDecl{}
	;

ConstDecl: CONST INT ConstDefs ';'{}
	| CONST error ';'{printf("Const Definition Error\n");}
	;

ConstDefs: ConstDef{}
	| ConstDefs ',' ConstDef{}
	;

	
ConstArr: /*empty*/ {$$ = 0;}
	| '[' ConstExp ']' ConstArr{
		if($4) sta[$$ = $4].val = sta[$2].val * sta[$4].val;
		else sta[$$ = ++tot].val = sta[$2].val;
		sta[$$].dim.push_back(sta[$2].val);
	}
	;
	
ConstDef: ID ConstArr '=' ConstInitVal{
		if(symbol[level].find(tag[$1]) != symbol[level].end()){
			yyerror("Const Redefinition Error");
			exit(0);
		}
		char tmp[114];
		if(!level){
			if(!sta[$4].is_const){
				printf("Const Definition Error\n");
				exit(0);
			}
			if (!$2){
				Assemble.push_back("\t.section\t.rodata\n");
				Assemble.push_back("\t.align\t4\n");
				sprintf (tmp, "\t.type\t%s, @object\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, 4\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.long\t%d\n", sta[$4].val); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][tag[$1]] = Var(Constint, sta[$4].val, 1);
			}
			//else ?
		}
		else{
			if (!$2){
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t$%d, %%edi\n", sta[$4].val); Assemble.push_back(tmp);
				sprintf (tmp, "\tmovl\t%%edi, %d(%rbp)\n", offset); Assemble.push_back(tmp);
				symbol[level][tag[$1]] = Var(Constint, sta[$4].val, offset);
			}
			//else ?
		}
		
	}
	;

ConstInitVals: ConstInitVal{}
	| ConstInitVals ',' ConstInitVal{}
	;

ConstInitVal: ConstExp{$$ = $1;}
	| '{' '}'{}
	| '{' ConstInitVals '}'{}
	;

VarDefs: VarDef{}
	| VarDefs ',' VarDef{}
	;

VarDecl: INT VarDefs ';'{}
	| INT error ';' {printf("Variable Definition Error\n");}
	;	
	
VarDef: ID ConstArr{
		if(symbol[level].find(tag[$1]) != symbol[level].end()){
			yyerror("Variable Redefinition Error");
			exit(0);
		}
		char tmp[114];
		if(!level){
			if(!$2){
				sprintf (tmp, "\t.globl\t%s\n", tag[$1]); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t4\n");
				sprintf (tmp, "\t.type\t%s, @object\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, 4\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", tag[$1]); Assemble.push_back(tmp);
				Assemble.push_back("\t.long\t0\n");
				Assemble.push_back("\t.text\n");
				symbol[0][tag[$1]] = Var(Int, 0, 1);
			}
			else{
				sprintf (tmp, "\t.globl\t%s\n", tag[$1]); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t32\n");
				sprintf (tmp, "\t.type\t%s, @object\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, %d\n", tag[$1], sta[$2].val * 4); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.zero\t%d\n", sta[$2].val * 4); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][tag[$1]] = Var(Arr, 0, 1, sta[$2].dim);
			}
		}
		else{
			if(!$2){
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				symbol[level][tag[$1]] = Var(Int, 0, offset);
			}
			else{
				offset -= 4 * sta[$2].val;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", sta[$2].val * 4); Assemble.push_back(tmp);
				symbol[level][tag[$1]] = Var(Arr, 0, offset, sta[$2].dim);
			}
		}
	}
	| ID ConstArr '=' InitVal{
		if(symbol[level].find(tag[$1]) != symbol[level].end()){
			yyerror("Variable Redefinition Error");
			exit(0);
		}
		char tmp[114];
		if(!level){
			if (!$2){
				if(!$4 || !sta[$4].is_const){
					yyerror("Initializer Error");
					exit(0);
				}
				sprintf (tmp, "\t.globl\t%s\n", tag[$1]); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t4\n");
				sprintf (tmp, "\t.type\t%s, @object\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, 4\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.long\t%d\n", sta[$4].val); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][tag[$1]] = Var(Int, sta[$4].val, 1);
			}
			else {
				if($4){
					yyerror("Initializer Error");
					exit(0);
				}
				if(ArrInitVal.size() > sta[$2].val){
					yyerror("Too Many Initializers Error");
					exit(0);
				}
				sprintf (tmp, "\t.globl\t%s\n", tag[$1]); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t32\n");
				sprintf (tmp, "\t.type\t%s, @object\n", tag[$1]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, %d\n", tag[$1], sta[$2].val * 4); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", tag[$1]); Assemble.push_back(tmp);
				for(auto x : ArrInitVal) {
					sprintf (tmp, "\t.long\t%d\n", sta[x].val);  Assemble.push_back(tmp);
				}
				sprintf (tmp, "\t.zero\t%d\n", sta[$2].val * 4 - ArrInitVal.size() * 4); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][tag[$1]] = Var(Arr, 0, 1, sta[$2].dim);
				ArrInitVal.clear();
			}
		}
		else{
			if (!$2){
				if(!$4){
					yyerror("Initializer Error");
					exit(0);
				}
				var2reg($4, "edi");
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t%%edi, %d(%rbp)\n", offset); Assemble.push_back(tmp);
				symbol[level][tag[$1]] = Var(Int, sta[$4].val, offset);
			}
			else {
				if($4){
					yyerror("Initializer Error");
					exit(0);
				}
				if(ArrInitVal.size() > sta[$2].val){
					yyerror("Too Many Initializers Error");
					exit(0);
				}
				offset -= 4 * sta[$2].val;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", 4 * sta[$2].val); Assemble.push_back(tmp);
				for(int i = 0; i < ArrInitVal.size(); ++i){
					var2reg(ArrInitVal[i], "edi");
					sprintf (tmp, "\tmovl\t%%edi, %d(%rbp)\n", offset + i * 4);  Assemble.push_back(tmp);
				}
				symbol[level][tag[$1]] = Var(Arr, 0, offset, sta[$2].dim);
				ArrInitVal.clear();
			} 
		}
	}
	; 

InitVals: InitVal {
		if(!level && !sta[$1].is_const){		
			yyerror("Initializer Error");
			exit(0);
		}
		ArrInitVal.push_back($1);
	}
	| InitVals ',' InitVal {
		if(!level && !sta[$3].is_const){		
			yyerror("Initializer Error");
			exit(0);
		}
		ArrInitVal.push_back($3);
	}
	;

InitVal: Exp{$$ = $1;}
	| '{' '}'{$$ = 0;}
	| '{' InitVals '}'{$$ = 0;}
	;
	
FuncDef: VOID FName '(' ')' Entry_Void Block{
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
	}	
	| INT FName '(' ')' Entry_Int Block{
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
	}
	| VOID FName '(' FuncFParams ')' Entry_Void_Para Block{
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
	}
	| INT FName '(' FuncFParams ')' Entry_Int_Para Block{
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
	}
	| INT MAIN '(' ')' Entry_Main Block{
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0;
	}
	| INT error Block{printf("Function Parameter Error\n");}
	| VOID error Block{printf("Function Parameter Error\n");}
	;

FName: ID {
		strcpy(funcname, tag[$1]);
		Para.clear();
	}
	;

Entry_Main: /*empty*/ {
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		Assemble.push_back("\t.globl\tmain\n");
		Assemble.push_back("\t.type\tmain, @function\n");
		Assemble.push_back("main:\n");
		call_func_push();
	}
	;

Entry_Void: /*empty*/ {
		is_func_void = true;
		for(int i = level; i >= 0; --i)
			if(symbol[i].find(funcname) != symbol[i].end()){
				yyerror("Function Redefinition Error");
				exit(0);
			}
		symbol[level][funcname] = Var(Fvoid, 0, 0);
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		char tmp[114];
		sprintf(tmp, "\t.globl\t%s\n", funcname); Assemble.push_back(tmp);
		Assemble.push_back("\t.type\tmain, @function\n");
		sprintf(tmp, "%s:\n", funcname); Assemble.push_back(tmp);
		call_func_push();
	}
	;

Entry_Int: /*empty*/ {
		is_func_void = false;
		for(int i = level; i >= 0; --i)
			if(symbol[i].find(funcname) != symbol[i].end()){
				yyerror("Function Redefinition Error");
				exit(0);
			}
		symbol[level][funcname] = Var(Fint, 0, 0);
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		char tmp[114];
		sprintf(tmp, "\t.globl\t%s\n", funcname); Assemble.push_back(tmp);
		Assemble.push_back("\t.type\tmain, @function\n");
		sprintf(tmp, "%s:\n", funcname); Assemble.push_back(tmp);
		call_func_push();
	}
	;

Entry_Void_Para: /*empty*/ {
		is_func_void = true;
		for(int i = level; i >= 0; --i)
			if(symbol[i].find(funcname) != symbol[i].end()){
				yyerror("Function Redefinition Error");
				exit(0);
			}
		symbol[level][funcname] = Var(Fvoid, 0, 0);
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		char tmp[114];
		sprintf(tmp, "\t.globl\t%s\n", funcname); Assemble.push_back(tmp);
		Assemble.push_back("\t.type\tmain, @function\n");
		sprintf(tmp, "%s:\n", funcname); Assemble.push_back(tmp);
		call_func_push();
		for(int i = 0; i < Para.size(); ++i) {
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf(tmp, "\tmovl\t%d(%rbp), %r8d\n", 32 + i * 4); Assemble.push_back(tmp);
			sprintf(tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			symbol[level][Para[i].c_str()] = Var(Int, 0, offset);
		}
	}
	;

Entry_Int_Para: /*empty*/ {
		is_func_void = false;
		for(int i = level; i >= 0; --i)
			if(symbol[i].find(funcname) != symbol[i].end()){
				yyerror("Function Redefinition Error");
				exit(0);
			}
		symbol[level][funcname] = Var(Fint, 0, 0);
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		char tmp[114];
		sprintf(tmp, "\t.globl\t%s\n", funcname); Assemble.push_back(tmp);
		Assemble.push_back("\t.type\tmain, @function\n");
		sprintf(tmp, "%s:\n", funcname); Assemble.push_back(tmp);
		call_func_push();
		for(int i = 0; i < Para.size(); ++i) {
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf(tmp, "\tmovl\t%d(%rbp), %r8d\n", 32 + i * 4); Assemble.push_back(tmp);
			sprintf(tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			symbol[level][Para[i].c_str()] = Var(Int, 0, offset);
		}
	}
	;

FuncFParams: FuncFParam {}        
	| FuncFParams ',' FuncFParam{}        
	;

FuncFParam: INT ID ParaArr{Para.push_back(tag[$2]);}        
	;

ParaArr: /*empty*/ {}
	| '[' ']' Arr{if($3) $$ = $3;}
	;

Arr:	/*empty*/ {$$ = 0;}
	| '[' Exp ']' Arr{
		$$ = ($4) ? $4 : ++tot;
		sta[$$].dim.push_back($2);
	}
	;

Block:	'{' '}'{}        
	| '{' BlockItem '}'{}    
	| '{' error '}'{printf("Block Error\n");}
	;
	
BlockItem:	Decl{}
	| Stmt{}     
	| BlockItem Decl{}
	| BlockItem Stmt{} 
	;

Stmt:	LVal '=' Exp ';'{
		if(sta[$1].is_const){
			yyerror("Const Error");
			exit(0);
		}
		var2reg($3, "r9d");
		reg2var("r9d", $1);
	}       
	| Exp ';'{}       
	| ';'{}       
	| Block{}       
	| IF '(' Cond ')' NewLabel BeforeStmt Stmt AfterStmt %prec WITHOUTELSE {
		--level;
		symbol.pop_back();
		char tmp[114];
		++num;
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		for(auto x : sta[$3].truelist) Assemble[x] += ".L" + to_string($5) + "\n";
		for(auto x : sta[$3].falselist) Assemble[x] += ".L" + to_string(num) + "\n";
	}  
	| IF '(' Cond ')' NewLabel BeforeStmt Stmt AfterStmt ELSE AfterElse NewLabel BeforeStmt Stmt AfterStmt NewLabel{ 
		--level;
		symbol.pop_back();
		for(auto x : sta[$3].truelist) Assemble[x] += ".L" + to_string($5) + "\n";
		for(auto x : sta[$3].falselist) Assemble[x] += ".L" + to_string($11) + "\n";
		Assemble[sta[$10].truelist[0]] += ".L" + to_string($15) + "\n";
	}  
	| WHILE WhileBegin BeforeStmt '('  Cond  ')' WhileEnd NewLabel Stmt AfterStmt{
		--level;
		symbol.pop_back();
		backpatch(sta[$5].truelist, $8);
		char tmp[114];
		sprintf (tmp, "\tjmp\t.L%d\n", $2); Assemble.push_back(tmp);
		++num;
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		backpatch(sta[$5].falselist, sta[$7].quad);
		backpatch(sta[$7].truelist, num);
		for(auto it : *breaklist.rbegin()){
			sprintf(tmp, "\taddq\t$%d, %rsp\n", offset - it.second);
			Assemble[it.first - 1] = string(tmp);
			Assemble[it.first] += ".L" + to_string(num) + "\n";
		}
        	breaklist.pop_back();
		for(auto it : *continuelist.rbegin()){
			sprintf(tmp, "\taddq\t$%d, %rsp\n", offset - it.second);
			Assemble[it.first - 1] = string(tmp);
			Assemble[it.first] += ".L" + to_string($2) + "\n";
		}
        	continuelist.pop_back();
 	}  
	| BREAK ';'{
		Assemble.push_back("wait");
		Assemble.push_back("\tjmp\t");
		(*breaklist.rbegin()).push_back(make_pair(Assemble.size() - 1, offset));
	}  
	| CONTINUE ';'{
		Assemble.push_back("wait");
		Assemble.push_back("\tjmp\t");
		(*continuelist.rbegin()).push_back(make_pair(Assemble.size() - 1, offset));
	}  
	| RETURN Exp ';'{
		if(is_func_void){
			yyerror("Return Value Error");
			exit(0);
		}
		char tmp[114];
		var2reg($2, "eax");
		sprintf (tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
	}  
	| RETURN ';'{
		if(!is_func_void){
			yyerror("Return Value Error");
			exit(0);
		}
		char tmp[114];
		sprintf (tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
	}  
	| error ';'{printf("Statement Error\n");}
	;

NewLabel: /*empty*/ {
		$$ = ++num;
		char tmp[114];
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
	}
	;

BeforeStmt: /*empty*/ {
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		p_offset.push_back(offset);
		if(abs(offset) % 16 != 0){
			int padding = 16 - abs(offset) % 16;
			offset -= padding;
			char tmp[114];
			sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);
		}
	}
	;

AfterStmt: /*empty*/ {
		char tmp[114];
		int last = *p_offset.rbegin();
		sprintf (tmp, "\taddq\t$%d, %rsp\n", last - offset); Assemble.push_back(tmp);
		p_offset.pop_back();
		offset = last;
	}
	;
	
AfterElse: /*empty*/ {
		Assemble.push_back("\tjmp\t");
		sta[$$ = ++tot].truelist.push_back(Assemble.size() - 1);
	}
	;
	
WhileBegin: /*empty*/ {
		$$ = ++num;
		char tmp[114];
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		vector< pair<int, int> > x, y;
		breaklist.push_back(x);
		continuelist.push_back(y);
	}
	;

WhileEnd: /*empty*/ {
		char tmp[114];
		sta[$$ = ++tot].quad = ++num;
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		int last = *p_offset.rbegin();
		sprintf (tmp, "\taddq\t$%d, %rsp\n", last - offset); Assemble.push_back(tmp);
		Assemble.push_back("\tjmp\t");
		sta[$$].truelist.push_back(Assemble.size() - 1);
	}
	;
	
Exp: 	AddExp{$$ = $1;}  
	;

Cond:	LOrExp{$$ = $1;}  
	| error{printf("Condition Error\n");}
	;

LVal:	ID Arr{
		bool used = false;
		if(!$2){
			for(int i = level; i >= 0; --i)
				if(symbol[i].find(tag[$1]) != symbol[i].end()){
					used = true;
					Var cur = symbol[i][tag[$1]];
					$$ = ++tot;
					if(cur.type == Constint) {
						sta[$$].is_const = true;
						sta[$$].val = cur.val;
					}
					else {
						sta[$$].is_const = false;
						sta[$$].offset = cur.offset;
						if(!i) sta[$$].name = string(tag[$1]);
					}
					break;
				}
		}
		else{
			char tmp[114];
			for(int i = level; i >= 0; --i)
				if(symbol[i].find(tag[$1]) != symbol[i].end()){
					Var cur = symbol[i][tag[$1]];
					if(cur.type != Arr || sta[$2].dim.size() != cur.dim.size()) continue;
					used = true;
					int x = 1;
					offset -= 4;
					Assemble.push_back("\tsubq\t$4, %rsp\n");
					sprintf (tmp, "\tmovl\t$0, %d(%rbp)\n", offset); Assemble.push_back(tmp);
					int z = offset;
					for(int j = 0; j < cur.dim.size(); ++j){
						var2reg(sta[$2].dim[j], "r8d");
						sprintf (tmp, "\timull\t$%d, %r8d\n", x); Assemble.push_back(tmp);
						sprintf (tmp, "\taddl\t%d(%rbp), %r8d\n", z); Assemble.push_back(tmp);
						sprintf (tmp, "\tmovl\t%%r8d, %d(%rbp)\n", z); Assemble.push_back(tmp);
						x *= cur.dim[j];
					}
					$$ = ++tot;
					sta[$$].is_arr = true;
					if(cur.type == Constint){
						sta[$$].is_const = true;
						sta[$$].val = cur.val;
					}
					else{
						sta[$$].is_const = false;
						sta[$$].offset = cur.offset;
						sta[$$].offset_of_arr = z;
						if(!i) sta[$$].name = string(tag[$1]);
					}
					break;
				}
		}
		if(!used){
			yyerror("Reference Undefined Variable");
			exit(0);
		}
	}  
	;

PrimaryExp: '(' Exp ')'{$$ = $2;}  
	| LVal{$$ = $1;}  
	| NUMBER{sta[$$ = ++tot].val = $1, sta[$$].is_const = true;}  
	;

UnaryExp: PrimaryExp{$$ = $1;}  
	| ID '(' ')'{
		if(symbol[0].find(tag[$1]) == symbol[0].end()){
			yyerror("Funciton Not Defined Error");
			exit(0);
		}
		char tmp[114];
		if(abs(offset) % 16){
			int padding = 16 - abs(offset) % 16;
			offset -= padding;
			sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
		}
		sprintf (tmp, "\tcall\t%s\n", tag[$1]); Assemble.push_back(tmp);
		Var function = symbol[0][tag[$1]];
		if(function.type == Fint){
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n"); 
			sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[$$ = ++tot].offset = offset;
		}	 
	}  
	| ID '(' FuncRParams ')'{
		if(!strcmp(tag[$1], "scanf")){
			if(sta[$3].para_name.size() != 1){
				yyerror("Scanf Error");
				exit(0);
			}
			char tmp[114];
			if(abs(offset) % 16){
				int padding = 16 - abs(offset) % 16;
				offset -= padding;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
			}		
			lea_para2reg($3, 0, "rsi");
			Assemble.push_back("\tleaq\t.LC0(%rip), %rdi\n");
			Assemble.push_back("\tmovl\t$0, %eax\n");
			Assemble.push_back("\tcall\t__isoc99_scanf@PLT\n");
		}
		else if(!strcmp(tag[$1], "printf")){
			if(sta[$3].para_name.size() != 1){
				yyerror("Printf Error");
				exit(0);
			}
			char tmp[114];
			if(abs(offset) % 16){
				int padding = 16 - abs(offset) % 16;
				offset -= padding;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
			}
			para2reg($3, 0, "esi");
			Assemble.push_back("\tleaq\t.LC1(%rip), %rdi\n");
			Assemble.push_back("\tmovl\t$0, %eax\n");
			Assemble.push_back("\tcall\tprintf@PLT\n");
		}
		else{
			if(symbol[0].find(tag[$1]) == symbol[0].end()){
				yyerror("Funciton Undefined Error");
				exit(0);
			}
			char tmp[114];
			if((-(offset - 4 * sta[$3].para_name.size())) % 16){
				int padding = 16 - abs(offset) % 16;
				offset -= padding;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
			}
			for(int i = sta[$3].para_name.size() - 1; i >= 0 ; --i){
				para2reg($3, i, "r8d");
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t%%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			}
			sprintf (tmp, "\tcall\t%s\n", tag[$1]); Assemble.push_back(tmp);
			Var function = symbol[0][tag[$1]];
				if(function.type == Fint){
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
				sta[$$ = ++tot].offset = offset;
			}	
		}
	}  
	| '+' UnaryExp{$$ = $2;}  
	| '-' UnaryExp{
		if(sta[$2].is_const) sta[$$ = $2].val = -sta[$2].val;
		else{
			char tmp[114];
			var2reg($2, "r8d");
			Assemble.push_back("\tnegl\t%r8d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[$$ = ++tot].offset = offset;
		}
	}  
	| '!' UnaryExp{
		char tmp[114];
		var2reg($2, "eax");	
		Assemble.push_back("\ttestl\t%eax, %eax\n");
		Assemble.push_back("\tsete\t%al\n");
		Assemble.push_back("\tmovzbl\t%al, %eax\n");
		offset -= 4;
		Assemble.push_back("\tsubq\t$4, %rsp\n");
		sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
		sta[$$ = ++tot].offset = offset;
	}  
	;

FuncRParams: Exp{
		$$ = $1;
		sta[$$].para_name.push_back(sta[$1].name);
		sta[$$].para_val.push_back(sta[$1].val);
		sta[$$].para_offset.push_back(sta[$1].offset);
		sta[$$].para_offset_of_arr.push_back(sta[$1].offset_of_arr);
		sta[$$].para_is_const.push_back(sta[$1].is_const);
		sta[$$].para_is_arr.push_back(sta[$1].is_arr);
	}  
	| FuncRParams ',' Exp{
		$$ = $1;
		sta[$$].para_name.push_back(sta[$3].name);
		sta[$$].para_val.push_back(sta[$3].val);
		sta[$$].para_offset.push_back(sta[$3].offset);
		sta[$$].para_offset_of_arr.push_back(sta[$3].offset_of_arr);
		sta[$$].para_is_const.push_back(sta[$3].is_const);
		sta[$$].para_is_arr.push_back(sta[$3].is_arr);
	}  
	;

MulExp: UnaryExp{$$ = $1;}  
	| MulExp '*' UnaryExp{
		if(sta[$1].is_const && sta[$3].is_const)
			sta[$$ = ++tot].val = sta[$1].val * sta[$3].val, sta[$$].is_const = 1;
		else{
			char tmp[114];
			var2reg($1, "r8d");
			var2reg($3, "r9d");
			Assemble.push_back("\timull\t%r8d, %r9d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r9d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[$$ = ++tot].offset = offset;
		}
	}  
	| MulExp '/' UnaryExp{
		if(sta[$1].is_const && sta[$3].is_const)
			sta[$$ = ++tot].val = sta[$1].val / sta[$3].val, sta[$$].is_const = 1;
		else{
			char tmp[114];
			var2reg($1, "eax");
			var2reg($3, "r9d");
			Assemble.push_back("\tcltd\n");
			Assemble.push_back("\tidivl\t%r9d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[$$ = ++tot].offset = offset;
		}
	}  
	| MulExp '%' UnaryExp{
		if(sta[$1].is_const && sta[$3].is_const)
			sta[$$ = ++tot].val = sta[$1].val % sta[$3].val, sta[$$].is_const = 1;
		else{
			char tmp[114];
			var2reg($1, "eax");
			var2reg($3, "r9d");
			Assemble.push_back("\tcltd\n");
			Assemble.push_back("\tidivl\t%r9d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%%edx, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[$$ = ++tot].offset = offset;
		}
	}  
	;

AddExp: MulExp {$$ = $1;}  
	| AddExp '+' MulExp{
		if(sta[$1].is_const && sta[$3].is_const)
			sta[$$ = ++tot].val = sta[$1].val + sta[$3].val, sta[$$].is_const = 1;
		else{
			char tmp[114];
			var2reg($1, "r8d");
			var2reg($3, "r9d");
			Assemble.push_back("\taddl\t%r9d, %r8d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[$$ = ++tot].offset = offset;
		}
	}  
	| AddExp '-' MulExp{
		if(sta[$1].is_const && sta[$3].is_const)
			sta[$$ = ++tot].val = sta[$1].val - sta[$3].val, sta[$$].is_const = 1;
		else{
			char tmp[114];
			var2reg($1, "r8d");
			var2reg($3, "r9d");
			Assemble.push_back("\tsubl\t%r9d, %r8d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[$$ = ++tot].offset = offset;
		}
	}  
	;
	
RelExp: AddExp {$$ = $1;}  
	| RelExp '<' AddExp{
		char tmp[114];
		sta[$$ = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg($1, "r8d");
		var2reg($3, "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjl\t");
		sta[$$].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjge\t");
		sta[$$].falselist.push_back(Assemble.size() - 1);
	}  
	| RelExp '>' AddExp{
		char tmp[114];
		sta[$$ = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg($1, "r8d");
		var2reg($3, "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjg\t");
		sta[$$].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjle\t");
		sta[$$].falselist.push_back(Assemble.size() - 1);
	}  
	| RelExp LEQ AddExp{
		char tmp[114];
		sta[$$ = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg($1, "r8d");
		var2reg($3, "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjle\t");
		sta[$$].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjg\t");
		sta[$$].falselist.push_back(Assemble.size() - 1);
	}  
	| RelExp GEQ AddExp{
		char tmp[114];
		sta[$$ = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg($1, "r8d");
		var2reg($3, "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjge\t");
		sta[$$].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjl\t");
		sta[$$].falselist.push_back(Assemble.size() - 1);
	}  
	;
	
EqExp:	RelExp {$$ = $1;}  
	| EqExp EQ RelExp{
		char tmp[114];
		sta[$$ = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg($1, "r8d");
		var2reg($3, "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tje\t");
		sta[$$].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjne\t");
		sta[$$].falselist.push_back(Assemble.size() - 1);
	}  
	| EqExp UEQ RelExp{
		char tmp[114];
		sta[$$ = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg($1, "r8d");
		var2reg($3, "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjne\t");
		sta[$$].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tje\t");
		sta[$$].falselist.push_back(Assemble.size() - 1);
	}  
	;

LAndExp: EqExp{
		$$ = $1;
		if(!sta[$$].quad){
			char tmp[114];
			sta[$$ = ++tot].quad = ++num;
			sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
			var2reg($1, "r8d");
			Assemble.push_back("\tcmpl\t$0, %r8d\n");
			Assemble.push_back("\tjne\t");
			sta[$$].truelist.push_back(Assemble.size() - 1);
			Assemble.push_back("\tje\t");
			sta[$$].falselist.push_back(Assemble.size() - 1);
		}
	}  
	| LAndExp AND EqExp{
		backpatch(sta[$1].truelist, sta[$3].quad);
		sta[$$].falselist = merge(sta[$1].falselist, sta[$3].falselist);
		sta[$$].truelist = sta[$3].truelist;
		sta[$$].quad = sta[$1].quad;
	}  
	;
	
LOrExp:	LAndExp{ $$ = $1; }  
	| LOrExp OR LAndExp{
		backpatch(sta[$1].falselist, sta[$3].quad);
		sta[$$].truelist = merge(sta[$1].truelist, sta[$3].truelist);
		sta[$$].falselist = sta[$3].falselist;
		sta[$$].quad = sta[$1].quad;
	}  
	;

ConstExp: AddExp{
		$$ = $1;
		if(sta[$$].is_const == false){
			yyerror("Const Error");	
			exit(0);
		}
	}  
	;
%%

int yywrap() {
	return 1;
}

int main(int argc, char *argv[]) {
	freopen (argv[1], "r", stdin);
    	f = fopen("assemble.s", "w");
    	
   	Assemble.push_back(".LC0:\n");
   	Assemble.push_back("\t.string\t\"\%d\"\n");
    	Assemble.push_back(".LC1:\n");
    	Assemble.push_back("\t.string\t\"\%d\\n\"\n");
    	map<string, Var> x;
    	symbol.push_back(x);
    	p_offset.push_back(0);
    	
    	yyparse();
    	
    	for(auto x : Assemble) fprintf(f, "%s", x.c_str());
    	
	fclose(f);
	return 0;
}
