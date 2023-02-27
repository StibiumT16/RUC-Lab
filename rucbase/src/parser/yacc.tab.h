/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_HOME_TYT_DESKTOP_RUCBASE_LAB_SRC_PARSER_YACC_TAB_H_INCLUDED
# define YY_YY_HOME_TYT_DESKTOP_RUCBASE_LAB_SRC_PARSER_YACC_TAB_H_INCLUDED
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
    ORDER = 258,                   /* ORDER  */
    BY = 259,                      /* BY  */
    SHOW = 260,                    /* SHOW  */
    TABLES = 261,                  /* TABLES  */
    CREATE = 262,                  /* CREATE  */
    TABLE = 263,                   /* TABLE  */
    DROP = 264,                    /* DROP  */
    DESC = 265,                    /* DESC  */
    INSERT = 266,                  /* INSERT  */
    INTO = 267,                    /* INTO  */
    VALUES = 268,                  /* VALUES  */
    DELETE = 269,                  /* DELETE  */
    FROM = 270,                    /* FROM  */
    ASC = 271,                     /* ASC  */
    LIMIT = 272,                   /* LIMIT  */
    WHERE = 273,                   /* WHERE  */
    UPDATE = 274,                  /* UPDATE  */
    SET = 275,                     /* SET  */
    SELECT = 276,                  /* SELECT  */
    INT = 277,                     /* INT  */
    CHAR = 278,                    /* CHAR  */
    FLOAT = 279,                   /* FLOAT  */
    INDEX = 280,                   /* INDEX  */
    AND = 281,                     /* AND  */
    JOIN = 282,                    /* JOIN  */
    EXIT = 283,                    /* EXIT  */
    HELP = 284,                    /* HELP  */
    TXN_BEGIN = 285,               /* TXN_BEGIN  */
    TXN_COMMIT = 286,              /* TXN_COMMIT  */
    TXN_ABORT = 287,               /* TXN_ABORT  */
    TXN_ROLLBACK = 288,            /* TXN_ROLLBACK  */
    LEQ = 289,                     /* LEQ  */
    NEQ = 290,                     /* NEQ  */
    GEQ = 291,                     /* GEQ  */
    T_EOF = 292,                   /* T_EOF  */
    IDENTIFIER = 293,              /* IDENTIFIER  */
    VALUE_STRING = 294,            /* VALUE_STRING  */
    VALUE_INT = 295,               /* VALUE_INT  */
    VALUE_FLOAT = 296              /* VALUE_FLOAT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (void);


#endif /* !YY_YY_HOME_TYT_DESKTOP_RUCBASE_LAB_SRC_PARSER_YACC_TAB_H_INCLUDED  */
