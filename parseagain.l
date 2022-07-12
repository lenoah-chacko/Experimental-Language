%{
	#include<stdio.h>
    #include "labellist.h"
    FILE *F1, *F2;
    int pos=0;
    int flag=0;
    char * buffer;
    int addr;
    node *HEAD;
%}
%%
[L][0-9]+                         {
                                    (flag==1)&&(addr=search(HEAD,strdup(yytext)))&&(fprintf(F2,"%d",addr));
                                  }
[L][0-9]+[:][\n]                   {
                                    if(flag==0)
                                    HEAD=pushNode(HEAD,strndup(yytext,yyleng-1),pos*2-8+2048);
                                  }
[\n\r]                            {
                                    pos++;
                                    //printf("linenumber: %d\n",pos);
                                    if(flag==1)
                                      fprintf(F2,"%c",*yytext);
                                  }
.                                 {
                                    if(flag==1)
                                      fprintf(F2,"%c",*yytext);
                                  }

%%

int main()
{
    
  	F1 = fopen("exptree.xsm","r");
  	F2 = fopen("B190657CS.xsm","w");
    yyin=F1;
  	yylex();
    //traverse(HEAD);
  	return 0;
}

int yywrap()
{
    if(flag==0)
    {
        flag=1;
        pos=0;
        F1=fopen("exptree.xsm","r");
        yyin=F1;
        return 0;
    }
    return 1;
}
