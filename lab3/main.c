#include <stdio.h>
#include <string.h>
extern FILE *yyin;
int parse();
int main()
{
	setbuf(stdout, NULL);
	char file_path[20];
	printf("请输入文件路径:");
	scanf("%s",file_path);
	if ((yyin = fopen(file_path, "r")) == NULL)
	{
		printf("文件不存在!");
		system("pause");
		return 0;
	}
	printf("文件已经打开\n");
    yyparse();
    return 0;
}
