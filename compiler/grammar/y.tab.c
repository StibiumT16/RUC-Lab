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
#line 1 "grammar.y"

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



#line 88 "y.tab.c"

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
    LEQ = 268,                     /* LEQ  */
    GEQ = 269,                     /* GEQ  */
    EQ = 270,                      /* EQ  */
    UEQ = 271,                     /* UEQ  */
    AND = 272,                     /* AND  */
    OR = 273,                      /* OR  */
    ID = 274,                      /* ID  */
    NUMBER = 275,                  /* NUMBER  */
    ERROR = 276                    /* ERROR  */
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
#define LEQ 268
#define GEQ 269
#define EQ 270
#define UEQ 271
#define AND 272
#define OR 273
#define ID 274
#define NUMBER 275
#define ERROR 276

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
  YYSYMBOL_LEQ = 13,                       /* LEQ  */
  YYSYMBOL_GEQ = 14,                       /* GEQ  */
  YYSYMBOL_EQ = 15,                        /* EQ  */
  YYSYMBOL_UEQ = 16,                       /* UEQ  */
  YYSYMBOL_AND = 17,                       /* AND  */
  YYSYMBOL_OR = 18,                        /* OR  */
  YYSYMBOL_ID = 19,                        /* ID  */
  YYSYMBOL_NUMBER = 20,                    /* NUMBER  */
  YYSYMBOL_ERROR = 21,                     /* ERROR  */
  YYSYMBOL_22_ = 22,                       /* '>'  */
  YYSYMBOL_23_ = 23,                       /* '<'  */
  YYSYMBOL_24_ = 24,                       /* '+'  */
  YYSYMBOL_25_ = 25,                       /* '-'  */
  YYSYMBOL_26_ = 26,                       /* '*'  */
  YYSYMBOL_27_ = 27,                       /* '/'  */
  YYSYMBOL_28_ = 28,                       /* '%'  */
  YYSYMBOL_29_ = 29,                       /* ','  */
  YYSYMBOL_30_ = 30,                       /* ';'  */
  YYSYMBOL_31_ = 31,                       /* '['  */
  YYSYMBOL_32_ = 32,                       /* ']'  */
  YYSYMBOL_33_ = 33,                       /* '='  */
  YYSYMBOL_34_ = 34,                       /* '{'  */
  YYSYMBOL_35_ = 35,                       /* '}'  */
  YYSYMBOL_36_ = 36,                       /* '('  */
  YYSYMBOL_37_ = 37,                       /* ')'  */
  YYSYMBOL_38_ = 38,                       /* '!'  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_CompUnits = 40,                 /* CompUnits  */
  YYSYMBOL_CompUnit = 41,                  /* CompUnit  */
  YYSYMBOL_Decl = 42,                      /* Decl  */
  YYSYMBOL_ConstDecls = 43,                /* ConstDecls  */
  YYSYMBOL_ConstDecl = 44,                 /* ConstDecl  */
  YYSYMBOL_ConstDefs = 45,                 /* ConstDefs  */
  YYSYMBOL_ConstDef = 46,                  /* ConstDef  */
  YYSYMBOL_ConstInitVals = 47,             /* ConstInitVals  */
  YYSYMBOL_ConstInitVal = 48,              /* ConstInitVal  */
  YYSYMBOL_VarDecls = 49,                  /* VarDecls  */
  YYSYMBOL_VarDecl = 50,                   /* VarDecl  */
  YYSYMBOL_VarDefs = 51,                   /* VarDefs  */
  YYSYMBOL_VarDef = 52,                    /* VarDef  */
  YYSYMBOL_InitVals = 53,                  /* InitVals  */
  YYSYMBOL_InitVal = 54,                   /* InitVal  */
  YYSYMBOL_FuncDef = 55,                   /* FuncDef  */
  YYSYMBOL_FuncFParams = 56,               /* FuncFParams  */
  YYSYMBOL_FuncFParam = 57,                /* FuncFParam  */
  YYSYMBOL_Blocks = 58,                    /* Blocks  */
  YYSYMBOL_Block = 59,                     /* Block  */
  YYSYMBOL_BlockItem = 60,                 /* BlockItem  */
  YYSYMBOL_Stmt = 61,                      /* Stmt  */
  YYSYMBOL_Exp = 62,                       /* Exp  */
  YYSYMBOL_Cond = 63,                      /* Cond  */
  YYSYMBOL_LVals = 64,                     /* LVals  */
  YYSYMBOL_LVal = 65,                      /* LVal  */
  YYSYMBOL_PrimaryExp = 66,                /* PrimaryExp  */
  YYSYMBOL_UnaryExp = 67,                  /* UnaryExp  */
  YYSYMBOL_FuncRParams = 68,               /* FuncRParams  */
  YYSYMBOL_MulExp = 69,                    /* MulExp  */
  YYSYMBOL_AddExp = 70,                    /* AddExp  */
  YYSYMBOL_RelExp = 71,                    /* RelExp  */
  YYSYMBOL_EqExp = 72,                     /* EqExp  */
  YYSYMBOL_LAndExp = 73,                   /* LAndExp  */
  YYSYMBOL_LOrExp = 74,                    /* LOrExp  */
  YYSYMBOL_ConstExp = 75                   /* ConstExp  */
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
#define YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   273

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  103
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  197

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   276


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
       2,     2,     2,    38,     2,     2,     2,    28,     2,     2,
      36,    37,    26,    24,    29,    25,     2,    27,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    30,
      23,    33,    22,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    31,     2,    32,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,    35,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    47,    47,    53,    62,    68,    76,    82,    90,    96,
     106,   115,   125,   133,   144,   152,   163,   169,   179,   185,
     192,   202,   208,   218,   226,   236,   244,   255,   261,   269,
     276,   287,   293,   303,   309,   316,   326,   336,   346,   357,
     368,   376,   386,   392,   402,   409,   418,   430,   436,   445,
     452,   460,   470,   476,   484,   493,   500,   506,   512,   522,
     534,   544,   551,   558,   566,   573,   582,   590,   596,   604,
     612,   623,   629,   638,   646,   652,   660,   666,   674,   683,
     690,   697,   706,   712,   722,   728,   736,   744,   754,   760,
     768,   778,   784,   792,   800,   808,   818,   824,   832,   842,
     848,   858,   864,   874
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
  "VOID", "LEQ", "GEQ", "EQ", "UEQ", "AND", "OR", "ID", "NUMBER", "ERROR",
  "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "'%'", "','", "';'", "'['",
  "']'", "'='", "'{'", "'}'", "'('", "')'", "'!'", "$accept", "CompUnits",
  "CompUnit", "Decl", "ConstDecls", "ConstDecl", "ConstDefs", "ConstDef",
  "ConstInitVals", "ConstInitVal", "VarDecls", "VarDecl", "VarDefs",
  "VarDef", "InitVals", "InitVal", "FuncDef", "FuncFParams", "FuncFParam",
  "Blocks", "Block", "BlockItem", "Stmt", "Exp", "Cond", "LVals", "LVal",
  "PrimaryExp", "UnaryExp", "FuncRParams", "MulExp", "AddExp", "RelExp",
  "EqExp", "LAndExp", "LOrExp", "ConstExp", YY_NULLPTR
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
     275,   276,    62,    60,    43,    45,    42,    47,    37,    44,
      59,    91,    93,    61,   123,   125,    40,    41,    33
};
#endif

#define YYPACT_NINF (-152)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      49,    47,    11,    16,    45,  -152,  -152,  -152,  -152,  -152,
       7,    27,   112,   105,   114,  -152,    43,    38,  -152,  -152,
    -152,    -8,   123,  -152,  -152,    91,  -152,   131,   188,     3,
      81,    69,  -152,  -152,    10,   131,   208,   146,    27,  -152,
      73,    86,   111,    74,   127,   211,    17,   109,  -152,   131,
     131,  -152,  -152,   131,   131,  -152,   165,  -152,  -152,  -152,
     153,   154,  -152,  -152,   104,   157,  -152,   157,   166,    71,
    -152,  -152,   183,    43,    12,  -152,   131,   188,   147,  -152,
      43,    41,   174,    99,  -152,  -152,   131,   208,  -152,  -152,
    -152,    14,    14,  -152,  -152,  -152,   184,   190,   131,   231,
     180,  -152,  -152,   192,  -152,   195,  -152,  -152,  -152,   131,
     131,   131,   131,   131,   131,  -152,  -152,    -3,  -152,   187,
    -152,   223,    43,   207,  -152,  -152,    43,  -152,  -152,    58,
    -152,   213,  -152,  -152,   203,   157,    40,   181,   226,   230,
     215,  -152,   221,  -152,  -152,    52,   131,  -152,   224,  -152,
    -152,  -152,   104,   104,   188,  -152,   225,  -152,  -152,  -152,
    -152,   208,  -152,  -152,   185,   131,   131,   131,   131,   131,
     131,   131,   131,   185,  -152,   131,  -152,   227,  -152,  -152,
     229,  -152,   254,   157,   157,   157,   157,    40,    40,   181,
     226,  -152,  -152,  -152,   180,   185,  -152
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     4,     6,     7,     5,
       0,     0,     0,    27,     0,    21,     0,     0,     1,     3,
      11,     0,     0,     8,    24,     0,    40,     0,     0,     0,
      29,     0,    23,    41,     0,     0,     0,     0,     0,    10,
       0,     0,     0,     0,     0,     0,     0,    71,    75,     0,
       0,    56,    49,     0,     0,    52,     0,    57,    47,    53,
       0,    74,    76,    84,    88,    66,    74,   103,     0,     0,
      28,    33,     0,     0,     0,    42,     0,     0,    27,    22,
       0,     0,     0,     0,    14,    18,     0,     0,     9,    65,
      51,     0,     0,    62,    61,    64,     0,     0,     0,     0,
      72,    79,    80,     0,    81,     0,    50,    48,    55,     0,
       0,     0,     0,     0,     0,    25,    34,     0,    31,    44,
      37,     0,     0,     0,    30,    36,     0,    12,    19,     0,
      16,     0,    15,    68,     0,    91,    96,    99,   101,    67,
       0,    63,     0,    77,    82,     0,     0,    73,     0,    85,
      86,    87,    89,    90,     0,    35,     0,    43,    39,    26,
      38,     0,    20,    13,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,     0,    78,     0,    54,    32,
      45,    17,    58,    94,    95,    93,    92,    97,    98,   100,
     102,    60,    83,    70,    46,     0,    59
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -152,  -152,   257,   -14,  -152,  -152,  -152,   228,  -152,   -79,
    -152,  -152,  -152,   232,  -152,   -68,  -152,   236,   141,  -152,
      -9,   209,  -151,   -26,   172,    92,   -25,  -152,   -44,  -152,
     103,    -7,    68,   100,   101,  -152,   -11
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     4,     5,     6,    22,     7,    37,    23,   129,    84,
      14,     8,    30,    15,   117,    70,     9,    74,    75,    56,
      57,    58,    59,    60,   134,   100,    66,    62,    63,   145,
      64,    65,   136,   137,   138,   139,    85
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      61,   118,    71,    26,   130,   101,   102,    33,   132,   124,
     104,    55,    12,   182,    72,   133,    68,    16,    97,    96,
      67,    72,   191,    35,    82,    36,   154,   103,    67,    67,
      13,    61,   155,    47,    48,    17,    78,    20,    49,    50,
      73,   121,    55,    71,   196,    18,    21,    80,    10,   122,
      53,    71,    54,   165,   166,     1,     2,     3,    11,     1,
       2,     3,   167,   168,   120,   123,   149,   150,   151,    67,
     121,   125,   142,   144,    34,   131,    67,    25,   126,    67,
      67,   175,   181,   148,   135,   135,   179,   161,    78,   176,
      47,    48,    40,   162,    41,    49,    50,    42,    43,    44,
      45,     1,    46,    89,    93,    69,   116,    53,    90,    54,
      47,    48,    76,   158,    77,    49,    50,   160,    47,    48,
     177,    51,    91,    49,    50,    25,    52,    53,    71,    54,
     110,   111,   112,    83,   128,    53,    27,    54,    28,    61,
      98,    29,    24,    31,    32,    99,    25,    92,    61,   192,
      47,    48,    38,    39,    67,    49,    50,    94,   183,   184,
     185,   186,   135,   135,   135,   135,   105,    53,    41,    54,
      61,    42,    43,    44,    45,     1,    46,    86,    27,    87,
      28,   113,   114,   108,    47,    48,   105,   109,    41,    49,
      50,    42,    43,    44,    45,    51,   169,   170,   115,    25,
     106,    53,   119,    54,    47,    48,   127,    47,    48,    49,
      50,   146,    49,    50,   141,    51,   152,   153,   156,    25,
      24,    53,    69,    54,    53,    89,    54,    47,    48,   147,
      47,    48,    49,    50,    72,    49,    50,   187,   188,   159,
     164,    95,    83,   171,    53,   163,    54,    53,   172,    54,
      47,    48,   173,   174,   178,    49,    50,   180,   195,   193,
      98,    19,   157,    79,   140,   107,    88,    53,   143,    54,
      81,   189,   194,   190
};

static const yytype_uint8 yycheck[] =
{
      25,    69,    28,    12,    83,    49,    50,    16,    87,    77,
      54,    25,     1,   164,    11,     1,    27,     1,     1,    45,
      27,    11,   173,    31,    35,    33,    29,    53,    35,    36,
      19,    56,    35,    19,    20,    19,    19,    30,    24,    25,
      37,    29,    56,    69,   195,     0,    19,    37,     1,    37,
      36,    77,    38,    13,    14,    10,    11,    12,    11,    10,
      11,    12,    22,    23,    73,    76,   110,   111,   112,    76,
      29,    80,    98,    99,    36,    86,    83,    34,    37,    86,
      87,    29,   161,   109,    91,    92,   154,    29,    19,    37,
      19,    20,     1,    35,     3,    24,    25,     6,     7,     8,
       9,    10,    11,    30,    30,    34,    35,    36,    35,    38,
      19,    20,    31,   122,    33,    24,    25,   126,    19,    20,
     146,    30,    36,    24,    25,    34,    35,    36,   154,    38,
      26,    27,    28,    34,    35,    36,    31,    38,    33,   164,
      31,    36,    30,    29,    30,    36,    34,    36,   173,   175,
      19,    20,    29,    30,   161,    24,    25,    30,   165,   166,
     167,   168,   169,   170,   171,   172,     1,    36,     3,    38,
     195,     6,     7,     8,     9,    10,    11,    31,    31,    33,
      33,    24,    25,    30,    19,    20,     1,    33,     3,    24,
      25,     6,     7,     8,     9,    30,    15,    16,    32,    34,
      35,    36,    19,    38,    19,    20,    32,    19,    20,    24,
      25,    31,    24,    25,    30,    30,   113,   114,    31,    34,
      30,    36,    34,    38,    36,    30,    38,    19,    20,    37,
      19,    20,    24,    25,    11,    24,    25,   169,   170,    32,
      37,    30,    34,    17,    36,    32,    38,    36,    18,    38,
      19,    20,    37,    32,    30,    24,    25,    32,     4,    32,
      31,     4,   121,    31,    92,    56,    38,    36,    37,    38,
      34,   171,   180,   172
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    10,    11,    12,    40,    41,    42,    44,    50,    55,
       1,    11,     1,    19,    49,    52,     1,    19,     0,    41,
      30,    19,    43,    46,    30,    34,    59,    31,    33,    36,
      51,    29,    30,    59,    36,    31,    33,    45,    29,    30,
       1,     3,     6,     7,     8,     9,    11,    19,    20,    24,
      25,    30,    35,    36,    38,    42,    58,    59,    60,    61,
      62,    65,    66,    67,    69,    70,    65,    70,    75,    34,
      54,    62,    11,    37,    56,    57,    31,    33,    19,    52,
      37,    56,    75,    34,    48,    75,    31,    33,    46,    30,
      35,    36,    36,    30,    30,    30,    62,     1,    31,    36,
      64,    67,    67,    62,    67,     1,    35,    60,    30,    33,
      26,    27,    28,    24,    25,    32,    35,    53,    54,    19,
      59,    29,    37,    75,    54,    59,    37,    32,    35,    47,
      48,    75,    48,     1,    63,    70,    71,    72,    73,    74,
      63,    30,    62,    37,    62,    68,    31,    37,    62,    67,
      67,    67,    69,    69,    29,    35,    31,    57,    59,    32,
      59,    29,    35,    32,    37,    13,    14,    22,    23,    15,
      16,    17,    18,    37,    32,    29,    37,    62,    30,    54,
      32,    48,    61,    70,    70,    70,    70,    71,    71,    72,
      73,    61,    62,    32,    64,     4,    61
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    40,    40,    41,    41,    42,    42,    43,    43,
      44,    44,    45,    45,    46,    46,    47,    47,    48,    48,
      48,    49,    49,    50,    50,    51,    51,    52,    52,    52,
      52,    53,    53,    54,    54,    54,    55,    55,    55,    55,
      55,    55,    56,    56,    57,    57,    57,    58,    58,    59,
      59,    59,    60,    60,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    62,    63,    63,    64,
      64,    65,    65,    66,    66,    66,    67,    67,    67,    67,
      67,    67,    68,    68,    69,    69,    69,    69,    70,    70,
      70,    71,    71,    71,    71,    71,    72,    72,    72,    73,
      73,    74,    74,    75
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     3,
       4,     3,     3,     4,     3,     4,     1,     3,     1,     2,
       3,     1,     3,     3,     3,     3,     4,     1,     3,     2,
       4,     1,     3,     1,     2,     3,     5,     5,     6,     6,
       3,     3,     1,     3,     2,     4,     5,     1,     2,     2,
       3,     3,     1,     1,     4,     2,     1,     1,     5,     7,
       5,     2,     2,     3,     2,     2,     1,     1,     1,     3,
       4,     1,     2,     3,     1,     1,     1,     3,     4,     2,
       2,     2,     1,     3,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     1
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
#line 47 "grammar.y"
                   {
		fprintf(f2, "CompUnits -> CompUnit\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "CompUnits");
	}
#line 1389 "y.tab.c"
    break;

  case 3: /* CompUnits: CompUnits CompUnit  */
#line 53 "grammar.y"
                            {
		fprintf(f2, "CompUnits -> CompUnits CompUnit\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "CompUnits");
	}
#line 1401 "y.tab.c"
    break;

  case 4: /* CompUnit: Decl  */
#line 62 "grammar.y"
              {
		fprintf(f2, "CompUnit -> Decl\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "CompUnit");
	}
#line 1412 "y.tab.c"
    break;

  case 5: /* CompUnit: FuncDef  */
#line 68 "grammar.y"
                 {
		fprintf(f2, "CompUnit -> FuncDef\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "CompUnit");
	}
#line 1423 "y.tab.c"
    break;

  case 6: /* Decl: ConstDecl  */
#line 76 "grammar.y"
                 {
		fprintf(f2, "Decl-> ConstDecl\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "Decl");	
	}
#line 1434 "y.tab.c"
    break;

  case 7: /* Decl: VarDecl  */
#line 82 "grammar.y"
                 {
		fprintf(f2, "Decl-> VarDecl\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "Decl");	
	}
#line 1445 "y.tab.c"
    break;

  case 8: /* ConstDecls: ConstDef  */
#line 90 "grammar.y"
                    {
		fprintf(f2, "ConstDecls -> ConstDef\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "ConstDecls");	
	}
#line 1456 "y.tab.c"
    break;

  case 9: /* ConstDecls: ConstDecls ',' ConstDef  */
#line 96 "grammar.y"
                                 {
		fprintf(f2, "ConstDecls -> ConstDecls ,  ConstDef\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "ConstDecls");
	}
#line 1469 "y.tab.c"
    break;

  case 10: /* ConstDecl: CONST INT ConstDecls ';'  */
#line 106 "grammar.y"
                                   {
		fprintf(f2, "ConstDecl -> const int ConstDecls ;\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; node[yyvsp[-3]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "ConstDecl");
	}
#line 1483 "y.tab.c"
    break;

  case 11: /* ConstDecl: CONST error ';'  */
#line 115 "grammar.y"
                         {
		printf("Constant definition error!\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0; strcpy(tag[yyvsp[-1]], "ERROR");
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "ConstDecl");
	}
#line 1496 "y.tab.c"
    break;

  case 12: /* ConstDefs: '[' ConstExp ']'  */
#line 125 "grammar.y"
                           {
		fprintf(f2, "ConstDefs -> [ ConstExp ]\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1];
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "ConstDefs");
	}
#line 1509 "y.tab.c"
    break;

  case 13: /* ConstDefs: ConstDefs '[' ConstExp ']'  */
#line 133 "grammar.y"
                                    {
		fprintf(f2, "ConstDefs -> ConstDefs [ ConstExp ]\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "ConstDefs");
	}
#line 1523 "y.tab.c"
    break;

  case 14: /* ConstDef: ID '=' ConstInitVal  */
#line 144 "grammar.y"
                             {
		fprintf(f2, "ConstDef -> ID = ConstInitVal\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "ConstDef");
	}
#line 1536 "y.tab.c"
    break;

  case 15: /* ConstDef: ID ConstDefs '=' ConstInitVal  */
#line 152 "grammar.y"
                                       {
		fprintf(f2, "ConstDef -> ID ConstDefs = ConstInitVal\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "ConstDef");
	}
#line 1550 "y.tab.c"
    break;

  case 16: /* ConstInitVals: ConstInitVal  */
#line 163 "grammar.y"
                           {
		fprintf(f2, "ConstInitVals -> ConstInitVal\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "ConstInitVals");	
	}
#line 1561 "y.tab.c"
    break;

  case 17: /* ConstInitVals: ConstInitVals ',' ConstInitVal  */
#line 169 "grammar.y"
                                        {
		fprintf(f2, "ConstInitVals -> ConstInitVals , ConstInitVal\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "ConstInitVals");
	}
#line 1574 "y.tab.c"
    break;

  case 18: /* ConstInitVal: ConstExp  */
#line 179 "grammar.y"
                      {
		fprintf(f2, "ConstInitVal -> ConstExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "ConstInitVal");	
	}
#line 1585 "y.tab.c"
    break;

  case 19: /* ConstInitVal: '{' '}'  */
#line 185 "grammar.y"
                 {
		fprintf(f2, "ConstInitVal -> { }\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "ConstInitVal");
	}
#line 1597 "y.tab.c"
    break;

  case 20: /* ConstInitVal: '{' ConstInitVals '}'  */
#line 192 "grammar.y"
                               {
		fprintf(f2, "ConstInitVal -> { ConstInitVals }\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1];
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "ConstInitVal");
	}
#line 1610 "y.tab.c"
    break;

  case 21: /* VarDecls: VarDef  */
#line 202 "grammar.y"
                {
		fprintf(f2, "VarDecls -> VarDef\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "VarDecls");	
	}
#line 1621 "y.tab.c"
    break;

  case 22: /* VarDecls: VarDecls ',' VarDef  */
#line 208 "grammar.y"
                             {
		fprintf(f2, "VarDecls -> VarDecls , VarDef\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "VarDecls");
	}
#line 1634 "y.tab.c"
    break;

  case 23: /* VarDecl: INT VarDecls ';'  */
#line 218 "grammar.y"
                         {
		fprintf(f2, "ValDecl -> int VarDecls ;\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1];
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "ValDecl");
	}
#line 1647 "y.tab.c"
    break;

  case 24: /* VarDecl: INT error ';'  */
#line 226 "grammar.y"
                        {
		printf("Variable definition error!\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0; strcpy(tag[yyvsp[-1]], "ERROR");
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "VarDecl");
	}
#line 1660 "y.tab.c"
    break;

  case 25: /* VarDefs: '[' ConstExp ']'  */
#line 236 "grammar.y"
                         {
		fprintf(f2, "VarDefs -> [ ConstExp ]\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1];
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "VarDefs");
	}
#line 1673 "y.tab.c"
    break;

  case 26: /* VarDefs: VarDefs '[' ConstExp ']'  */
#line 244 "grammar.y"
                                  {
		fprintf(f2, "VarDefs -> VarDefs [ ConstExp ]\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "VarDefs");
	}
#line 1687 "y.tab.c"
    break;

  case 27: /* VarDef: ID  */
#line 255 "grammar.y"
          {
		fprintf(f2, "VarDef -> ID\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "VarDef");
	}
#line 1698 "y.tab.c"
    break;

  case 28: /* VarDef: ID '=' InitVal  */
#line 261 "grammar.y"
                        {
		fprintf(f2, "VarDefs -> ID = InitVal\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "VarDef");
	}
#line 1711 "y.tab.c"
    break;

  case 29: /* VarDef: ID VarDefs  */
#line 269 "grammar.y"
                     {
		fprintf(f2, "VarDefs -> ID VarDefs\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "VarDef");
	}
#line 1723 "y.tab.c"
    break;

  case 30: /* VarDef: ID VarDefs '=' InitVal  */
#line 276 "grammar.y"
                                {
		fprintf(f2, "VarDefs -> ID VarDefs = InitVal\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "VarDef");
	}
#line 1737 "y.tab.c"
    break;

  case 31: /* InitVals: InitVal  */
#line 287 "grammar.y"
                 {
		fprintf(f2, "InitVals -> InitVal\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "InitVals");
	}
#line 1748 "y.tab.c"
    break;

  case 32: /* InitVals: InitVals ',' InitVal  */
#line 293 "grammar.y"
                               {
		fprintf(f2, "InitVals -> InitVals , InitVal\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "InitVals");
	}
#line 1761 "y.tab.c"
    break;

  case 33: /* InitVal: Exp  */
#line 303 "grammar.y"
            {
		fprintf(f2, "InitVal -> Exp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "InitVal");
	}
#line 1772 "y.tab.c"
    break;

  case 34: /* InitVal: '{' '}'  */
#line 309 "grammar.y"
                 {
		fprintf(f2, "InitVal -> { }\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "InitVal");
	}
#line 1784 "y.tab.c"
    break;

  case 35: /* InitVal: '{' InitVals '}'  */
#line 316 "grammar.y"
                          {
		fprintf(f2, "InitVal -> { InitVals }\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "InitVal");
	}
#line 1797 "y.tab.c"
    break;

  case 36: /* FuncDef: VOID ID '(' ')' Block  */
#line 326 "grammar.y"
                              {
		fprintf(f2, "FuncDef -> void ID ( ) Block\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-4]; node[yyvsp[-4]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "FuncDef");		
	}
#line 1812 "y.tab.c"
    break;

  case 37: /* FuncDef: INT ID '(' ')' Block  */
#line 336 "grammar.y"
                              {
		fprintf(f2, "FuncDef -> int ID ( ) Block\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-4]; node[yyvsp[-4]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "FuncDef");		
	}
#line 1827 "y.tab.c"
    break;

  case 38: /* FuncDef: VOID ID '(' FuncFParams ')' Block  */
#line 346 "grammar.y"
                                           {
		fprintf(f2, "FuncDef -> void ID ( FuncFParams ) Block\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-5]; node[yyvsp[-5]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-4]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; node[yyvsp[-3]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "FuncDef");		
	}
#line 1843 "y.tab.c"
    break;

  case 39: /* FuncDef: INT ID '(' FuncFParams ')' Block  */
#line 357 "grammar.y"
                                          {
		fprintf(f2, "FuncDef -> int ID ( FuncFParams ) Block\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-5]; node[yyvsp[-5]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-4]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; node[yyvsp[-3]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "FuncDef");		
	}
#line 1859 "y.tab.c"
    break;

  case 40: /* FuncDef: INT error Block  */
#line 368 "grammar.y"
                         {
		printf("Function parameter error!\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0; strcpy(tag[yyvsp[-1]], "ERROR");
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "FuncDef");	
	}
#line 1872 "y.tab.c"
    break;

  case 41: /* FuncDef: VOID error Block  */
#line 376 "grammar.y"
                          {
		printf("Function parameter error!\n");	
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0; strcpy(tag[yyvsp[-1]], "ERROR");
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "FuncDef");
	}
#line 1885 "y.tab.c"
    break;

  case 42: /* FuncFParams: FuncFParam  */
#line 386 "grammar.y"
                        {
		fprintf(f2, "FuncFParams -> FuncFParam\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "FuncFParams");
	}
#line 1896 "y.tab.c"
    break;

  case 43: /* FuncFParams: FuncFParams ',' FuncFParam  */
#line 392 "grammar.y"
                                    {
		fprintf(f2, "FuncFParams -> FuncFParams , FuncFParam\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];  
		strcpy(tag[yyval], "FuncFParams");
	}
#line 1909 "y.tab.c"
    break;

  case 44: /* FuncFParam: INT ID  */
#line 402 "grammar.y"
                  {
		fprintf(f2, "FuncFParam -> int ID\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "FuncFParam");
	}
#line 1921 "y.tab.c"
    break;

  case 45: /* FuncFParam: INT ID '[' ']'  */
#line 409 "grammar.y"
                        {
		fprintf(f2, "FuncFParam -> int ID [ ]\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; node[yyvsp[-3]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "FuncFParam");
	}
#line 1935 "y.tab.c"
    break;

  case 46: /* FuncFParam: INT ID '[' ']' LVals  */
#line 418 "grammar.y"
                              {
		fprintf(f2, "FuncFParam -> int ID [ ] LVals\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-4]; node[yyvsp[-4]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "FuncFParam");
	}
#line 1950 "y.tab.c"
    break;

  case 47: /* Blocks: BlockItem  */
#line 430 "grammar.y"
                 {
		fprintf(f2, "Blocks -> BlockItem\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "Blocks");
	}
#line 1961 "y.tab.c"
    break;

  case 48: /* Blocks: Blocks BlockItem  */
#line 436 "grammar.y"
                          {
		fprintf(f2, "Blocks -> Blocks BlockItem\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "Blocks");
	}
#line 1973 "y.tab.c"
    break;

  case 49: /* Block: '{' '}'  */
#line 445 "grammar.y"
               {
		fprintf(f2, "Block -> { }\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Block");
	}
#line 1985 "y.tab.c"
    break;

  case 50: /* Block: '{' Blocks '}'  */
#line 452 "grammar.y"
                        {
		fprintf(f2, "Block -> { Blocks }\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Block");
	}
#line 1998 "y.tab.c"
    break;

  case 51: /* Block: '{' error '}'  */
#line 460 "grammar.y"
                       {
		printf("BLock error!\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0; strcpy(tag[yyvsp[-1]], "ERROR");
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Block");
	}
#line 2011 "y.tab.c"
    break;

  case 52: /* BlockItem: Decl  */
#line 470 "grammar.y"
               {
		fprintf(f2, "BlockItem -> Decl\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "BlockItem");
	}
#line 2022 "y.tab.c"
    break;

  case 53: /* BlockItem: Stmt  */
#line 476 "grammar.y"
              {
		fprintf(f2, "BlockItem -> Stmt\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "BlockItem");
	}
#line 2033 "y.tab.c"
    break;

  case 54: /* Stmt: LVal '=' Exp ';'  */
#line 484 "grammar.y"
                        {
		fprintf(f2, "Stmt -> LVal = Exp ;\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Stmt");
	}
#line 2047 "y.tab.c"
    break;

  case 55: /* Stmt: Exp ';'  */
#line 493 "grammar.y"
                 {
		fprintf(f2, "Stmt -> Exp ;\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Stmt");
	}
#line 2059 "y.tab.c"
    break;

  case 56: /* Stmt: ';'  */
#line 500 "grammar.y"
             {
		fprintf(f2, "Stmt -> ;\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Stmt");
	}
#line 2070 "y.tab.c"
    break;

  case 57: /* Stmt: Block  */
#line 506 "grammar.y"
               {
		fprintf(f2, "Stmt -> Block\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "Stmt");
	}
#line 2081 "y.tab.c"
    break;

  case 58: /* Stmt: IF '(' Cond ')' Stmt  */
#line 512 "grammar.y"
                                                 {
		fprintf(f2, "Stmt -> if ( Cond ) Stmt\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-4]; node[yyvsp[-4]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; node[yyvsp[-3]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];    
		strcpy(tag[yyval], "Stmt");
	}
#line 2096 "y.tab.c"
    break;

  case 59: /* Stmt: IF '(' Cond ')' Stmt ELSE Stmt  */
#line 522 "grammar.y"
                                         {
		fprintf(f2, "Stmt -> if ( Cond ) Stmt else Stmt\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-6]; node[yyvsp[-6]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-5]; node[yyvsp[-5]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-4];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; node[yyvsp[-3]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];    
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "Stmt");
	}
#line 2113 "y.tab.c"
    break;

  case 60: /* Stmt: WHILE '(' Cond ')' Stmt  */
#line 534 "grammar.y"
                                 {
		fprintf(f2, "Stmt -> while ( Cond ) Stmt\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-4]; node[yyvsp[-4]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; node[yyvsp[-3]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];     
		strcpy(tag[yyval], "Stmt");
	}
#line 2128 "y.tab.c"
    break;

  case 61: /* Stmt: BREAK ';'  */
#line 544 "grammar.y"
                   {
		fprintf(f2, "Stmt -> break ;\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Stmt");
	}
#line 2140 "y.tab.c"
    break;

  case 62: /* Stmt: CONTINUE ';'  */
#line 551 "grammar.y"
                      {
		fprintf(f2, "Stmt -> continue ;\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Stmt");
	}
#line 2152 "y.tab.c"
    break;

  case 63: /* Stmt: RETURN Exp ';'  */
#line 558 "grammar.y"
                        {
		fprintf(f2, "Stmt -> return Exp ;\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1];
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Stmt");
	}
#line 2165 "y.tab.c"
    break;

  case 64: /* Stmt: RETURN ';'  */
#line 566 "grammar.y"
                    {
		fprintf(f2, "Stmt -> return ;\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Stmt");
	}
#line 2177 "y.tab.c"
    break;

  case 65: /* Stmt: error ';'  */
#line 573 "grammar.y"
                   {
		printf("Statement error!\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0; strcpy(tag[yyvsp[-1]], "ERROR");
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "Stmt");
	}
#line 2189 "y.tab.c"
    break;

  case 66: /* Exp: AddExp  */
#line 582 "grammar.y"
              {
		fprintf(f2, "Exp -> AddExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "Exp");
	}
#line 2200 "y.tab.c"
    break;

  case 67: /* Cond: LOrExp  */
#line 590 "grammar.y"
              {
		fprintf(f2, "Cond -> LOrExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "Cond");
	}
#line 2211 "y.tab.c"
    break;

  case 68: /* Cond: error  */
#line 596 "grammar.y"
               {
		printf("Condition error!\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0; strcpy(tag[yyvsp[0]], "ERROR");
		strcpy(tag[yyval], "Cond");
	}
#line 2222 "y.tab.c"
    break;

  case 69: /* LVals: '[' Exp ']'  */
#line 604 "grammar.y"
                   {
		fprintf(f2, "LVals -> [ exp ]\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1];
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "LVals");
	}
#line 2235 "y.tab.c"
    break;

  case 70: /* LVals: LVals '[' Exp ']'  */
#line 612 "grammar.y"
                           {
		fprintf(f2, "LVals -> LVals [ exp ]\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1];  
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "LVals");
	}
#line 2249 "y.tab.c"
    break;

  case 71: /* LVal: ID  */
#line 623 "grammar.y"
          {
		fprintf(f2, "LVal -> ID\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "LVal");
	}
#line 2260 "y.tab.c"
    break;

  case 72: /* LVal: ID LVals  */
#line 629 "grammar.y"
                  {
		fprintf(f2, "LVal -> ID LVals\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; 
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "LVal");
	}
#line 2272 "y.tab.c"
    break;

  case 73: /* PrimaryExp: '(' Exp ')'  */
#line 638 "grammar.y"
                       {
		fprintf(f2, "PrimaryExp -> ( Exp )\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1];
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "PrimaryExp");
	}
#line 2285 "y.tab.c"
    break;

  case 74: /* PrimaryExp: LVal  */
#line 646 "grammar.y"
              {
		fprintf(f2, "PrimaryExp -> LVal\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "PrimaryExp");
	}
#line 2296 "y.tab.c"
    break;

  case 75: /* PrimaryExp: NUMBER  */
#line 652 "grammar.y"
                {
		fprintf(f2, "PrimaryExp -> Number\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "PrimaryExp");
	}
#line 2307 "y.tab.c"
    break;

  case 76: /* UnaryExp: PrimaryExp  */
#line 660 "grammar.y"
                    {
		fprintf(f2, "UnaryExp -> PrimaryExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; 
		strcpy(tag[yyval], "UnaryExp");
	}
#line 2318 "y.tab.c"
    break;

  case 77: /* UnaryExp: ID '(' ')'  */
#line 666 "grammar.y"
                    {
		fprintf(f2, "UnaryExp -> ID ( )\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "UnaryExp");
	}
#line 2331 "y.tab.c"
    break;

  case 78: /* UnaryExp: ID '(' FuncRParams ')'  */
#line 674 "grammar.y"
                                {
		fprintf(f2, "UnaryExp -> ID ( FuncRParams )\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-3];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2]; node[yyvsp[-2]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1];  
		node[yyval].ch[node[yyval].siz++] = yyvsp[0]; node[yyvsp[0]].siz = 0;
		strcpy(tag[yyval], "UnaryExp");
	}
#line 2345 "y.tab.c"
    break;

  case 79: /* UnaryExp: '+' UnaryExp  */
#line 683 "grammar.y"
                      {
		fprintf(f2, "UnaryExp -> + UnaryExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];   
		strcpy(tag[yyval], "UnaryExp");
	}
#line 2357 "y.tab.c"
    break;

  case 80: /* UnaryExp: '-' UnaryExp  */
#line 690 "grammar.y"
                      {
		fprintf(f2, "UnaryExp -> - UnaryExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];   
		strcpy(tag[yyval], "UnaryExp");
	}
#line 2369 "y.tab.c"
    break;

  case 81: /* UnaryExp: '!' UnaryExp  */
#line 697 "grammar.y"
                      {
		fprintf(f2, "UnaryExp -> ! UnaryExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];   
		strcpy(tag[yyval], "UnaryExp");
	}
#line 2381 "y.tab.c"
    break;

  case 82: /* FuncRParams: Exp  */
#line 706 "grammar.y"
                {
		fprintf(f2, "FuncRParams -> Exp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "FuncRParams");
	}
#line 2392 "y.tab.c"
    break;

  case 83: /* FuncRParams: FuncRParams ',' Exp  */
#line 712 "grammar.y"
                             {
		fprintf(f2, "FuncRParams -> FuncRParams , Exp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "FuncRParams");
	}
#line 2405 "y.tab.c"
    break;

  case 84: /* MulExp: UnaryExp  */
#line 722 "grammar.y"
                {
		fprintf(f2, "MulExp -> UnaryExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "MulExp");
	}
#line 2416 "y.tab.c"
    break;

  case 85: /* MulExp: MulExp '*' UnaryExp  */
#line 728 "grammar.y"
                             {
		fprintf(f2, "MulExp -> MulExp * UnaryExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "MulExp");
	}
#line 2429 "y.tab.c"
    break;

  case 86: /* MulExp: MulExp '/' UnaryExp  */
#line 736 "grammar.y"
                             {
		fprintf(f2, "MulExp -> MulExp / UnaryExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "MulExp");
	}
#line 2442 "y.tab.c"
    break;

  case 87: /* MulExp: MulExp '%' UnaryExp  */
#line 744 "grammar.y"
                             {
		fprintf(f2, "MulExp -> MulExp % UnaryExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "MulExp");
	}
#line 2455 "y.tab.c"
    break;

  case 88: /* AddExp: MulExp  */
#line 754 "grammar.y"
               {
		fprintf(f2, "AddExp -> MulExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "AddExp");
	}
#line 2466 "y.tab.c"
    break;

  case 89: /* AddExp: AddExp '+' MulExp  */
#line 760 "grammar.y"
                           {
		fprintf(f2, "AddExp -> AddExp + MulExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "AddExp");
	}
#line 2479 "y.tab.c"
    break;

  case 90: /* AddExp: AddExp '-' MulExp  */
#line 768 "grammar.y"
                           {
		fprintf(f2, "AddExp -> AddExp - MulExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "AddExp");
	}
#line 2492 "y.tab.c"
    break;

  case 91: /* RelExp: AddExp  */
#line 778 "grammar.y"
               {
		fprintf(f2, "RelExp -> AddExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "RelExp");
	}
#line 2503 "y.tab.c"
    break;

  case 92: /* RelExp: RelExp '<' AddExp  */
#line 784 "grammar.y"
                           {
		fprintf(f2, "RelExp -> RelExp < AddExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "RelExp");
	}
#line 2516 "y.tab.c"
    break;

  case 93: /* RelExp: RelExp '>' AddExp  */
#line 792 "grammar.y"
                           {
		fprintf(f2, "RelExp -> RelExp > AddExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "RelExp");
	}
#line 2529 "y.tab.c"
    break;

  case 94: /* RelExp: RelExp LEQ AddExp  */
#line 800 "grammar.y"
                           {
		fprintf(f2, "RelExp -> RelExp <= AddExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "RelExp");
	}
#line 2542 "y.tab.c"
    break;

  case 95: /* RelExp: RelExp GEQ AddExp  */
#line 808 "grammar.y"
                           {
		fprintf(f2, "RelExp -> RelExp >= AddExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "RelExp");
	}
#line 2555 "y.tab.c"
    break;

  case 96: /* EqExp: RelExp  */
#line 818 "grammar.y"
              {
		fprintf(f2, "EqExp -> RelExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "EqExp");
	}
#line 2566 "y.tab.c"
    break;

  case 97: /* EqExp: EqExp EQ RelExp  */
#line 824 "grammar.y"
                         {
		fprintf(f2, "EqExp -> EqExp == RelExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "EqExp");
	}
#line 2579 "y.tab.c"
    break;

  case 98: /* EqExp: EqExp UEQ RelExp  */
#line 832 "grammar.y"
                          {
		fprintf(f2, "EqExp -> EqExp != RelExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "EqExp");
	}
#line 2592 "y.tab.c"
    break;

  case 99: /* LAndExp: EqExp  */
#line 842 "grammar.y"
              {
		fprintf(f2, "LAndExp -> EqExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "LAndExp");
	}
#line 2603 "y.tab.c"
    break;

  case 100: /* LAndExp: LAndExp AND EqExp  */
#line 848 "grammar.y"
                           {
		fprintf(f2, "LAndExp -> LAndExp && EqExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "LAndExp");
	}
#line 2616 "y.tab.c"
    break;

  case 101: /* LOrExp: LAndExp  */
#line 858 "grammar.y"
               {
		fprintf(f2, "LOrExp -> LAndExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "LOrExp");
	}
#line 2627 "y.tab.c"
    break;

  case 102: /* LOrExp: LOrExp OR LAndExp  */
#line 864 "grammar.y"
                           {
		fprintf(f2, "LOrExp -> LOrExp || LAndExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[-2];
		node[yyval].ch[node[yyval].siz++] = yyvsp[-1]; node[yyvsp[-1]].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "LOrExp");
	}
#line 2640 "y.tab.c"
    break;

  case 103: /* ConstExp: AddExp  */
#line 874 "grammar.y"
                {
		fprintf(f2, "ConstExp -> AddExp\n");
		node[yyval = ++tot].siz = 0;
		node[yyval].ch[node[yyval].siz++] = yyvsp[0];
		strcpy(tag[yyval], "ConstExp");
	}
#line 2651 "y.tab.c"
    break;


#line 2655 "y.tab.c"

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

#line 882 "grammar.y"


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
	
