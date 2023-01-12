/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "./grammar.y"

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

#line 264 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    WITHOUTELSE = 260,             /* WITHOUTELSE  */
    WHILE = 261,                   /* WHILE  */
    CONTINUE = 262,                /* CONTINUE  */
    BREAK = 263,                   /* BREAK  */
    RETURN = 264,                  /* RETURN  */
    CONST = 265,                   /* CONST  */
    INT = 266,                     /* INT  */
    VOID = 267,                    /* VOID  */
    MAIN = 268,                    /* MAIN  */
    LEQ = 269,                     /* LEQ  */
    GEQ = 270,                     /* GEQ  */
    EQ = 271,                      /* EQ  */
    UEQ = 272,                     /* UEQ  */
    AND = 273,                     /* AND  */
    OR = 274,                      /* OR  */
    ID = 275,                      /* ID  */
    NUMBER = 276,                  /* NUMBER  */
    ERROR = 277                    /* ERROR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IF 258
#define ELSE 259
#define WITHOUTELSE 260
#define WHILE 261
#define CONTINUE 262
#define BREAK 263
#define RETURN 264
#define CONST 265
#define INT 266
#define VOID 267
#define MAIN 268
#define LEQ 269
#define GEQ 270
#define EQ 271
#define UEQ 272
#define AND 273
#define OR 274
#define ID 275
#define NUMBER 276
#define ERROR 277

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IF = 3,                         /* IF  */
  YYSYMBOL_ELSE = 4,                       /* ELSE  */
  YYSYMBOL_WITHOUTELSE = 5,                /* WITHOUTELSE  */
  YYSYMBOL_WHILE = 6,                      /* WHILE  */
  YYSYMBOL_CONTINUE = 7,                   /* CONTINUE  */
  YYSYMBOL_BREAK = 8,                      /* BREAK  */
  YYSYMBOL_RETURN = 9,                     /* RETURN  */
  YYSYMBOL_CONST = 10,                     /* CONST  */
  YYSYMBOL_INT = 11,                       /* INT  */
  YYSYMBOL_VOID = 12,                      /* VOID  */
  YYSYMBOL_MAIN = 13,                      /* MAIN  */
  YYSYMBOL_LEQ = 14,                       /* LEQ  */
  YYSYMBOL_GEQ = 15,                       /* GEQ  */
  YYSYMBOL_EQ = 16,                        /* EQ  */
  YYSYMBOL_UEQ = 17,                       /* UEQ  */
  YYSYMBOL_AND = 18,                       /* AND  */
  YYSYMBOL_OR = 19,                        /* OR  */
  YYSYMBOL_ID = 20,                        /* ID  */
  YYSYMBOL_NUMBER = 21,                    /* NUMBER  */
  YYSYMBOL_ERROR = 22,                     /* ERROR  */
  YYSYMBOL_23_ = 23,                       /* ';'  */
  YYSYMBOL_24_ = 24,                       /* ','  */
  YYSYMBOL_25_ = 25,                       /* '['  */
  YYSYMBOL_26_ = 26,                       /* ']'  */
  YYSYMBOL_27_ = 27,                       /* '='  */
  YYSYMBOL_28_ = 28,                       /* '{'  */
  YYSYMBOL_29_ = 29,                       /* '}'  */
  YYSYMBOL_30_ = 30,                       /* '('  */
  YYSYMBOL_31_ = 31,                       /* ')'  */
  YYSYMBOL_32_ = 32,                       /* '+'  */
  YYSYMBOL_33_ = 33,                       /* '-'  */
  YYSYMBOL_34_ = 34,                       /* '!'  */
  YYSYMBOL_35_ = 35,                       /* '*'  */
  YYSYMBOL_36_ = 36,                       /* '/'  */
  YYSYMBOL_37_ = 37,                       /* '%'  */
  YYSYMBOL_38_ = 38,                       /* '<'  */
  YYSYMBOL_39_ = 39,                       /* '>'  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_CompUnits = 41,                 /* CompUnits  */
  YYSYMBOL_CompUnit = 42,                  /* CompUnit  */
  YYSYMBOL_Decl = 43,                      /* Decl  */
  YYSYMBOL_ConstDecl = 44,                 /* ConstDecl  */
  YYSYMBOL_ConstDefs = 45,                 /* ConstDefs  */
  YYSYMBOL_ConstArr = 46,                  /* ConstArr  */
  YYSYMBOL_ConstDef = 47,                  /* ConstDef  */
  YYSYMBOL_ConstInitVals = 48,             /* ConstInitVals  */
  YYSYMBOL_ConstInitVal = 49,              /* ConstInitVal  */
  YYSYMBOL_VarDefs = 50,                   /* VarDefs  */
  YYSYMBOL_VarDecl = 51,                   /* VarDecl  */
  YYSYMBOL_VarDef = 52,                    /* VarDef  */
  YYSYMBOL_InitVals = 53,                  /* InitVals  */
  YYSYMBOL_InitVal = 54,                   /* InitVal  */
  YYSYMBOL_FuncDef = 55,                   /* FuncDef  */
  YYSYMBOL_FName = 56,                     /* FName  */
  YYSYMBOL_Entry_Main = 57,                /* Entry_Main  */
  YYSYMBOL_Entry_Void = 58,                /* Entry_Void  */
  YYSYMBOL_Entry_Int = 59,                 /* Entry_Int  */
  YYSYMBOL_Entry_Void_Para = 60,           /* Entry_Void_Para  */
  YYSYMBOL_Entry_Int_Para = 61,            /* Entry_Int_Para  */
  YYSYMBOL_FuncFParams = 62,               /* FuncFParams  */
  YYSYMBOL_FuncFParam = 63,                /* FuncFParam  */
  YYSYMBOL_ParaArr = 64,                   /* ParaArr  */
  YYSYMBOL_Arr = 65,                       /* Arr  */
  YYSYMBOL_Block = 66,                     /* Block  */
  YYSYMBOL_BlockItem = 67,                 /* BlockItem  */
  YYSYMBOL_Stmt = 68,                      /* Stmt  */
  YYSYMBOL_NewLabel = 69,                  /* NewLabel  */
  YYSYMBOL_BeforeStmt = 70,                /* BeforeStmt  */
  YYSYMBOL_AfterStmt = 71,                 /* AfterStmt  */
  YYSYMBOL_AfterElse = 72,                 /* AfterElse  */
  YYSYMBOL_WhileBegin = 73,                /* WhileBegin  */
  YYSYMBOL_WhileEnd = 74,                  /* WhileEnd  */
  YYSYMBOL_Exp = 75,                       /* Exp  */
  YYSYMBOL_Cond = 76,                      /* Cond  */
  YYSYMBOL_LVal = 77,                      /* LVal  */
  YYSYMBOL_PrimaryExp = 78,                /* PrimaryExp  */
  YYSYMBOL_UnaryExp = 79,                  /* UnaryExp  */
  YYSYMBOL_FuncRParams = 80,               /* FuncRParams  */
  YYSYMBOL_MulExp = 81,                    /* MulExp  */
  YYSYMBOL_AddExp = 82,                    /* AddExp  */
  YYSYMBOL_RelExp = 83,                    /* RelExp  */
  YYSYMBOL_EqExp = 84,                     /* EqExp  */
  YYSYMBOL_LAndExp = 85,                   /* LAndExp  */
  YYSYMBOL_LOrExp = 86,                    /* LOrExp  */
  YYSYMBOL_ConstExp = 87                   /* ConstExp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   273

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  110
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  208

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   277


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,     2,     2,    37,     2,     2,
      30,    31,    35,    32,    24,    33,     2,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    23,
      38,    27,    39,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    25,     2,    26,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    28,     2,    29,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   219,   219,   220,   223,   224,   227,   228,   231,   232,
     235,   236,   240,   241,   248,   285,   286,   289,   290,   291,
     294,   295,   298,   299,   302,   345,   425,   432,   441,   442,
     443,   446,   454,   462,   470,   478,   486,   487,   490,   496,
     507,   526,   545,   571,   597,   598,   601,   604,   605,   608,
     609,   615,   616,   617,   620,   621,   622,   623,   626,   634,
     635,   636,   637,   646,   653,   676,   681,   686,   696,   705,
     708,   715,   729,   738,   744,   754,   765,   768,   769,   772,
     833,   834,   835,   838,   839,   859,   919,   920,   932,   945,
     954,   965,   966,   980,   995,  1012,  1013,  1027,  1043,  1044,
    1056,  1068,  1080,  1094,  1095,  1107,  1121,  1135,  1143,  1144,
    1152
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE",
  "WITHOUTELSE", "WHILE", "CONTINUE", "BREAK", "RETURN", "CONST", "INT",
  "VOID", "MAIN", "LEQ", "GEQ", "EQ", "UEQ", "AND", "OR", "ID", "NUMBER",
  "ERROR", "';'", "','", "'['", "']'", "'='", "'{'", "'}'", "'('", "')'",
  "'+'", "'-'", "'!'", "'*'", "'/'", "'%'", "'<'", "'>'", "$accept",
  "CompUnits", "CompUnit", "Decl", "ConstDecl", "ConstDefs", "ConstArr",
  "ConstDef", "ConstInitVals", "ConstInitVal", "VarDefs", "VarDecl",
  "VarDef", "InitVals", "InitVal", "FuncDef", "FName", "Entry_Main",
  "Entry_Void", "Entry_Int", "Entry_Void_Para", "Entry_Int_Para",
  "FuncFParams", "FuncFParam", "ParaArr", "Arr", "Block", "BlockItem",
  "Stmt", "NewLabel", "BeforeStmt", "AfterStmt", "AfterElse", "WhileBegin",
  "WhileEnd", "Exp", "Cond", "LVal", "PrimaryExp", "UnaryExp",
  "FuncRParams", "MulExp", "AddExp", "RelExp", "EqExp", "LAndExp",
  "LOrExp", "ConstExp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    59,    44,    91,    93,    61,   123,   125,
      40,    41,    43,    45,    33,    42,    47,    37,    60,    62
};
#endif

#define YYPACT_NINF (-174)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-39)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     147,    49,    39,    19,    75,  -174,  -174,  -174,  -174,  -174,
      32,     4,    -1,    21,    47,    77,  -174,    23,    29,  -174,
      46,  -174,  -174,  -174,    69,    83,  -174,  -174,   171,  -174,
      73,   163,    86,  -174,    97,     5,  -174,     7,    93,  -174,
       4,    45,    99,  -174,   111,   117,    98,    28,    65,  -174,
    -174,  -174,   163,   163,   163,   163,  -174,  -174,   205,  -174,
     120,   142,  -174,  -174,   125,    90,  -174,  -174,    90,   145,
     105,    69,  -174,   153,  -174,    25,  -174,  -174,    30,   189,
    -174,  -174,  -174,    11,  -174,  -174,  -174,  -174,   152,   162,
     163,   223,  -174,   155,  -174,  -174,  -174,   164,  -174,  -174,
    -174,  -174,   163,   163,   163,   163,   163,   163,    29,    69,
      50,  -174,  -174,   165,    29,   177,  -174,    29,  -174,    82,
    -174,  -174,  -174,   158,    90,    -4,   129,   180,   183,   188,
    -174,   181,  -174,  -174,    57,  -174,   197,  -174,  -174,  -174,
     125,   125,  -174,  -174,  -174,    67,  -174,   198,  -174,  -174,
    -174,    29,  -174,    29,  -174,    68,  -174,  -174,   163,   163,
     163,   163,   163,   163,   163,   163,    11,   202,   163,  -174,
    -174,   105,  -174,   202,  -174,  -174,   189,  -174,  -174,    90,
      90,    90,    90,    -4,    -4,   129,   180,   199,  -174,  -174,
    -174,  -174,  -174,   239,  -174,  -174,  -174,   225,   239,  -174,
    -174,  -174,  -174,  -174,   239,  -174,  -174,  -174
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     4,     6,     7,     5,
       0,     0,     0,     0,    12,     0,    20,     0,     0,    38,
       0,     1,     3,     9,    12,     0,    10,    23,     0,    36,
       0,     0,    24,    22,     0,     0,    37,     0,     0,     8,
       0,     0,     0,    74,     0,     0,     0,     0,    49,    82,
      60,    51,     0,     0,     0,     0,    54,    61,     0,    55,
       0,    81,    83,    91,    95,    76,    39,    81,   110,     0,
       0,    12,    21,     0,    41,     0,    44,    40,     0,     0,
      11,    69,    53,     0,    71,    66,    65,    68,     0,     0,
       0,     0,    79,     0,    86,    87,    88,     0,    52,    56,
      57,    59,     0,     0,     0,     0,     0,     0,     0,    12,
       0,    25,    28,    47,     0,     0,    43,     0,    42,     0,
      14,    17,    78,     0,    98,   103,   106,   108,    77,     0,
      67,     0,    84,    89,     0,    80,     0,    92,    93,    94,
      96,    97,    35,    13,    29,     0,    26,     0,    46,    32,
      45,     0,    31,     0,    18,     0,    15,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,     0,    85,
      58,     0,    30,    49,    34,    33,     0,    19,    71,   101,
     102,    99,   100,   104,   105,   107,   109,     0,    50,    90,
      27,    48,    16,     0,    75,    72,    70,    62,     0,    73,
      72,    70,    64,    71,     0,    72,    70,    63
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -174,  -174,   227,   -21,  -174,  -174,   -20,   192,  -174,  -114,
    -174,  -174,   207,  -174,  -104,  -174,   233,  -174,  -174,  -174,
    -174,  -174,   212,   135,  -174,  -148,    -9,  -174,   -57,  -173,
    -161,  -107,  -174,  -174,  -174,   -44,    85,   -28,  -174,   -40,
    -174,    61,   -10,     1,    88,    96,  -174,   232
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     4,     5,     6,     7,    25,    32,    26,   155,   120,
      15,     8,    16,   145,   111,     9,    17,   108,   117,   114,
     153,   151,    75,    76,   148,    92,    57,    58,    59,   178,
     129,   197,   201,    84,   196,    60,   123,    67,    62,    63,
     134,    64,    65,   125,   126,   127,   128,   121
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      61,   100,    88,    29,    38,   156,   146,    56,    93,    36,
     158,   159,   122,    94,    95,    96,    73,   193,    73,   188,
      18,    68,    27,   198,    24,   191,   112,    28,   203,    89,
      61,    48,    49,   207,   160,   161,    74,    99,    77,    19,
      12,    52,   204,    53,    54,    55,   131,   133,    71,   115,
      10,    30,    13,    35,   115,    23,   116,    28,   136,    14,
      11,   118,   192,   137,   138,   139,   112,   190,    81,    68,
      48,    49,    31,   124,    82,    21,    37,   -38,   110,   144,
      52,   168,    53,    54,    55,     1,     2,     3,   169,   143,
      90,   171,   176,   202,    31,    91,   172,   177,   206,   142,
      33,    34,    48,    49,    66,   149,    39,    40,   152,    68,
     119,   154,    52,    70,    53,    54,    55,    71,    48,    49,
      79,    87,   106,   107,   189,    48,    49,   112,    52,    83,
      53,    54,    55,   110,    85,    52,   195,    53,    54,    55,
      86,   200,   174,   101,   175,   162,   163,   205,   179,   180,
     181,   182,   124,   124,   124,   124,   124,     1,     2,     3,
     103,   104,   105,   183,   184,    61,    68,   140,   141,   102,
      61,   109,    41,   113,    42,   130,    61,    43,    44,    45,
      46,     1,    47,    48,    49,    27,   135,    81,    73,   157,
     147,    48,    49,    52,    50,    53,    54,    55,   164,    28,
      51,    52,   165,    53,    54,    55,    97,   167,    42,    48,
      49,    43,    44,    45,    46,     1,    47,   119,   166,    52,
     170,    53,    54,    55,   173,    48,    49,    90,    50,   199,
     194,    22,    80,    28,    98,    52,    20,    53,    54,    55,
      97,    72,    42,    48,    49,    43,    44,    45,    46,    78,
     150,   187,   185,    52,   132,    53,    54,    55,     0,    48,
      49,   186,    50,    69,     0,     0,     0,    28,     0,    52,
       0,    53,    54,    55
};

static const yytype_int16 yycheck[] =
{
      28,    58,    46,    12,    24,   119,   110,    28,    52,    18,
      14,    15,     1,    53,    54,    55,    11,   178,    11,   167,
       1,    31,    23,   196,    20,   173,    70,    28,   201,     1,
      58,    20,    21,   206,    38,    39,    31,    58,    31,    20,
       1,    30,   203,    32,    33,    34,    90,    91,    20,    24,
       1,    30,    13,    30,    24,    23,    31,    28,   102,    20,
      11,    31,   176,   103,   104,   105,   110,   171,    23,    79,
      20,    21,    25,    83,    29,     0,    30,    30,    28,    29,
      30,    24,    32,    33,    34,    10,    11,    12,    31,   109,
      25,    24,    24,   200,    25,    30,    29,    29,   205,   108,
      23,    24,    20,    21,    31,   114,    23,    24,   117,   119,
      28,    29,    30,    27,    32,    33,    34,    20,    20,    21,
      27,    23,    32,    33,   168,    20,    21,   171,    30,    30,
      32,    33,    34,    28,    23,    30,   193,    32,    33,    34,
      23,   198,   151,    23,   153,    16,    17,   204,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    10,    11,    12,
      35,    36,    37,   162,   163,   193,   176,   106,   107,    27,
     198,    26,     1,    20,     3,    23,   204,     6,     7,     8,
       9,    10,    11,    20,    21,    23,    31,    23,    11,    31,
      25,    20,    21,    30,    23,    32,    33,    34,    18,    28,
      29,    30,    19,    32,    33,    34,     1,    26,     3,    20,
      21,     6,     7,     8,     9,    10,    11,    28,    30,    30,
      23,    32,    33,    34,    26,    20,    21,    25,    23,     4,
      31,     4,    40,    28,    29,    30,     3,    32,    33,    34,
       1,    34,     3,    20,    21,     6,     7,     8,     9,    37,
     115,   166,   164,    30,    31,    32,    33,    34,    -1,    20,
      21,   165,    23,    31,    -1,    -1,    -1,    28,    -1,    30,
      -1,    32,    33,    34
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    10,    11,    12,    41,    42,    43,    44,    51,    55,
       1,    11,     1,    13,    20,    50,    52,    56,     1,    20,
      56,     0,    42,    23,    20,    45,    47,    23,    28,    66,
      30,    25,    46,    23,    24,    30,    66,    30,    46,    23,
      24,     1,     3,     6,     7,     8,     9,    11,    20,    21,
      23,    29,    30,    32,    33,    34,    43,    66,    67,    68,
      75,    77,    78,    79,    81,    82,    31,    77,    82,    87,
      27,    20,    52,    11,    31,    62,    63,    31,    62,    27,
      47,    23,    29,    30,    73,    23,    23,    23,    75,     1,
      25,    30,    65,    75,    79,    79,    79,     1,    29,    43,
      68,    23,    27,    35,    36,    37,    32,    33,    57,    26,
      28,    54,    75,    20,    59,    24,    31,    58,    31,    28,
      49,    87,     1,    76,    82,    83,    84,    85,    86,    70,
      23,    75,    31,    75,    80,    31,    75,    79,    79,    79,
      81,    81,    66,    46,    29,    53,    54,    25,    64,    66,
      63,    61,    66,    60,    29,    48,    49,    31,    14,    15,
      38,    39,    16,    17,    18,    19,    30,    26,    24,    31,
      23,    24,    29,    26,    66,    66,    24,    29,    69,    82,
      82,    82,    82,    83,    83,    84,    85,    76,    65,    75,
      54,    65,    49,    70,    31,    68,    74,    71,    69,     4,
      68,    72,    71,    69,    70,    68,    71,    69
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    41,    42,    42,    43,    43,    44,    44,
      45,    45,    46,    46,    47,    48,    48,    49,    49,    49,
      50,    50,    51,    51,    52,    52,    53,    53,    54,    54,
      54,    55,    55,    55,    55,    55,    55,    55,    56,    57,
      58,    59,    60,    61,    62,    62,    63,    64,    64,    65,
      65,    66,    66,    66,    67,    67,    67,    67,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    76,    77,
      78,    78,    78,    79,    79,    79,    79,    79,    79,    80,
      80,    81,    81,    81,    81,    82,    82,    82,    83,    83,
      83,    83,    83,    84,    84,    84,    85,    85,    86,    86,
      87
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     4,     3,
       1,     3,     0,     4,     4,     1,     3,     1,     2,     3,
       1,     3,     3,     3,     2,     4,     1,     3,     1,     2,
       3,     6,     6,     7,     7,     6,     3,     3,     1,     0,
       0,     0,     0,     0,     1,     3,     3,     0,     3,     0,
       4,     2,     3,     3,     1,     1,     2,     2,     4,     2,
       1,     1,     8,    15,    10,     2,     2,     3,     2,     2,
       0,     0,     0,     0,     0,     0,     1,     1,     1,     2,
       3,     1,     1,     1,     3,     4,     2,     2,     2,     1,
       3,     1,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     1,     3,
       1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* CompUnits: CompUnit  */
#line 219 "./grammar.y"
                   {}
#line 1585 "y.tab.c"
    break;

  case 3: /* CompUnits: CompUnits CompUnit  */
#line 220 "./grammar.y"
                            {}
#line 1591 "y.tab.c"
    break;

  case 4: /* CompUnit: Decl  */
#line 223 "./grammar.y"
              {}
#line 1597 "y.tab.c"
    break;

  case 5: /* CompUnit: FuncDef  */
#line 224 "./grammar.y"
                 {}
#line 1603 "y.tab.c"
    break;

  case 6: /* Decl: ConstDecl  */
#line 227 "./grammar.y"
                 {}
#line 1609 "y.tab.c"
    break;

  case 7: /* Decl: VarDecl  */
#line 228 "./grammar.y"
                 {}
#line 1615 "y.tab.c"
    break;

  case 8: /* ConstDecl: CONST INT ConstDefs ';'  */
#line 231 "./grammar.y"
                                  {}
#line 1621 "y.tab.c"
    break;

  case 9: /* ConstDecl: CONST error ';'  */
#line 232 "./grammar.y"
                         {printf("Const Definition Error\n");}
#line 1627 "y.tab.c"
    break;

  case 10: /* ConstDefs: ConstDef  */
#line 235 "./grammar.y"
                   {}
#line 1633 "y.tab.c"
    break;

  case 11: /* ConstDefs: ConstDefs ',' ConstDef  */
#line 236 "./grammar.y"
                                {}
#line 1639 "y.tab.c"
    break;

  case 12: /* ConstArr: %empty  */
#line 240 "./grammar.y"
                    {yyval = 0;}
#line 1645 "y.tab.c"
    break;

  case 13: /* ConstArr: '[' ConstExp ']' ConstArr  */
#line 241 "./grammar.y"
                                   {
		if(yyvsp[0]) sta[yyval = yyvsp[0]].val = sta[yyvsp[-2]].val * sta[yyvsp[0]].val;
		else sta[yyval = ++tot].val = sta[yyvsp[-2]].val;
		sta[yyval].dim.push_back(sta[yyvsp[-2]].val);
	}
#line 1655 "y.tab.c"
    break;

  case 14: /* ConstDef: ID ConstArr '=' ConstInitVal  */
#line 248 "./grammar.y"
                                      {
		if(symbol[level].find(tag[yyvsp[-3]]) != symbol[level].end()){
			yyerror("Const Redefinition Error");
			exit(0);
		}
		char tmp[114];
		if(!level){
			if(!sta[yyvsp[0]].is_const){
				printf("Const Definition Error\n");
				exit(0);
			}
			if (!yyvsp[-2]){
				Assemble.push_back("\t.section\t.rodata\n");
				Assemble.push_back("\t.align\t4\n");
				sprintf (tmp, "\t.type\t%s, @object\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, 4\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.long\t%d\n", sta[yyvsp[0]].val); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][tag[yyvsp[-3]]] = Var(Constint, sta[yyvsp[0]].val, 1);
			}
			//else ?
		}
		else{
			if (!yyvsp[-2]){
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t$%d, %%edi\n", sta[yyvsp[0]].val); Assemble.push_back(tmp);
				sprintf (tmp, "\tmovl\t%%edi, %d(%rbp)\n", offset); Assemble.push_back(tmp);
				symbol[level][tag[yyvsp[-3]]] = Var(Constint, sta[yyvsp[0]].val, offset);
			}
			//else ?
		}
		
	}
#line 1695 "y.tab.c"
    break;

  case 15: /* ConstInitVals: ConstInitVal  */
#line 285 "./grammar.y"
                           {}
#line 1701 "y.tab.c"
    break;

  case 16: /* ConstInitVals: ConstInitVals ',' ConstInitVal  */
#line 286 "./grammar.y"
                                        {}
#line 1707 "y.tab.c"
    break;

  case 17: /* ConstInitVal: ConstExp  */
#line 289 "./grammar.y"
                      {yyval = yyvsp[0];}
#line 1713 "y.tab.c"
    break;

  case 18: /* ConstInitVal: '{' '}'  */
#line 290 "./grammar.y"
                 {}
#line 1719 "y.tab.c"
    break;

  case 19: /* ConstInitVal: '{' ConstInitVals '}'  */
#line 291 "./grammar.y"
                               {}
#line 1725 "y.tab.c"
    break;

  case 20: /* VarDefs: VarDef  */
#line 294 "./grammar.y"
               {}
#line 1731 "y.tab.c"
    break;

  case 21: /* VarDefs: VarDefs ',' VarDef  */
#line 295 "./grammar.y"
                            {}
#line 1737 "y.tab.c"
    break;

  case 22: /* VarDecl: INT VarDefs ';'  */
#line 298 "./grammar.y"
                        {}
#line 1743 "y.tab.c"
    break;

  case 23: /* VarDecl: INT error ';'  */
#line 299 "./grammar.y"
                        {printf("Variable Definition Error\n");}
#line 1749 "y.tab.c"
    break;

  case 24: /* VarDef: ID ConstArr  */
#line 302 "./grammar.y"
                   {
		if(symbol[level].find(tag[yyvsp[-1]]) != symbol[level].end()){
			yyerror("Variable Redefinition Error");
			exit(0);
		}
		char tmp[114];
		if(!level){
			if(!yyvsp[0]){
				sprintf (tmp, "\t.globl\t%s\n", tag[yyvsp[-1]]); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t4\n");
				sprintf (tmp, "\t.type\t%s, @object\n", tag[yyvsp[-1]]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, 4\n", tag[yyvsp[-1]]); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", tag[yyvsp[-1]]); Assemble.push_back(tmp);
				Assemble.push_back("\t.long\t0\n");
				Assemble.push_back("\t.text\n");
				symbol[0][tag[yyvsp[-1]]] = Var(Int, 0, 1);
			}
			else{
				sprintf (tmp, "\t.globl\t%s\n", tag[yyvsp[-1]]); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t32\n");
				sprintf (tmp, "\t.type\t%s, @object\n", tag[yyvsp[-1]]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, %d\n", tag[yyvsp[-1]], sta[yyvsp[0]].val * 4); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", tag[yyvsp[-1]]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.zero\t%d\n", sta[yyvsp[0]].val * 4); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][tag[yyvsp[-1]]] = Var(Arr, 0, 1, sta[yyvsp[0]].dim);
			}
		}
		else{
			if(!yyvsp[0]){
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				symbol[level][tag[yyvsp[-1]]] = Var(Int, 0, offset);
			}
			else{
				offset -= 4 * sta[yyvsp[0]].val;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", sta[yyvsp[0]].val * 4); Assemble.push_back(tmp);
				symbol[level][tag[yyvsp[-1]]] = Var(Arr, 0, offset, sta[yyvsp[0]].dim);
			}
		}
	}
#line 1797 "y.tab.c"
    break;

  case 25: /* VarDef: ID ConstArr '=' InitVal  */
#line 345 "./grammar.y"
                                 {
		if(symbol[level].find(tag[yyvsp[-3]]) != symbol[level].end()){
			yyerror("Variable Redefinition Error");
			exit(0);
		}
		char tmp[114];
		if(!level){
			if (!yyvsp[-2]){
				if(!yyvsp[0] || !sta[yyvsp[0]].is_const){
					yyerror("Initializer Error");
					exit(0);
				}
				sprintf (tmp, "\t.globl\t%s\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t4\n");
				sprintf (tmp, "\t.type\t%s, @object\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, 4\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.long\t%d\n", sta[yyvsp[0]].val); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][tag[yyvsp[-3]]] = Var(Int, sta[yyvsp[0]].val, 1);
			}
			else {
				if(yyvsp[0]){
					yyerror("Initializer Error");
					exit(0);
				}
				if(ArrInitVal.size() > sta[yyvsp[-2]].val){
					yyerror("Too Many Initializers Error");
					exit(0);
				}
				sprintf (tmp, "\t.globl\t%s\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
				Assemble.push_back("\t.data\n");
				Assemble.push_back("\t.align\t32\n");
				sprintf (tmp, "\t.type\t%s, @object\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
				sprintf (tmp, "\t.size\t%s, %d\n", tag[yyvsp[-3]], sta[yyvsp[-2]].val * 4); Assemble.push_back(tmp);
				sprintf (tmp, "%s:\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
				for(auto x : ArrInitVal) {
					sprintf (tmp, "\t.long\t%d\n", sta[x].val);  Assemble.push_back(tmp);
				}
				sprintf (tmp, "\t.zero\t%d\n", sta[yyvsp[-2]].val * 4 - ArrInitVal.size() * 4); Assemble.push_back(tmp);
				Assemble.push_back("\t.text\n");
				symbol[0][tag[yyvsp[-3]]] = Var(Arr, 0, 1, sta[yyvsp[-2]].dim);
				ArrInitVal.clear();
			}
		}
		else{
			if (!yyvsp[-2]){
				if(!yyvsp[0]){
					yyerror("Initializer Error");
					exit(0);
				}
				var2reg(yyvsp[0], "edi");
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t%%edi, %d(%rbp)\n", offset); Assemble.push_back(tmp);
				symbol[level][tag[yyvsp[-3]]] = Var(Int, sta[yyvsp[0]].val, offset);
			}
			else {
				if(yyvsp[0]){
					yyerror("Initializer Error");
					exit(0);
				}
				if(ArrInitVal.size() > sta[yyvsp[-2]].val){
					yyerror("Too Many Initializers Error");
					exit(0);
				}
				offset -= 4 * sta[yyvsp[-2]].val;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", 4 * sta[yyvsp[-2]].val); Assemble.push_back(tmp);
				for(int i = 0; i < ArrInitVal.size(); ++i){
					var2reg(ArrInitVal[i], "edi");
					sprintf (tmp, "\tmovl\t%%edi, %d(%rbp)\n", offset + i * 4);  Assemble.push_back(tmp);
				}
				symbol[level][tag[yyvsp[-3]]] = Var(Arr, 0, offset, sta[yyvsp[-2]].dim);
				ArrInitVal.clear();
			} 
		}
	}
#line 1880 "y.tab.c"
    break;

  case 26: /* InitVals: InitVal  */
#line 425 "./grammar.y"
                  {
		if(!level && !sta[yyvsp[0]].is_const){		
			yyerror("Initializer Error");
			exit(0);
		}
		ArrInitVal.push_back(yyvsp[0]);
	}
#line 1892 "y.tab.c"
    break;

  case 27: /* InitVals: InitVals ',' InitVal  */
#line 432 "./grammar.y"
                               {
		if(!level && !sta[yyvsp[0]].is_const){		
			yyerror("Initializer Error");
			exit(0);
		}
		ArrInitVal.push_back(yyvsp[0]);
	}
#line 1904 "y.tab.c"
    break;

  case 28: /* InitVal: Exp  */
#line 441 "./grammar.y"
            {yyval = yyvsp[0];}
#line 1910 "y.tab.c"
    break;

  case 29: /* InitVal: '{' '}'  */
#line 442 "./grammar.y"
                 {yyval = 0;}
#line 1916 "y.tab.c"
    break;

  case 30: /* InitVal: '{' InitVals '}'  */
#line 443 "./grammar.y"
                          {yyval = 0;}
#line 1922 "y.tab.c"
    break;

  case 31: /* FuncDef: VOID FName '(' ')' Entry_Void Block  */
#line 446 "./grammar.y"
                                            {
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
	}
#line 1935 "y.tab.c"
    break;

  case 32: /* FuncDef: INT FName '(' ')' Entry_Int Block  */
#line 454 "./grammar.y"
                                           {
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
	}
#line 1948 "y.tab.c"
    break;

  case 33: /* FuncDef: VOID FName '(' FuncFParams ')' Entry_Void_Para Block  */
#line 462 "./grammar.y"
                                                              {
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
	}
#line 1961 "y.tab.c"
    break;

  case 34: /* FuncDef: INT FName '(' FuncFParams ')' Entry_Int_Para Block  */
#line 470 "./grammar.y"
                                                            {
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0, is_func_void = false;
	}
#line 1974 "y.tab.c"
    break;

  case 35: /* FuncDef: INT MAIN '(' ')' Entry_Main Block  */
#line 478 "./grammar.y"
                                           {
		--level;
		symbol.pop_back();
		char tmp[114];
		sprintf(tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
		offset = 0;
	}
#line 1987 "y.tab.c"
    break;

  case 36: /* FuncDef: INT error Block  */
#line 486 "./grammar.y"
                         {printf("Function Parameter Error\n");}
#line 1993 "y.tab.c"
    break;

  case 37: /* FuncDef: VOID error Block  */
#line 487 "./grammar.y"
                          {printf("Function Parameter Error\n");}
#line 1999 "y.tab.c"
    break;

  case 38: /* FName: ID  */
#line 490 "./grammar.y"
          {
		strcpy(funcname, tag[yyvsp[0]]);
		Para.clear();
	}
#line 2008 "y.tab.c"
    break;

  case 39: /* Entry_Main: %empty  */
#line 496 "./grammar.y"
                      {
		++level;
		map<string, Var> x;
		symbol.push_back(x);
		Assemble.push_back("\t.globl\tmain\n");
		Assemble.push_back("\t.type\tmain, @function\n");
		Assemble.push_back("main:\n");
		call_func_push();
	}
#line 2022 "y.tab.c"
    break;

  case 40: /* Entry_Void: %empty  */
#line 507 "./grammar.y"
                      {
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
#line 2044 "y.tab.c"
    break;

  case 41: /* Entry_Int: %empty  */
#line 526 "./grammar.y"
                     {
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
#line 2066 "y.tab.c"
    break;

  case 42: /* Entry_Void_Para: %empty  */
#line 545 "./grammar.y"
                           {
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
#line 2095 "y.tab.c"
    break;

  case 43: /* Entry_Int_Para: %empty  */
#line 571 "./grammar.y"
                          {
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
#line 2124 "y.tab.c"
    break;

  case 44: /* FuncFParams: FuncFParam  */
#line 597 "./grammar.y"
                        {}
#line 2130 "y.tab.c"
    break;

  case 45: /* FuncFParams: FuncFParams ',' FuncFParam  */
#line 598 "./grammar.y"
                                    {}
#line 2136 "y.tab.c"
    break;

  case 46: /* FuncFParam: INT ID ParaArr  */
#line 601 "./grammar.y"
                          {Para.push_back(tag[yyvsp[-1]]);}
#line 2142 "y.tab.c"
    break;

  case 47: /* ParaArr: %empty  */
#line 604 "./grammar.y"
                   {}
#line 2148 "y.tab.c"
    break;

  case 48: /* ParaArr: '[' ']' Arr  */
#line 605 "./grammar.y"
                     {if(yyvsp[0]) yyval = yyvsp[0];}
#line 2154 "y.tab.c"
    break;

  case 49: /* Arr: %empty  */
#line 608 "./grammar.y"
                  {yyval = 0;}
#line 2160 "y.tab.c"
    break;

  case 50: /* Arr: '[' Exp ']' Arr  */
#line 609 "./grammar.y"
                         {
		yyval = (yyvsp[0]) ? yyvsp[0] : ++tot;
		sta[yyval].dim.push_back(yyvsp[-2]);
	}
#line 2169 "y.tab.c"
    break;

  case 51: /* Block: '{' '}'  */
#line 615 "./grammar.y"
               {}
#line 2175 "y.tab.c"
    break;

  case 52: /* Block: '{' BlockItem '}'  */
#line 616 "./grammar.y"
                           {}
#line 2181 "y.tab.c"
    break;

  case 53: /* Block: '{' error '}'  */
#line 617 "./grammar.y"
                       {printf("Block Error\n");}
#line 2187 "y.tab.c"
    break;

  case 54: /* BlockItem: Decl  */
#line 620 "./grammar.y"
                    {}
#line 2193 "y.tab.c"
    break;

  case 55: /* BlockItem: Stmt  */
#line 621 "./grammar.y"
              {}
#line 2199 "y.tab.c"
    break;

  case 56: /* BlockItem: BlockItem Decl  */
#line 622 "./grammar.y"
                        {}
#line 2205 "y.tab.c"
    break;

  case 57: /* BlockItem: BlockItem Stmt  */
#line 623 "./grammar.y"
                        {}
#line 2211 "y.tab.c"
    break;

  case 58: /* Stmt: LVal '=' Exp ';'  */
#line 626 "./grammar.y"
                        {
		if(sta[yyvsp[-3]].is_const){
			yyerror("Const Error");
			exit(0);
		}
		var2reg(yyvsp[-1], "r9d");
		reg2var("r9d", yyvsp[-3]);
	}
#line 2224 "y.tab.c"
    break;

  case 59: /* Stmt: Exp ';'  */
#line 634 "./grammar.y"
                 {}
#line 2230 "y.tab.c"
    break;

  case 60: /* Stmt: ';'  */
#line 635 "./grammar.y"
             {}
#line 2236 "y.tab.c"
    break;

  case 61: /* Stmt: Block  */
#line 636 "./grammar.y"
               {}
#line 2242 "y.tab.c"
    break;

  case 62: /* Stmt: IF '(' Cond ')' NewLabel BeforeStmt Stmt AfterStmt  */
#line 637 "./grammar.y"
                                                                               {
		--level;
		symbol.pop_back();
		char tmp[114];
		++num;
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		for(auto x : sta[yyvsp[-5]].truelist) Assemble[x] += ".L" + to_string(yyvsp[-3]) + "\n";
		for(auto x : sta[yyvsp[-5]].falselist) Assemble[x] += ".L" + to_string(num) + "\n";
	}
#line 2256 "y.tab.c"
    break;

  case 63: /* Stmt: IF '(' Cond ')' NewLabel BeforeStmt Stmt AfterStmt ELSE AfterElse NewLabel BeforeStmt Stmt AfterStmt NewLabel  */
#line 646 "./grammar.y"
                                                                                                                       { 
		--level;
		symbol.pop_back();
		for(auto x : sta[yyvsp[-12]].truelist) Assemble[x] += ".L" + to_string(yyvsp[-10]) + "\n";
		for(auto x : sta[yyvsp[-12]].falselist) Assemble[x] += ".L" + to_string(yyvsp[-4]) + "\n";
		Assemble[sta[yyvsp[-5]].truelist[0]] += ".L" + to_string(yyvsp[0]) + "\n";
	}
#line 2268 "y.tab.c"
    break;

  case 64: /* Stmt: WHILE WhileBegin BeforeStmt '(' Cond ')' WhileEnd NewLabel Stmt AfterStmt  */
#line 653 "./grammar.y"
                                                                                     {
		--level;
		symbol.pop_back();
		backpatch(sta[yyvsp[-5]].truelist, yyvsp[-2]);
		char tmp[114];
		sprintf (tmp, "\tjmp\t.L%d\n", yyvsp[-8]); Assemble.push_back(tmp);
		++num;
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		backpatch(sta[yyvsp[-5]].falselist, sta[yyvsp[-3]].quad);
		backpatch(sta[yyvsp[-3]].truelist, num);
		for(auto it : *breaklist.rbegin()){
			sprintf(tmp, "\taddq\t$%d, %rsp\n", offset - it.second);
			Assemble[it.first - 1] = string(tmp);
			Assemble[it.first] += ".L" + to_string(num) + "\n";
		}
        	breaklist.pop_back();
		for(auto it : *continuelist.rbegin()){
			sprintf(tmp, "\taddq\t$%d, %rsp\n", offset - it.second);
			Assemble[it.first - 1] = string(tmp);
			Assemble[it.first] += ".L" + to_string(yyvsp[-8]) + "\n";
		}
        	continuelist.pop_back();
 	}
#line 2296 "y.tab.c"
    break;

  case 65: /* Stmt: BREAK ';'  */
#line 676 "./grammar.y"
                   {
		Assemble.push_back("wait");
		Assemble.push_back("\tjmp\t");
		(*breaklist.rbegin()).push_back(make_pair(Assemble.size() - 1, offset));
	}
#line 2306 "y.tab.c"
    break;

  case 66: /* Stmt: CONTINUE ';'  */
#line 681 "./grammar.y"
                      {
		Assemble.push_back("wait");
		Assemble.push_back("\tjmp\t");
		(*continuelist.rbegin()).push_back(make_pair(Assemble.size() - 1, offset));
	}
#line 2316 "y.tab.c"
    break;

  case 67: /* Stmt: RETURN Exp ';'  */
#line 686 "./grammar.y"
                        {
		if(is_func_void){
			yyerror("Return Value Error");
			exit(0);
		}
		char tmp[114];
		var2reg(yyvsp[-1], "eax");
		sprintf (tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
	}
#line 2331 "y.tab.c"
    break;

  case 68: /* Stmt: RETURN ';'  */
#line 696 "./grammar.y"
                    {
		if(!is_func_void){
			yyerror("Return Value Error");
			exit(0);
		}
		char tmp[114];
		sprintf (tmp, "\taddq\t$%d, %rsp\n", -offset); Assemble.push_back(tmp);
		ret_func_pop();
	}
#line 2345 "y.tab.c"
    break;

  case 69: /* Stmt: error ';'  */
#line 705 "./grammar.y"
                   {printf("Statement Error\n");}
#line 2351 "y.tab.c"
    break;

  case 70: /* NewLabel: %empty  */
#line 708 "./grammar.y"
                    {
		yyval = ++num;
		char tmp[114];
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
	}
#line 2361 "y.tab.c"
    break;

  case 71: /* BeforeStmt: %empty  */
#line 715 "./grammar.y"
                      {
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
#line 2378 "y.tab.c"
    break;

  case 72: /* AfterStmt: %empty  */
#line 729 "./grammar.y"
                     {
		char tmp[114];
		int last = *p_offset.rbegin();
		sprintf (tmp, "\taddq\t$%d, %rsp\n", last - offset); Assemble.push_back(tmp);
		p_offset.pop_back();
		offset = last;
	}
#line 2390 "y.tab.c"
    break;

  case 73: /* AfterElse: %empty  */
#line 738 "./grammar.y"
                     {
		Assemble.push_back("\tjmp\t");
		sta[yyval = ++tot].truelist.push_back(Assemble.size() - 1);
	}
#line 2399 "y.tab.c"
    break;

  case 74: /* WhileBegin: %empty  */
#line 744 "./grammar.y"
                      {
		yyval = ++num;
		char tmp[114];
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		vector< pair<int, int> > x, y;
		breaklist.push_back(x);
		continuelist.push_back(y);
	}
#line 2412 "y.tab.c"
    break;

  case 75: /* WhileEnd: %empty  */
#line 754 "./grammar.y"
                    {
		char tmp[114];
		sta[yyval = ++tot].quad = ++num;
		sprintf (tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		int last = *p_offset.rbegin();
		sprintf (tmp, "\taddq\t$%d, %rsp\n", last - offset); Assemble.push_back(tmp);
		Assemble.push_back("\tjmp\t");
		sta[yyval].truelist.push_back(Assemble.size() - 1);
	}
#line 2426 "y.tab.c"
    break;

  case 76: /* Exp: AddExp  */
#line 765 "./grammar.y"
              {yyval = yyvsp[0];}
#line 2432 "y.tab.c"
    break;

  case 77: /* Cond: LOrExp  */
#line 768 "./grammar.y"
              {yyval = yyvsp[0];}
#line 2438 "y.tab.c"
    break;

  case 78: /* Cond: error  */
#line 769 "./grammar.y"
               {printf("Condition Error\n");}
#line 2444 "y.tab.c"
    break;

  case 79: /* LVal: ID Arr  */
#line 772 "./grammar.y"
              {
		bool used = false;
		if(!yyvsp[0]){
			for(int i = level; i >= 0; --i)
				if(symbol[i].find(tag[yyvsp[-1]]) != symbol[i].end()){
					used = true;
					Var cur = symbol[i][tag[yyvsp[-1]]];
					yyval = ++tot;
					if(cur.type == Constint) {
						sta[yyval].is_const = true;
						sta[yyval].val = cur.val;
					}
					else {
						sta[yyval].is_const = false;
						sta[yyval].offset = cur.offset;
						if(!i) sta[yyval].name = string(tag[yyvsp[-1]]);
					}
					break;
				}
		}
		else{
			char tmp[114];
			for(int i = level; i >= 0; --i)
				if(symbol[i].find(tag[yyvsp[-1]]) != symbol[i].end()){
					Var cur = symbol[i][tag[yyvsp[-1]]];
					if(cur.type != Arr || sta[yyvsp[0]].dim.size() != cur.dim.size()) continue;
					used = true;
					int x = 1;
					offset -= 4;
					Assemble.push_back("\tsubq\t$4, %rsp\n");
					sprintf (tmp, "\tmovl\t$0, %d(%rbp)\n", offset); Assemble.push_back(tmp);
					int z = offset;
					for(int j = 0; j < cur.dim.size(); ++j){
						var2reg(sta[yyvsp[0]].dim[j], "r8d");
						sprintf (tmp, "\timull\t$%d, %r8d\n", x); Assemble.push_back(tmp);
						sprintf (tmp, "\taddl\t%d(%rbp), %r8d\n", z); Assemble.push_back(tmp);
						sprintf (tmp, "\tmovl\t%%r8d, %d(%rbp)\n", z); Assemble.push_back(tmp);
						x *= cur.dim[j];
					}
					yyval = ++tot;
					sta[yyval].is_arr = true;
					if(cur.type == Constint){
						sta[yyval].is_const = true;
						sta[yyval].val = cur.val;
					}
					else{
						sta[yyval].is_const = false;
						sta[yyval].offset = cur.offset;
						sta[yyval].offset_of_arr = z;
						if(!i) sta[yyval].name = string(tag[yyvsp[-1]]);
					}
					break;
				}
		}
		if(!used){
			yyerror("Reference Undefined Variable");
			exit(0);
		}
	}
#line 2508 "y.tab.c"
    break;

  case 80: /* PrimaryExp: '(' Exp ')'  */
#line 833 "./grammar.y"
                       {yyval = yyvsp[-1];}
#line 2514 "y.tab.c"
    break;

  case 81: /* PrimaryExp: LVal  */
#line 834 "./grammar.y"
              {yyval = yyvsp[0];}
#line 2520 "y.tab.c"
    break;

  case 82: /* PrimaryExp: NUMBER  */
#line 835 "./grammar.y"
                {sta[yyval = ++tot].val = yyvsp[0], sta[yyval].is_const = true;}
#line 2526 "y.tab.c"
    break;

  case 83: /* UnaryExp: PrimaryExp  */
#line 838 "./grammar.y"
                    {yyval = yyvsp[0];}
#line 2532 "y.tab.c"
    break;

  case 84: /* UnaryExp: ID '(' ')'  */
#line 839 "./grammar.y"
                    {
		if(symbol[0].find(tag[yyvsp[-2]]) == symbol[0].end()){
			yyerror("Funciton Not Defined Error");
			exit(0);
		}
		char tmp[114];
		if(abs(offset) % 16){
			int padding = 16 - abs(offset) % 16;
			offset -= padding;
			sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
		}
		sprintf (tmp, "\tcall\t%s\n", tag[yyvsp[-2]]); Assemble.push_back(tmp);
		Var function = symbol[0][tag[yyvsp[-2]]];
		if(function.type == Fint){
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n"); 
			sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[yyval = ++tot].offset = offset;
		}	 
	}
#line 2557 "y.tab.c"
    break;

  case 85: /* UnaryExp: ID '(' FuncRParams ')'  */
#line 859 "./grammar.y"
                                {
		if(!strcmp(tag[yyvsp[-3]], "scanf")){
			if(sta[yyvsp[-1]].para_name.size() != 1){
				yyerror("Scanf Error");
				exit(0);
			}
			char tmp[114];
			if(abs(offset) % 16){
				int padding = 16 - abs(offset) % 16;
				offset -= padding;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
			}		
			lea_para2reg(yyvsp[-1], 0, "rsi");
			Assemble.push_back("\tleaq\t.LC0(%rip), %rdi\n");
			Assemble.push_back("\tmovl\t$0, %eax\n");
			Assemble.push_back("\tcall\t__isoc99_scanf@PLT\n");
		}
		else if(!strcmp(tag[yyvsp[-3]], "printf")){
			if(sta[yyvsp[-1]].para_name.size() != 1){
				yyerror("Printf Error");
				exit(0);
			}
			char tmp[114];
			if(abs(offset) % 16){
				int padding = 16 - abs(offset) % 16;
				offset -= padding;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
			}
			para2reg(yyvsp[-1], 0, "esi");
			Assemble.push_back("\tleaq\t.LC1(%rip), %rdi\n");
			Assemble.push_back("\tmovl\t$0, %eax\n");
			Assemble.push_back("\tcall\tprintf@PLT\n");
		}
		else{
			if(symbol[0].find(tag[yyvsp[-3]]) == symbol[0].end()){
				yyerror("Funciton Undefined Error");
				exit(0);
			}
			char tmp[114];
			if((-(offset - 4 * sta[yyvsp[-1]].para_name.size())) % 16){
				int padding = 16 - abs(offset) % 16;
				offset -= padding;
				sprintf (tmp, "\tsubq\t$%d, %rsp\n", padding); Assemble.push_back(tmp);	 
			}
			for(int i = sta[yyvsp[-1]].para_name.size() - 1; i >= 0 ; --i){
				para2reg(yyvsp[-1], i, "r8d");
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t%%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			}
			sprintf (tmp, "\tcall\t%s\n", tag[yyvsp[-3]]); Assemble.push_back(tmp);
			Var function = symbol[0][tag[yyvsp[-3]]];
				if(function.type == Fint){
				offset -= 4;
				Assemble.push_back("\tsubq\t$4, %rsp\n");
				sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
				sta[yyval = ++tot].offset = offset;
			}	
		}
	}
#line 2622 "y.tab.c"
    break;

  case 86: /* UnaryExp: '+' UnaryExp  */
#line 919 "./grammar.y"
                      {yyval = yyvsp[0];}
#line 2628 "y.tab.c"
    break;

  case 87: /* UnaryExp: '-' UnaryExp  */
#line 920 "./grammar.y"
                      {
		if(sta[yyvsp[0]].is_const) sta[yyval = yyvsp[0]].val = -sta[yyvsp[0]].val;
		else{
			char tmp[114];
			var2reg(yyvsp[0], "r8d");
			Assemble.push_back("\tnegl\t%r8d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[yyval = ++tot].offset = offset;
		}
	}
#line 2645 "y.tab.c"
    break;

  case 88: /* UnaryExp: '!' UnaryExp  */
#line 932 "./grammar.y"
                      {
		char tmp[114];
		var2reg(yyvsp[0], "eax");	
		Assemble.push_back("\ttestl\t%eax, %eax\n");
		Assemble.push_back("\tsete\t%al\n");
		Assemble.push_back("\tmovzbl\t%al, %eax\n");
		offset -= 4;
		Assemble.push_back("\tsubq\t$4, %rsp\n");
		sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
		sta[yyval = ++tot].offset = offset;
	}
#line 2661 "y.tab.c"
    break;

  case 89: /* FuncRParams: Exp  */
#line 945 "./grammar.y"
                {
		yyval = yyvsp[0];
		sta[yyval].para_name.push_back(sta[yyvsp[0]].name);
		sta[yyval].para_val.push_back(sta[yyvsp[0]].val);
		sta[yyval].para_offset.push_back(sta[yyvsp[0]].offset);
		sta[yyval].para_offset_of_arr.push_back(sta[yyvsp[0]].offset_of_arr);
		sta[yyval].para_is_const.push_back(sta[yyvsp[0]].is_const);
		sta[yyval].para_is_arr.push_back(sta[yyvsp[0]].is_arr);
	}
#line 2675 "y.tab.c"
    break;

  case 90: /* FuncRParams: FuncRParams ',' Exp  */
#line 954 "./grammar.y"
                             {
		yyval = yyvsp[-2];
		sta[yyval].para_name.push_back(sta[yyvsp[0]].name);
		sta[yyval].para_val.push_back(sta[yyvsp[0]].val);
		sta[yyval].para_offset.push_back(sta[yyvsp[0]].offset);
		sta[yyval].para_offset_of_arr.push_back(sta[yyvsp[0]].offset_of_arr);
		sta[yyval].para_is_const.push_back(sta[yyvsp[0]].is_const);
		sta[yyval].para_is_arr.push_back(sta[yyvsp[0]].is_arr);
	}
#line 2689 "y.tab.c"
    break;

  case 91: /* MulExp: UnaryExp  */
#line 965 "./grammar.y"
                {yyval = yyvsp[0];}
#line 2695 "y.tab.c"
    break;

  case 92: /* MulExp: MulExp '*' UnaryExp  */
#line 966 "./grammar.y"
                             {
		if(sta[yyvsp[-2]].is_const && sta[yyvsp[0]].is_const)
			sta[yyval = ++tot].val = sta[yyvsp[-2]].val * sta[yyvsp[0]].val, sta[yyval].is_const = 1;
		else{
			char tmp[114];
			var2reg(yyvsp[-2], "r8d");
			var2reg(yyvsp[0], "r9d");
			Assemble.push_back("\timull\t%r8d, %r9d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r9d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[yyval = ++tot].offset = offset;
		}
	}
#line 2714 "y.tab.c"
    break;

  case 93: /* MulExp: MulExp '/' UnaryExp  */
#line 980 "./grammar.y"
                             {
		if(sta[yyvsp[-2]].is_const && sta[yyvsp[0]].is_const)
			sta[yyval = ++tot].val = sta[yyvsp[-2]].val / sta[yyvsp[0]].val, sta[yyval].is_const = 1;
		else{
			char tmp[114];
			var2reg(yyvsp[-2], "eax");
			var2reg(yyvsp[0], "r9d");
			Assemble.push_back("\tcltd\n");
			Assemble.push_back("\tidivl\t%r9d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%%eax, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[yyval = ++tot].offset = offset;
		}
	}
#line 2734 "y.tab.c"
    break;

  case 94: /* MulExp: MulExp '%' UnaryExp  */
#line 995 "./grammar.y"
                             {
		if(sta[yyvsp[-2]].is_const && sta[yyvsp[0]].is_const)
			sta[yyval = ++tot].val = sta[yyvsp[-2]].val % sta[yyvsp[0]].val, sta[yyval].is_const = 1;
		else{
			char tmp[114];
			var2reg(yyvsp[-2], "eax");
			var2reg(yyvsp[0], "r9d");
			Assemble.push_back("\tcltd\n");
			Assemble.push_back("\tidivl\t%r9d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%%edx, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[yyval = ++tot].offset = offset;
		}
	}
#line 2754 "y.tab.c"
    break;

  case 95: /* AddExp: MulExp  */
#line 1012 "./grammar.y"
               {yyval = yyvsp[0];}
#line 2760 "y.tab.c"
    break;

  case 96: /* AddExp: AddExp '+' MulExp  */
#line 1013 "./grammar.y"
                           {
		if(sta[yyvsp[-2]].is_const && sta[yyvsp[0]].is_const)
			sta[yyval = ++tot].val = sta[yyvsp[-2]].val + sta[yyvsp[0]].val, sta[yyval].is_const = 1;
		else{
			char tmp[114];
			var2reg(yyvsp[-2], "r8d");
			var2reg(yyvsp[0], "r9d");
			Assemble.push_back("\taddl\t%r9d, %r8d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[yyval = ++tot].offset = offset;
		}
	}
#line 2779 "y.tab.c"
    break;

  case 97: /* AddExp: AddExp '-' MulExp  */
#line 1027 "./grammar.y"
                           {
		if(sta[yyvsp[-2]].is_const && sta[yyvsp[0]].is_const)
			sta[yyval = ++tot].val = sta[yyvsp[-2]].val - sta[yyvsp[0]].val, sta[yyval].is_const = 1;
		else{
			char tmp[114];
			var2reg(yyvsp[-2], "r8d");
			var2reg(yyvsp[0], "r9d");
			Assemble.push_back("\tsubl\t%r9d, %r8d\n");
			offset -= 4;
			Assemble.push_back("\tsubq\t$4, %rsp\n");
			sprintf (tmp, "\tmovl\t%r8d, %d(%rbp)\n", offset); Assemble.push_back(tmp);
			sta[yyval = ++tot].offset = offset;
		}
	}
#line 2798 "y.tab.c"
    break;

  case 98: /* RelExp: AddExp  */
#line 1043 "./grammar.y"
               {yyval = yyvsp[0];}
#line 2804 "y.tab.c"
    break;

  case 99: /* RelExp: RelExp '<' AddExp  */
#line 1044 "./grammar.y"
                           {
		char tmp[114];
		sta[yyval = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg(yyvsp[-2], "r8d");
		var2reg(yyvsp[0], "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjl\t");
		sta[yyval].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjge\t");
		sta[yyval].falselist.push_back(Assemble.size() - 1);
	}
#line 2821 "y.tab.c"
    break;

  case 100: /* RelExp: RelExp '>' AddExp  */
#line 1056 "./grammar.y"
                           {
		char tmp[114];
		sta[yyval = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg(yyvsp[-2], "r8d");
		var2reg(yyvsp[0], "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjg\t");
		sta[yyval].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjle\t");
		sta[yyval].falselist.push_back(Assemble.size() - 1);
	}
#line 2838 "y.tab.c"
    break;

  case 101: /* RelExp: RelExp LEQ AddExp  */
#line 1068 "./grammar.y"
                           {
		char tmp[114];
		sta[yyval = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg(yyvsp[-2], "r8d");
		var2reg(yyvsp[0], "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjle\t");
		sta[yyval].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjg\t");
		sta[yyval].falselist.push_back(Assemble.size() - 1);
	}
#line 2855 "y.tab.c"
    break;

  case 102: /* RelExp: RelExp GEQ AddExp  */
#line 1080 "./grammar.y"
                           {
		char tmp[114];
		sta[yyval = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg(yyvsp[-2], "r8d");
		var2reg(yyvsp[0], "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjge\t");
		sta[yyval].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjl\t");
		sta[yyval].falselist.push_back(Assemble.size() - 1);
	}
#line 2872 "y.tab.c"
    break;

  case 103: /* EqExp: RelExp  */
#line 1094 "./grammar.y"
               {yyval = yyvsp[0];}
#line 2878 "y.tab.c"
    break;

  case 104: /* EqExp: EqExp EQ RelExp  */
#line 1095 "./grammar.y"
                         {
		char tmp[114];
		sta[yyval = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg(yyvsp[-2], "r8d");
		var2reg(yyvsp[0], "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tje\t");
		sta[yyval].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tjne\t");
		sta[yyval].falselist.push_back(Assemble.size() - 1);
	}
#line 2895 "y.tab.c"
    break;

  case 105: /* EqExp: EqExp UEQ RelExp  */
#line 1107 "./grammar.y"
                          {
		char tmp[114];
		sta[yyval = ++tot].quad = ++num;
		sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
		var2reg(yyvsp[-2], "r8d");
		var2reg(yyvsp[0], "r9d");
		Assemble.push_back("\tcmpl\t%r9d, %r8d\n");
		Assemble.push_back("\tjne\t");
		sta[yyval].truelist.push_back(Assemble.size() - 1);
		Assemble.push_back("\tje\t");
		sta[yyval].falselist.push_back(Assemble.size() - 1);
	}
#line 2912 "y.tab.c"
    break;

  case 106: /* LAndExp: EqExp  */
#line 1121 "./grammar.y"
              {
		yyval = yyvsp[0];
		if(!sta[yyval].quad){
			char tmp[114];
			sta[yyval = ++tot].quad = ++num;
			sprintf(tmp, ".L%d:\n", num); Assemble.push_back(tmp);
			var2reg(yyvsp[0], "r8d");
			Assemble.push_back("\tcmpl\t$0, %r8d\n");
			Assemble.push_back("\tjne\t");
			sta[yyval].truelist.push_back(Assemble.size() - 1);
			Assemble.push_back("\tje\t");
			sta[yyval].falselist.push_back(Assemble.size() - 1);
		}
	}
#line 2931 "y.tab.c"
    break;

  case 107: /* LAndExp: LAndExp AND EqExp  */
#line 1135 "./grammar.y"
                           {
		backpatch(sta[yyvsp[-2]].truelist, sta[yyvsp[0]].quad);
		sta[yyval].falselist = merge(sta[yyvsp[-2]].falselist, sta[yyvsp[0]].falselist);
		sta[yyval].truelist = sta[yyvsp[0]].truelist;
		sta[yyval].quad = sta[yyvsp[-2]].quad;
	}
#line 2942 "y.tab.c"
    break;

  case 108: /* LOrExp: LAndExp  */
#line 1143 "./grammar.y"
               { yyval = yyvsp[0]; }
#line 2948 "y.tab.c"
    break;

  case 109: /* LOrExp: LOrExp OR LAndExp  */
#line 1144 "./grammar.y"
                           {
		backpatch(sta[yyvsp[-2]].falselist, sta[yyvsp[0]].quad);
		sta[yyval].truelist = merge(sta[yyvsp[-2]].truelist, sta[yyvsp[0]].truelist);
		sta[yyval].falselist = sta[yyvsp[0]].falselist;
		sta[yyval].quad = sta[yyvsp[-2]].quad;
	}
#line 2959 "y.tab.c"
    break;

  case 110: /* ConstExp: AddExp  */
#line 1152 "./grammar.y"
                {
		yyval = yyvsp[0];
		if(sta[yyval].is_const == false){
			yyerror("Const Error");	
			exit(0);
		}
	}
#line 2971 "y.tab.c"
    break;


#line 2975 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1160 "./grammar.y"


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
