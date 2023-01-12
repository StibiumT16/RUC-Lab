%{
#include <stdio.h>
#include <stdlib.h>

extern int row, col, yyleng;
extern char *yytext;
int tot = 0;//number of tree node
int num = 0;//node number
char tag[114514][100];
FILE *f1, *f2;//f1: print the tree   f2: show the process

typedef struct {int ch[15], siz, fa, which_ch; }Node;
Node node[114514];


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

%left '>' '<' EQ UEQ GEQ LEQ
%left '+' '-'
%left '*' '/' '%'

%%

CompUnits: CompUnit{
		fprintf(f2, "CompUnits -> CompUnit\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "CompUnits");
	}
	| CompUnits CompUnit{
		fprintf(f2, "CompUnits -> CompUnits CompUnit\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2;
		strcpy(tag[$$], "CompUnits");
	}
	;

CompUnit: Decl{
		fprintf(f2, "CompUnit -> Decl\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "CompUnit");
	}
	| FuncDef{
		fprintf(f2, "CompUnit -> FuncDef\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "CompUnit");
	}
	;

Decl:	ConstDecl{
		fprintf(f2, "Decl-> ConstDecl\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "Decl");	
	}
	| VarDecl{
		fprintf(f2, "Decl-> VarDecl\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "Decl");	
	}
	;

ConstDecls: ConstDef{
		fprintf(f2, "ConstDecls -> ConstDef\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "ConstDecls");	
	}
	| ConstDecls ',' ConstDef{
		fprintf(f2, "ConstDecls -> ConstDecls ,  ConstDef\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3; 
		strcpy(tag[$$], "ConstDecls");
	}
	;

ConstDecl: CONST INT ConstDecls ';'{
		fprintf(f2, "ConstDecl -> const int ConstDecls ;\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3; 
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		strcpy(tag[$$], "ConstDecl");
	}
	| CONST error ';'{
		printf("Constant definition error!\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0; strcpy(tag[$2], "ERROR");
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "ConstDecl");
	}
	;
	
ConstDefs: '[' ConstExp ']'{
		fprintf(f2, "ConstDefs -> [ ConstExp ]\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2;
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "ConstDefs");
	}
	| ConstDefs '[' ConstExp ']'{
		fprintf(f2, "ConstDefs -> ConstDefs [ ConstExp ]\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3; 
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		strcpy(tag[$$], "ConstDefs");
	}
	;
	
ConstDef: ID '=' ConstInitVal{
		fprintf(f2, "ConstDef -> ID = ConstInitVal\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3; 
		strcpy(tag[$$], "ConstDef");
	}
	| ID ConstDefs '=' ConstInitVal{
		fprintf(f2, "ConstDef -> ID ConstDefs = ConstInitVal\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2;
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		node[$$].ch[node[$$].siz++] = $4; 
		strcpy(tag[$$], "ConstDef");
	}
	;

ConstInitVals: ConstInitVal{
		fprintf(f2, "ConstInitVals -> ConstInitVal\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "ConstInitVals");	
	}
	| ConstInitVals ',' ConstInitVal{
		fprintf(f2, "ConstInitVals -> ConstInitVals , ConstInitVal\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3; 
		strcpy(tag[$$], "ConstInitVals");
	}
	;

ConstInitVal: ConstExp{
		fprintf(f2, "ConstInitVal -> ConstExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "ConstInitVal");	
	}
	| '{' '}'{
		fprintf(f2, "ConstInitVal -> { }\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		strcpy(tag[$$], "ConstInitVal");
	}
	| '{' ConstInitVals '}'{
		fprintf(f2, "ConstInitVal -> { ConstInitVals }\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2;
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "ConstInitVal");
	}
	;

VarDecls: VarDef{
		fprintf(f2, "VarDecls -> VarDef\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "VarDecls");	
	}
	| VarDecls ',' VarDef{
		fprintf(f2, "VarDecls -> VarDecls , VarDef\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3; 
		strcpy(tag[$$], "VarDecls");
	}
	;

VarDecl: INT VarDecls ';'{
		fprintf(f2, "ValDecl -> int VarDecls ;\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2;
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "ValDecl");
	}
	| INT error ';' {
		printf("Variable definition error!\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0; strcpy(tag[$2], "ERROR");
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "VarDecl");
	}
	;

VarDefs: '[' ConstExp ']'{
		fprintf(f2, "VarDefs -> [ ConstExp ]\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2;
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "VarDefs");
	}
	| VarDefs '[' ConstExp ']'{
		fprintf(f2, "VarDefs -> VarDefs [ ConstExp ]\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3; 
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		strcpy(tag[$$], "VarDefs");
	}
	;	
	
VarDef: ID{
		fprintf(f2, "VarDef -> ID\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "VarDef");
	}
	| ID '=' InitVal{
		fprintf(f2, "VarDefs -> ID = InitVal\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3; 
		strcpy(tag[$$], "VarDef");
	}
	| ID VarDefs {
		fprintf(f2, "VarDefs -> ID VarDefs\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2;
		strcpy(tag[$$], "VarDef");
	}
	| ID VarDefs '=' InitVal{
		fprintf(f2, "VarDefs -> ID VarDefs = InitVal\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2;
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		node[$$].ch[node[$$].siz++] = $4; 
		strcpy(tag[$$], "VarDef");
	}
	; 

InitVals: InitVal{
		fprintf(f2, "InitVals -> InitVal\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "InitVals");
	}
	| InitVals ',' InitVal {
		fprintf(f2, "InitVals -> InitVals , InitVal\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "InitVals");
	}
	;

InitVal: Exp{
		fprintf(f2, "InitVal -> Exp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "InitVal");
	}
	| '{' '}'{
		fprintf(f2, "InitVal -> { }\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		strcpy(tag[$$], "InitVal");
	}
	| '{' InitVals '}'{
		fprintf(f2, "InitVal -> { InitVals }\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; 
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "InitVal");
	}
	;
	
FuncDef: VOID ID '(' ')' Block{
		fprintf(f2, "FuncDef -> void ID ( ) Block\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; 
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		node[$$].ch[node[$$].siz++] = $5;
		strcpy(tag[$$], "FuncDef");		
	}
	| INT ID '(' ')' Block{
		fprintf(f2, "FuncDef -> int ID ( ) Block\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; 
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		node[$$].ch[node[$$].siz++] = $5;
		strcpy(tag[$$], "FuncDef");		
	}
	| VOID ID '(' FuncFParams ')' Block{
		fprintf(f2, "FuncDef -> void ID ( FuncFParams ) Block\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; 
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		node[$$].ch[node[$$].siz++] = $4;
		node[$$].ch[node[$$].siz++] = $5; node[$5].siz = 0;
		node[$$].ch[node[$$].siz++] = $6;
		strcpy(tag[$$], "FuncDef");		
	}
	| INT ID '(' FuncFParams ')' Block{
		fprintf(f2, "FuncDef -> int ID ( FuncFParams ) Block\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; 
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		node[$$].ch[node[$$].siz++] = $4;
		node[$$].ch[node[$$].siz++] = $5; node[$5].siz = 0;
		node[$$].ch[node[$$].siz++] = $6;
		strcpy(tag[$$], "FuncDef");		
	}
	| INT error Block{
		printf("Function parameter error!\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0; strcpy(tag[$2], "ERROR");
		node[$$].ch[node[$$].siz++] = $3; 
		strcpy(tag[$$], "FuncDef");	
	}
	| VOID error Block{
		printf("Function parameter error!\n");	
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0; strcpy(tag[$2], "ERROR");
		node[$$].ch[node[$$].siz++] = $3; 
		strcpy(tag[$$], "FuncDef");
	}
	;

FuncFParams: FuncFParam {
		fprintf(f2, "FuncFParams -> FuncFParam\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "FuncFParams");
	}        
	| FuncFParams ',' FuncFParam{
		fprintf(f2, "FuncFParams -> FuncFParams , FuncFParam\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;  
		strcpy(tag[$$], "FuncFParams");
	}        
	;

FuncFParam: INT ID{
		fprintf(f2, "FuncFParam -> int ID\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; 
		strcpy(tag[$$], "FuncFParam");
	}        
	| INT ID '[' ']'{
		fprintf(f2, "FuncFParam -> int ID [ ]\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; 
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		strcpy(tag[$$], "FuncFParam");
	}        
	| INT ID '[' ']' LVals{
		fprintf(f2, "FuncFParam -> int ID [ ] LVals\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; 
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		node[$$].ch[node[$$].siz++] = $5;
		strcpy(tag[$$], "FuncFParam");
	}        
	;

Blocks:	BlockItem{
		fprintf(f2, "Blocks -> BlockItem\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "Blocks");
	}        
	| Blocks BlockItem{
		fprintf(f2, "Blocks -> Blocks BlockItem\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; 
		strcpy(tag[$$], "Blocks");
	}        
	;

Block:	'{' '}'{
		fprintf(f2, "Block -> { }\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		strcpy(tag[$$], "Block");
	}        
	| '{' Blocks '}'{
		fprintf(f2, "Block -> { Blocks }\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; 
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "Block");
	}    
	| '{' error '}'{
		printf("BLock error!\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0; strcpy(tag[$2], "ERROR");
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "Block");
	}
	;
	
BlockItem: Decl{
		fprintf(f2, "BlockItem -> Decl\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "BlockItem");
	}        
	| Stmt{
		fprintf(f2, "BlockItem -> Stmt\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "BlockItem");
	}        
	;

Stmt:	LVal '=' Exp ';'{
		fprintf(f2, "Stmt -> LVal = Exp ;\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3; 
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		strcpy(tag[$$], "Stmt");
	}       
	| Exp ';'{
		fprintf(f2, "Stmt -> Exp ;\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		strcpy(tag[$$], "Stmt");
	}       
	| ';'{
		fprintf(f2, "Stmt -> ;\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		strcpy(tag[$$], "Stmt");
	}       
	| Block{
		fprintf(f2, "Stmt -> Block\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "Stmt");
	}       
	| IF '(' Cond ')' Stmt %prec WITHOUTELSE {
		fprintf(f2, "Stmt -> if ( Cond ) Stmt\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		node[$$].ch[node[$$].siz++] = $5;    
		strcpy(tag[$$], "Stmt");
	}  
	| IF '(' Cond ')' Stmt ELSE Stmt {
		fprintf(f2, "Stmt -> if ( Cond ) Stmt else Stmt\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		node[$$].ch[node[$$].siz++] = $5;    
		node[$$].ch[node[$$].siz++] = $6; node[$6].siz = 0;
		node[$$].ch[node[$$].siz++] = $7; 
		strcpy(tag[$$], "Stmt");
	}  
	| WHILE '(' Cond ')' Stmt{
		fprintf(f2, "Stmt -> while ( Cond ) Stmt\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		node[$$].ch[node[$$].siz++] = $5;     
		strcpy(tag[$$], "Stmt");
	}  
	| BREAK ';'{
		fprintf(f2, "Stmt -> break ;\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		strcpy(tag[$$], "Stmt");
	}  
	| CONTINUE ';'{
		fprintf(f2, "Stmt -> continue ;\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		strcpy(tag[$$], "Stmt");
	}  
	| RETURN Exp ';'{
		fprintf(f2, "Stmt -> return Exp ;\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2;
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "Stmt");
	}  
	| RETURN ';'{
		fprintf(f2, "Stmt -> return ;\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		strcpy(tag[$$], "Stmt");
	}  
	| error ';'{
		printf("Statement error!\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0; strcpy(tag[$1], "ERROR");
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		strcpy(tag[$$], "Stmt");
	}
	;
	
Exp: 	AddExp{
		fprintf(f2, "Exp -> AddExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "Exp");
	}  
	;

Cond:	LOrExp{
		fprintf(f2, "Cond -> LOrExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "Cond");
	}  
	| error{
		printf("Condition error!\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0; strcpy(tag[$1], "ERROR");
		strcpy(tag[$$], "Cond");
	}
	;

LVals:	'[' Exp ']'{
		fprintf(f2, "LVals -> [ exp ]\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2;
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "LVals");
	}  
	| LVals '[' Exp ']'{
		fprintf(f2, "LVals -> LVals [ exp ]\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;  
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		strcpy(tag[$$], "LVals");
	}  
	;

LVal:	ID{
		fprintf(f2, "LVal -> ID\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "LVal");
	}  
	| ID LVals{
		fprintf(f2, "LVal -> ID LVals\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		node[$$].ch[node[$$].siz++] = $2;
		strcpy(tag[$$], "LVal");
	}  
	;

PrimaryExp: '(' Exp ')'{
		fprintf(f2, "PrimaryExp -> ( Exp )\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2;
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "PrimaryExp");
	}  
	| LVal{
		fprintf(f2, "PrimaryExp -> LVal\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "PrimaryExp");
	}  
	| NUMBER{
		fprintf(f2, "PrimaryExp -> Number\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "PrimaryExp");
	}  
	;
	
UnaryExp: PrimaryExp{
		fprintf(f2, "UnaryExp -> PrimaryExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; 
		strcpy(tag[$$], "UnaryExp");
	}  
	| ID '(' ')'{
		fprintf(f2, "UnaryExp -> ID ( )\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3; node[$3].siz = 0;
		strcpy(tag[$$], "UnaryExp");
	}  
	| ID '(' FuncRParams ')'{
		fprintf(f2, "UnaryExp -> ID ( FuncRParams )\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;  
		node[$$].ch[node[$$].siz++] = $4; node[$4].siz = 0;
		strcpy(tag[$$], "UnaryExp");
	}  
	| '+' UnaryExp{
		fprintf(f2, "UnaryExp -> + UnaryExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2;   
		strcpy(tag[$$], "UnaryExp");
	}  
	| '-' UnaryExp{
		fprintf(f2, "UnaryExp -> - UnaryExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2;   
		strcpy(tag[$$], "UnaryExp");
	}  
	| '!' UnaryExp{
		fprintf(f2, "UnaryExp -> ! UnaryExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1; node[$1].siz = 0;
		node[$$].ch[node[$$].siz++] = $2;   
		strcpy(tag[$$], "UnaryExp");
	}  
	;

FuncRParams: Exp{
		fprintf(f2, "FuncRParams -> Exp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "FuncRParams");
	}  
	| FuncRParams ',' Exp{
		fprintf(f2, "FuncRParams -> FuncRParams , Exp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "FuncRParams");
	}  
	;

MulExp: UnaryExp{
		fprintf(f2, "MulExp -> UnaryExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "MulExp");
	}  
	| MulExp '*' UnaryExp{
		fprintf(f2, "MulExp -> MulExp * UnaryExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "MulExp");
	}  
	| MulExp '/' UnaryExp{
		fprintf(f2, "MulExp -> MulExp / UnaryExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "MulExp");
	}  
	| MulExp '%' UnaryExp{
		fprintf(f2, "MulExp -> MulExp % UnaryExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "MulExp");
	}  
	;

AddExp: MulExp {
		fprintf(f2, "AddExp -> MulExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "AddExp");
	}  
	| AddExp '+' MulExp{
		fprintf(f2, "AddExp -> AddExp + MulExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "AddExp");
	}  
	| AddExp '-' MulExp{
		fprintf(f2, "AddExp -> AddExp - MulExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "AddExp");
	}  
	;
	
RelExp: AddExp {
		fprintf(f2, "RelExp -> AddExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "RelExp");
	}  
	| RelExp '<' AddExp{
		fprintf(f2, "RelExp -> RelExp < AddExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "RelExp");
	}  
	| RelExp '>' AddExp{
		fprintf(f2, "RelExp -> RelExp > AddExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "RelExp");
	}  
	| RelExp LEQ AddExp{
		fprintf(f2, "RelExp -> RelExp <= AddExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "RelExp");
	}  
	| RelExp GEQ AddExp{
		fprintf(f2, "RelExp -> RelExp >= AddExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "RelExp");
	}  
	;
	
EqExp:	RelExp{
		fprintf(f2, "EqExp -> RelExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "EqExp");
	}  
	| EqExp EQ RelExp{
		fprintf(f2, "EqExp -> EqExp == RelExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "EqExp");
	}  
	| EqExp UEQ RelExp{
		fprintf(f2, "EqExp -> EqExp != RelExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "EqExp");
	}  
	;

LAndExp: EqExp{
		fprintf(f2, "LAndExp -> EqExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "LAndExp");
	}  
	| LAndExp AND EqExp{
		fprintf(f2, "LAndExp -> LAndExp && EqExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "LAndExp");
	}  
	;
	
LOrExp:	LAndExp{
		fprintf(f2, "LOrExp -> LAndExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "LOrExp");
	}  
	| LOrExp OR LAndExp{
		fprintf(f2, "LOrExp -> LOrExp || LAndExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		node[$$].ch[node[$$].siz++] = $2; node[$2].siz = 0;
		node[$$].ch[node[$$].siz++] = $3;
		strcpy(tag[$$], "LOrExp");
	}  
	;

ConstExp: AddExp{
		fprintf(f2, "ConstExp -> AddExp\n");
		node[$$ = ++tot].siz = 0;
		node[$$].ch[node[$$].siz++] = $1;
		strcpy(tag[$$], "ConstExp");
	}  
	;

%%

void yyerror(const char *s){
	printf("%s:\t(%d, %d)\t %s\n", s, row, col - yyleng, yytext);
}

int yywrap() {
	return 1;
}

void dfs(int cur){
	fprintf(f1, "node%d[label = \"", ++num);
	for(int i = 0; i < node[cur].siz; ++i){
		node[node[cur].ch[i]].fa = num, node[node[cur].ch[i]].which_ch = i;
		fprintf (f1, "<f%d> %s", i, tag[node[cur].ch[i]]);
		if(i == node[cur].siz - 1) fprintf(f1, "\"];\n");
		else fprintf (f1, "|");
	}
	fprintf (f1, "\"node%d\":f%d->\"node%d\";\n", node[cur].fa, node[cur].which_ch, num);
	for(int i = 0; i < node[cur].siz; ++i)  if(node[node[cur].ch[i]].siz > 0) dfs(node[cur].ch[i]);
}

int main(int argc, char *argv[]) {
	freopen (argv[1], "r", stdin);
    	f1 = fopen("tree.dot", "w");
    	f2 = fopen("process.txt","w");
    	yyparse();
    
 	fprintf(f1, "digraph \" \"{\n");
 	fprintf(f1, "node [shape = record,height=.1]\n");
 	if(!tot) strcpy(tag[0], "Empty");
 	fprintf (f1, "node0[label = \"<f0> %s\"];\n", tag[tot]);
    	node[tot].fa = node[tot].which_ch = 0;
    	if(tot) dfs(tot);
    	fprintf(f1, "}\n");
	fclose(f1);
	fclose(f2);
	return 0;
}
	
