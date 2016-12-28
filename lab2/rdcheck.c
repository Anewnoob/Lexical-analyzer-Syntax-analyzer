#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype {
	NUMBER = 258, 
	STR = 259, 
	ID = 260, 
	INT = 261, 
	VOID = 262, 
	WHILE = 263, 
	RETURN = 264, 
	PRINT = 265, 
	SCAN = 266, 
	IF = 267,
	ELSE = 268,
	CMP = 269,
	ASSIGN = 270,
	STRING =271
};

int yylex();
char  yylval[10];
char* yytext;
FILE* yyin;

int tok;
void advance()
{
	tok = yylex();
	printf("tok: %s\n", yytext);
}

int program();
int exter_decaration();
int decl_or_stmt();
int declarator_list();
int intstr_list();
int initializer();
int declarator();
int parameter_list();
int parameter();
int type();
int statement();
int statement_list();
int expression_statement();
int expr();
int cmp_expr();
int add_expr();
int mul_expr();
int primary_expr();
int expr_list();
int id_list();
void exit();

int program()
{
	int b;
	b = external_declaration();
	 while(tok!='\0')
	 {
			b=b*external_declaration();
	}
	return b;
}

int external_declaration()
{
	int b=1;
	b = b*type();
	b = b*declarator();
	b = b*decl_or_stmt();
	return b;
}

int decl_or_stmt()
{
	int b;
	if (tok == '{')
	{
		advance();
		b = statement_list();
		if(tok == '}')
		{
			advance();
			return  1;
		}
		else
			return 0;
	}
	else if (tok == ',')
	{
		advance();
		b = declarator_list();
		if (tok == ';')
		{
			advance();
			return 1;
		}
		else 
			return 0;
	}
	else if (tok == ';')
	{
		advance();
		return 1;
	}
	else 
		return 0;
}

int declarator_list()
{
	int b,c;
	b=declarator();
	while (tok == ',')
	{
		advance();
		c=declarator();
	}
	return b;
}

int intstr_list()
{
	int b,c;
	b = initializer();
	while (tok == ',')
	{
		advance();
		c=initializer();
	}
	return b;
}

int initializer()
{
	if (tok == NUMBER || tok == STRING)
	{
		advance();
		return 1;
	}
	return 0;
}

int declarator()
{
	int b;
	if (tok == ID)
	{
		advance();
		if (tok == '=')
		{
			advance();
			b = expr();
			return b;
		}
		else if (tok == '(')
		{
			advance();
			if (tok == ')')
			{
				advance();
				return 1;
			}
			b = parameter_list();
			if (tok == ')')
			{
				advance();
				return 1;
			}
			return 0;
		}
		else if (tok == '[')
		{
			advance();
			if (tok == ']')
			{
				advance();
				if (tok == '=')
				{
					advance();
					if (tok == '{')
					{
						advance();
						b = intstr_list();
						b = b*(tok == '}');
						advance();
						return b;
					}
					return 0;
				}
				return 1;
			}
			b = expr();
			if (tok == ']')
			{
				advance();
				if (tok == '=')
				{
					advance();
					if (tok == '{')
					{
						advance();
						b = intstr_list();
						b = b*(tok == '}');
						advance();
						return b;
					}
					return 0;
				}
				return 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}


int parameter_list()
{
	int b,c;
	b = parameter();
	while (tok == ',')
	{
		advance();
		c=parameter_list();
		if(c==0)
			return 0;
	}
	return b;
}

int parameter()
{
	if (type())
	{
		if (tok == ID)
		{
			advance();
			return 1;
		}
		return 0;
	}
	return 0;
}

int type()
{
	if (tok == INT || tok == STR || tok == VOID)
	{
		advance();
		return 1;
	}
	return 0;
}

int statement()
{
	if (type())
	{
		if (declarator_list())
		{
			if (tok == ';')
			{
				advance();
				return 1;
			}
			return 0;
		}
		return 0;
	}
	else if (tok == '{')
	{
		advance();
		if (statement_list())
		{
			if (tok == '}')
			{
				advance();
				return 1;
			}
			return 0;
		}
		return 0;
	}
	else if (tok == IF)
	{
		advance();
		if (tok == '(')
		{
			advance();
			if (expr())
			{
				if (tok == ')')
				{
					advance();
					if (statement())
					{
						if (tok == ELSE)
						{
							advance();
							if (statement())
							{
								return 1;
							}
							return 0;
						}
						return 1;
					}
					return 0;
				}
				return 0;
			}
			return 0;
		}
		return 0;
	}
	else if (tok == WHILE)
	{
		advance();
		if (tok == '(')
		{
			advance();
			if (expr())
			{
				if (tok == ')')
				{
					advance();
					return statement();
				}
				return 0;
			}
			return 0;
		}
		return 0;
	}
	else if(tok == RETURN)
	{
		advance();
		if (tok == ';')
		{
			advance();
			return 1;
		}
		else if (expr())
		{
			if (tok == ';')
			{
				advance();
				return 1;
			}
			return 0;
		}
		return 0;
	}
	else if (tok == PRINT)
	{
		advance();
		if (tok == ';')
		{
			advance();
			return 1;
		}
		else if (expr_list())
		{
			if (tok == ';')
			{
				advance();
				return 1;
			}
			return 0;
		}
		return 0;
	}
	else if (tok == SCAN)
	{
		advance();
		if (id_list())
		{
			if (tok == ';')
			{
				advance();
				return 1;
			}
			return 0;
		}
		return 0;
	}
	else if (expression_statement())
	{
		return 1;
	}
	return 0;
}

int statement_list()
{
	int b,c;
	b=statement();
	c=b;
	while (c)
	{
		c=statement();
	}
	return b;
}

int expression_statement()
{
	if (tok == ';')
	{
		advance();
		return 1;
	}
	else if (expr())
	{
		if (tok == ';')
		{
			advance();
			return 1;
		}
		return 0;
	}
	return 0;
}

int expr()
{
	return cmp_expr();
}

int cmp_expr()
{
	int b,c;
	b=add_expr();
	while(tok==CMP)
	{
			advance();
			c=add_expr();
	}
	return b;
}

int add_expr()
{
	int b,c;
	b=mul_expr();
	while(tok=='+'||tok=='-')
	{
			advance();
			c=mul_expr();
	}
	return b;
}

int mul_expr()
{
	int b,c;
	if(tok=='-')
		advance();
	b=primary_expr();
	while(tok=='*'||tok=='/'||tok=='%')
	{
		advance();
		c=primary_expr();
		if(c==0)
		return 0;
	}
	return b;
}



int primary_expr()
{
	int b;
	if (tok == STRING)
	{
		advance();
		return 1;
	}
	else if (tok == NUMBER)
	{
		advance();
		return 1;
	}
	else if (tok == ID)
	{
		advance();
		if (tok == '(')
		{
			advance();
			if (tok == ')')
			{
				advance();
				return 1;
			}
			else if (expr_list())
			{
				if (tok == ')')
				{
					advance();
					return 1;
				}
				return 0;
			}
			return 0;
		}
		else if (tok == ASSIGN)
		{
			advance();
			if(expr())
			{
				return 1;
			}
			return 0;
		}
		else if (tok == '=')
		{
			advance();
			return expr();

		}
		else if (tok == '[')
		{
			advance();
			if (expr())
			{
				if (tok == ']')
				{
					advance();
					if(tok=='=')
					{ 
						advance();
						if (expr())
						{
							return 1;
						}
						return 0;
					}
					return 1;
				}
				return 0;
			}
			return 0;
		}
		return 1;
	}
	else if (tok == '(')
	{
		advance();
		b = expr();
		b = b*(tok == ')');
		return b;
	}
	return 0;
}

int expr_list()
{
	int b,c;
	b=expr();
	while(tok==',')
	{
		advance();
		c=expr();
		if(c==0)
		return 0;
	}
	return b;
}

int id_list()
{
	int b;
	b = (tok == ID);
	if (tok == ',')
	{
		advance();
		b = b*id_list();
		return b;
	}
	return b;
}


int main()
{
	setbuf(stdout, NULL);
	char file_path[20];
	printf("请输入文件路径:");
	scanf("%s", file_path);
	if ((yyin = fopen(file_path, "r")) == NULL)
	{
		printf("文件不存在!");
		system("pause");
		exit(0);
	}
	printf("文件已经打开\n");
	advance();
	 if(program())
		printf("你输入的文件正确!\n");
	else
		printf("你输入的文件不正确\n");
	return 0;
}
