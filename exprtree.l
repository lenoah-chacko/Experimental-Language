%{
	#include <stdlib.h>
	#include <stdio.h>
	#include<string.h>
	#include "y.tab.h"
	#include "exptree.h"

	int number;
	char *name;
	void yyerror(char const *);

%}

%%
"return"	{return RETURN;}
"FUN"	{return START;}
[0-9]+	{number = atoi(yytext); yylval.no = makeLeafNode(number,1);return NUM;}
"read"	{return READ;}
"write"	{return WRITE;}
"if"	{return IF;}
"{"	{return THEN;}
"}"	{return THENOVER;}
"do"	{return DO;}
"while"	{return WHILE;}
"else"	{return ELSE;}
"<"		{return LT;}
">"		{return GT;}
"=="	{return EQCHECK;}
"<="	{return LE;}
">="	{return GE;}
"!="	{return NE;}
[a-zA-Z][a-zA-Z0-9]* {name=(char*)malloc(yyleng) ;strcpy(name,strdup(yytext)); yylval.no = makeLeafNodeID(1,name); return ID;}
"+"	{return PLUS;}
"-"	{return MINUS;}
"*"	{return MUL;}
"/"	{return DIV;}
"="	{return ASS;}
";" {return ENDLINE;}
[ \t]	{}
[\n\r]	{}
[()]	{return *yytext;}
.	{yyerror("unknown character\n");exit(1);}

%%

int yywrap(void) {
	return 0;
}
