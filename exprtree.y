%{
	#include <stdlib.h>
	#include <stdio.h>
	#include<string.h>
	#include "exptree.h"
	#include "exptree.c"
	int yylex(void);
	void yyerror(char const*);
	int temp;
	FILE *F;
	int temp;
%}


%union{
	struct tnode *no;
	
}
%token PLUS MINUS MUL DIV START END READ WRITE ASS ENDLINE LT GT EQCHECK LE GE NE DO WHILE IF THEN THENOVER ELSE RETURN
%token <no> NUM ID ARGC
%type <no> expr program Slist Stmt InputStmt OutputStmt AsgStmt IfStmt IfElseStmt WhileStmt RetStmt
%left PLUS MINUS
%left MUL DIV
%right ASS
%nonassoc LT GT EQCHECK LE GE NE

%%

program : READ '(' ID ')' ENDLINE ID ASS START '(' ID ')' THEN Slist THENOVER WRITE '(' ID ')' ENDLINE	{
				$$ = $6;
				entry(F);
				//printf("Evaluation begins\n");
				temp=evaluate(F,$13);
				//printf("Answer : %d\n",evaluate(F,$6));
				exit(0);
			}
		| START THEN THENOVER {
								//printf("terminating..");
								exit(0);
							}
		;

Slist : Slist Stmt 		{$$ = makeOperatorNode(0,$1,$2);}
		| Stmt				{$$	= $1;}
		;

Stmt : InputStmt			{$$	= $1;}
		| OutputStmt		{$$	= $1;}
		| AsgStmt			{$$	= $1;}
		| IfStmt			{$$ = $1;}
		| IfElseStmt			{$$ = $1;}
		| WhileStmt			{$$ = $1;}
		| RetStmt			{$$ = $1;}
		;

InputStmt : READ '(' ID ')' ENDLINE		{$$ = makeOperatorNode(1,$3,NULL);}
		;

OutputStmt : WRITE '(' expr ')' ENDLINE		{$$	= makeOperatorNode(2,$3,NULL);}
		;

AsgStmt : ID ASS expr ENDLINE			{//printf("Assignment statement found\n");
										$$	= makeOperatorNode(9,$1,$3);}
		;

IfStmt : IF '(' expr ')' THEN Slist THENOVER			{//printf("If statement found\n");
														$$	= makeOperatorNode(10,$3,$6);}
		;

IfElseStmt : IF '(' expr ')' THEN Slist THENOVER ELSE THEN Slist THENOVER	{//printf("If-Else statement found\n");
																			$$ = makeOperatorNode(11,$3,makeOperatorNode(0,$6,$10));}
		;

WhileStmt : DO THEN Slist THENOVER WHILE '(' expr ')' ENDLINE	{//printf("While statement found\n");
																$$ = makeOperatorNode(18,$7,$3);}
		;

RetStmt : RETURN expr ENDLINE	{//printf("Return statement found\n");
							$$ = makeOperatorNode(20,$2,NULL);}

expr : expr PLUS expr		{$$ = makeOperatorNode(3,$1,$3); //printf("Looks like we found an sum\n");
							}
	 | expr MINUS expr  	{$$ = makeOperatorNode(5,$1,$3);}
	 | expr MUL expr		{$$ = makeOperatorNode(4,$1,$3);}
	 | expr DIV expr		{$$ = makeOperatorNode(6,$1,$3);}
	 | '(' expr ')'			{$$ = $2;}
	 | NUM					{$$ = $1; //printf("Looks like we found an number\n");
	 						}
	 | ID					{$$ = $1; //printf("Looks like we found an identifier\n");
	 						}
	 | expr LT expr		{$$ = makeOperatorNode(12,$1,$3); //printf("Looks like we found LT\n");
	 					}
	 | expr GT expr		{$$ = makeOperatorNode(13,$1,$3); //printf("Looks like we found GT\n");
	 					}
	 | expr LE expr		{$$ = makeOperatorNode(15,$1,$3); //printf("Looks like we found LE\n");
	 					}
	 | expr GE expr		{$$ = makeOperatorNode(16,$1,$3); //printf("Looks like we found GE\n");
	 					}
	 | expr EQCHECK expr		{$$ = makeOperatorNode(14,$1,$3); //printf("Looks like we found EQCHECK\n");
	 							}
	 | expr NE expr		{$$ = makeOperatorNode(17,$1,$3); //printf("Looks like we found NE\n");
	 					}
	 | START '(' expr ')'		{$$ = makeOperatorNode(19,$3,NULL); //printf("Looks like we found a function call\n");
	 							}
	 ;

%%

void yyerror(char const *s)
{
    //printf("yyerror: %s\n",s);
}


int main(void) {
	F = fopen("exptree.xsm", "w");
	yyparse();
	return 0;
}
