%{
#include <stdlib.h>
#include <stdio.h>
#include "bo.tab.h"
#include "ast.h"
%}

%%
\/\*[\w\W]*?.*|\/\/.* { }
"int" {return INT;}
"void" {return VOID;}
"str" {return STR;}
"while" {return WHILE;}
"return" {return RETURN;}
"print" {return PRINT;}
"scan" {return SCAN;}
"if" {return IF;}
"else" {;return ELSE;}
"assign" {return ASSIGN;}
\< {return CMP;}
\> {return CMP;}
"==" {return CMP;}
"+"|"-"|"*"|"/"|"{"|"}"|","|";"|"="|"("|")"|"["|"]" {return yytext[0]; }
\".*\" {strcpy(yylval.cValue,yytext); return STRING;}
[0-9]+ {yylval.iValue=atoi(yytext); return NUMBER;}
\n	{}
[a-zA-Z_][a-zA-Z0-9]* {strcpy(yylval.cValue,yytext);return ID;}
[ \t] {}


%%

int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}
int yywrap()
{
	return 1;
}
