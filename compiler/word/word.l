%{
#include <stdio.h>
#include <stdlib.h>

int row = 1;
int col = 1;

%}

KEYWORD		"auto"|"break"|"continue"|"case"|"char"|"const"|"default"|"define"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|"int"|"long"|"main"|"register"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while"
BORDER		[\(\)\{\}\"\'\[\]\;\,\"\']
OPERATOR    	"+"|"++"|"+="|"-"|"--"|"-="|"*"|"*="|"/"|"/="|"%"|"%="|"<"|"<="|">"|">="|"!"|"!="|"="|"=="|"&&"|"||"|"&"|"|"|"^"|"~"
DIGIT    	[0-9]
FRACTION	\.{DIGIT}+
INDEX		[Ee][+-]?{DIGIT}+
DECIMAL		{DIGIT}+{FRACTION}?{INDEX}?
OCTALCONS   	0|0[0-7]+
HEXCONS     	0[xX][0-9a-fA-F]+
NUMBER		{OCTALCONS}|{HEXCONS}|{DECIMAL}
afterNUMBER	{OPERATOR}|{BORDER}|" "|\n|\t|\v|\r|\f
IDENTIFIER  	[_a-zA-Z][_a-zA-Z0-9]*
other		[^ \n\t\v\r\f\(\)\{\}\"\'\[\]\;\,\"\'\+\-\*\/\%\<\>\=\&\|\!\^\~]


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

{KEYWORD}			{printf("%s:\t K, (%d, %d)\n",yytext,row,col); col += yyleng;}
{NUMBER}/{afterNUMBER}		{printf("%s:\t C, (%d, %d)\n",yytext,row,col); col += yyleng;}
{OPERATOR}			{printf("%s:\t O, (%d, %d)\n",yytext,row,col); col += yyleng;} 
{BORDER} 			{printf("%s:\t D, (%d, %d)\n",yytext,row,col); col += yyleng;}
{IDENTIFIER} 			{printf("%s:\t I, (%d, %d)\n",yytext,row,col); col += yyleng;}
\n				{++row; col = 1;}
\t				{col += 8;}
" "				{++col;}

{other}*			{printf("%s:\t T, (%d, %d)\n",yytext,row,col); col += yyleng;}

%%

int yywrap(){
    return 1;
}

void main(int argc, char **argv){
    yylex();
}