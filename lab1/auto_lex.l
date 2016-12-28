%{
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
%}
digit [0-9]
num {digit}+
space [ \t]+
enter [\n]+
%%
\/\*[\w\W]*?.*|\/\/.* {printf("<注释,%s>\n",yytext);}
\".*\" {printf("<字符串常量,%s>\n",yytext);}
[ \t] {}
[\n] {}
{digit}+ {printf("<整数,%s>\n",yytext);}
{digit}+"."{digit}* {printf("<浮点数,%s>\n",yytext);}
"int"|"else"|"return"|"void"|"if"|"while"|"float"|"else"|"for" {printf("<关键字,%s>\n",yytext);}
"#"|"("|")"|"{"|"}"|","|"="|";"|"+="|"&"|"!="|"%"|"/" {printf("<符号,%s>\n",yytext);}
[a-zA-Z_][a-zA-Z0-9]* {printf("<标识符,%s>\n",yytext);}


%%
int main(void)
{
	char file_path[20];
	printf("请输入文件路径:");
	scanf("%s",&file_path);
	FILE *fpr;
	if ((fpr = fopen(file_path, "r")) == NULL)
	{
		printf("打开文件失败!");
		system("pause");
		exit(0);
	}
    yyin = fopen(file_path,"r");
	printf("分析完成 \n");
    return yylex();
}
int yywrap()
{
	return 1;
}
