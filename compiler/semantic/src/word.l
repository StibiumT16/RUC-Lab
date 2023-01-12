%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

int row = 1;
int col = 1;
extern int tot;
extern char tag[114514][100];
%}

BORDER		[\(\)\{\}\"\'\[\]\;\,]
OPERATOR    	"+"|"-"|"*"|"/"|"%"|"<"|"<="|">"|">="|"!"|"!="|"="|"=="|"&&"|"||"|"&"|"|"|"^"
DECIMAL		(-?)([1-9][0-9]+)|([0-9])
OCTALCONS   	(-?)0[0-7]+
HEXCONS     	(-?)0[xX][0-9a-fA-F]+
NUMBER		{OCTALCONS}|{HEXCONS}|{DECIMAL}
afterNUMBER	{OPERATOR}|{BORDER}|" "|\n|\t|\v|\r|\f
IDENTIFIER  	[_a-zA-Z][_a-zA-Z0-9]*
other		[^ \n\t\v\r\f\(\)\{\}\"\'\[\]\;\,\"\'\+\-\*\/\%\<\>\=\&\|\!\^]


notebegin 	"/*"
noteend 	"*/"
linenotebegin 	"//"
linenoteend 	\n
%x 		NOTE
%x 		LINENOTE


%%

{notebegin} 			{BEGIN NOTE; col += yyleng;}
<NOTE>\n			{++row; col = 1;}
<NOTE>\t			{col += 8;}
<NOTE>. 			{col += yyleng;}
<NOTE>{noteend} 		{BEGIN INITIAL; col += yyleng;}
{linenotebegin} 		{BEGIN LINENOTE;}
<LINENOTE>. 			{}
<LINENOTE>{linenoteend} 	{BEGIN INITIAL; ++row; col=1;}

"if"		{col += yyleng; yylval = ++tot; return IF;}
"else"		{col += yyleng; yylval = ++tot; return ELSE;}
"while"		{col += yyleng; yylval = ++tot; return WHILE;}
"continue"	{col += yyleng; yylval = ++tot; return CONTINUE;}
"break"		{col += yyleng; yylval = ++tot; return BREAK;}
"return" 	{col += yyleng; yylval = ++tot; return RETURN;}
"const"		{col += yyleng; yylval = ++tot; return CONST;}
"int"		{col += yyleng; yylval = ++tot; return INT;}
"void"		{col += yyleng; yylval = ++tot; return VOID;}
"main"		{col += yyleng; yylval = ++tot; return MAIN;}

"+"		{col += yyleng; yylval = ++tot; return '+';}
"-"		{col += yyleng; yylval = ++tot; return '-';}
"*"		{col += yyleng; yylval = ++tot; return '*';}
"/"		{col += yyleng; yylval = ++tot; return '/';}
"%"		{col += yyleng; yylval = ++tot; return '%';}
"<"		{col += yyleng; yylval = ++tot; return '<';}
">"		{col += yyleng; yylval = ++tot; return '>';}
"!"		{col += yyleng; yylval = ++tot; return '!';}
"="		{col += yyleng; yylval = ++tot; return '=';}
"<="		{col += yyleng; yylval = ++tot; return LEQ;}
">="		{col += yyleng; yylval = ++tot; return GEQ;}
"=="		{col += yyleng; yylval = ++tot; return EQ;}
"!="		{col += yyleng; yylval = ++tot; return UEQ;}
"&&"		{col += yyleng; yylval = ++tot; return AND;}
"||"		{col += yyleng; yylval = ++tot; return OR;}

"("		{col += yyleng; yylval = ++tot; return '(';}
")"		{col += yyleng; yylval = ++tot; return ')';}
"{"		{col += yyleng; yylval = ++tot; return '{';}
"}"		{col += yyleng; yylval = ++tot; return '}';}
"["		{col += yyleng; yylval = ++tot; return '[';}
"]"		{col += yyleng; yylval = ++tot; return ']';}
";"		{col += yyleng; yylval = ++tot; return ';';}
","		{col += yyleng; yylval = ++tot; return ',';}


{IDENTIFIER} 	{col += yyleng; yylval = ++tot; strcpy(tag[tot], yytext); return ID;}
{NUMBER}/{afterNUMBER}	{col += yyleng; yylval = atoi(yytext); strcpy(tag[tot], yytext); return NUMBER;}

\n		{++row; col = 1;}
\t		{col += 8;}
" "		{++col;}

{other}*	{col += yyleng; return ERROR;}


%%
