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
	STRING = 271
};


typedef struct _ast ast;
typedef struct _ast *past;
struct _ast {
	char nodeValue[20];
	char nodeType[25];
	past left;
	past middle;
	past right;
	};

extern int yylex();
extern char  yylval[10];
extern char* yytext;
extern FILE* yyin;

int tok;
void advance()
{
	tok = yylex();
	printf("tok: %s\n", yytext);
}

past program();
past external_declaration();
past decl_or_stmt();
past declarator_list();
past intstr_list();
past initializer();
past declarator();
past parameter_list();
past parameter();
past type();
past statement();
past statement_list();
past expression_statement();
past expr();
past cmp_expr();
past add_expr();
past mul_expr();
past primary_expr();
past expr_list();
past id_list();

past newAstNode()
{
	past node = malloc(sizeof(ast));
	if (node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	return node;
}

past new_node(char* name, past l, past m,past r)
{
	past var = newAstNode();
	strcpy(var->nodeType, name);
	var->left = l;
	var->middle = m;
	var->right = r;
	return var;
}

past new_number()
{
	past retu = newAstNode();
	strcpy(retu->nodeType,"NUMBER");
	strcpy(retu->nodeValue, yylval);
	return retu;
}

past new_id()
{
	past retu = newAstNode();
	strcpy(retu->nodeType, "ID");
	strcpy(retu->nodeValue, yylval);
	return retu;
}
past new_string()
{
	past retu = newAstNode();
	strcpy(retu->nodeType, "STRING");
	strcpy(retu->nodeValue, yylval);
	return retu;
}
past new_void()
{
	past retu = newAstNode();
	strcpy(retu->nodeType, "VOID");
	strcpy(retu->nodeValue, "void");
	return retu;
}

past program()
{
	past l,r;
	l = external_declaration();
	while (tok != '\0')
	{
		r = external_declaration();
		l = new_node("program", l, r,NULL);
	}
	return l;
}

past external_declaration()
{
	past retu = newAstNode();
	strcpy(retu->nodeType, "external_declaration");
	retu->left = type();
	retu->middle = declarator();
	retu->right = decl_or_stmt();
	return retu;
}

past decl_or_stmt()
{
	if (tok == '{')
	{
		advance();
		if (tok == '}')
		{
			past retu = newAstNode();
			strcmp(retu->nodeType, "decl_or_stmt");
			return retu;
		}
		past l = statement_list();
		if (tok == '}')
		{
			advance();
			{
				past retu = new_node("decl_or_stmt", l, NULL, NULL);
				return retu;
			}
		}
		else
			return 0;
	}
	else if (tok == ',')
	{
		advance();
		past l = declarator_list();
		if (tok == ';')
		{
			advance();
			past retu = newAstNode();
			strcmp(retu->nodeType, "decl_or_stmt");
			retu->left = l;
			return retu;
		}
		else
			return NULL;
	}
	else if (tok == ';')
	{
		advance();
		past retu = newAstNode();
		strcmp(retu->nodeType, "decl_or_stmt");
		return retu;
	}
	else
		return NULL;
}

past declarator_list()
{
	past l, r;
	l = declarator();
	while (tok == ',')
	{
		advance();
		r = declarator();
		l = new_node("declarator", r, l,NULL);
	}
	return l;
}

past intstr_list()
{
	past l, r;
	l = initializer();
	while (tok == ',')
	{
		advance();
		r = initializer();
		l = new_node("intstr_list", r, l,NULL);
	}
	return l;
}

past initializer()
{
	past l;
	if (tok == NUMBER)
	{
		l=new_number();
		advance();
		past retu = new_node("initializer", l, NULL ,NULL);
		return retu;
	}
	if (tok == STRING)
	{
		l=new_string();
		advance();
		past retu = new_node("initializer", l, NULL ,NULL);
		return retu;
	}
	return NULL;
}

past declarator()
{
	past l, m, r;
	if (tok == ID)
	{
		l = new_id();
		advance();
		if (tok == '=')
		{
			advance();
			m = expr();
			past retu = new_node("declarator", l, m,NULL);
		}
		else if (tok == '(')
		{
			advance();
			if (tok == ')')
			{
				advance();
				past retu = new_node("declarator", l, NULL ,NULL);
				return retu;
			}
			m = parameter_list();
			if (tok == ')')
			{
				advance();
				past retu = new_node("declarator", l, m ,NULL);
				return retu;
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
						m=intstr_list();
						if(tok == '}');
						advance();
						past retu = new_node("declarator", l, m , NULL);
						return retu;
					}
					return NULL;
				}
				past retu = new_node("declarator",l,NULL,NULL);
				return retu;
			}
			m = expr();
			if (tok == ']')
			{
				advance();
				if (tok == '=')
				{
					advance();
					if (tok == '{')
					{
						advance();
						r = intstr_list();
						if(tok == '}');
						advance();
						past retu = new_node("declarator",l,m,r);
						return retu;
					}
					return NULL;
				}
				past retu = newAstNode();
				strcpy(retu->nodeType, "declarator");
				retu->left = l;
				return retu;
			}
			return NULL;
		}
		past retu = newAstNode();
		strcpy(retu->nodeType, "declarator");
		return retu;
	}
	return NULL;
}


past parameter_list()
{
	past l, r;
	l = parameter();
	while (tok == ',')
	{
		advance();
		r = parameter();
		l = new_node("parameter_list", l, r , NULL);
	}
	return l;
}

past parameter()
{
	past l, r;
	if (l=type())
	{
		if (tok == ID)
		{
			r = new_id();
			advance();
			past retu = new_node("parameter", l, r , NULL);
			return retu;
		}
		return NULL;
	}
	return NULL;
}

past type()
{
	if (tok == INT )
	{
		advance();
		past retu = new_string();
		return retu;
	}
	else if(tok==STR)
	{
		advance();
		past retu = new_string();
		return retu;
	}
	else if (tok==VOID)
	{
		advance();
		past retu = new_void();
		return retu;
	}
	return NULL;
}

past statement()
{
	past l, m, r;
	if (l=type())
	{
		if (m=declarator_list())
		{
			if (tok == ';')
			{
				advance();
				past retu=new_node("statement",l,m,NULL);
				return retu;
			}
			return NULL;
		}
		return NULL;
	}
	else if (tok == '{')
	{
		advance();
		if (l = statement_list())
		{
			if (tok == '}')
			{
				advance();
				past retu = newAstNode();
				strcpy(retu->nodeType, "statement");
				retu->left = l;
				return retu;
			}
			return NULL;
		}
		return NULL;
	}
	else if (tok == IF)
	{
		advance();
		if (tok == '(')
		{
			advance();
			if (l = expr())
			{
				if (tok == ')')
				{
					advance();
					if (m = statement())
					{
						if (tok == ELSE)
						{
							advance();
							if (r = statement())
							{
								past retu = new_node("statement", l, m, r);
								return retu;
							}
							return NULL;
						}
						past retu = new_node("statement",l,m,NULL);
					}
					return NULL;
				}
				return NULL;
			}
			return NULL;
		}
		return NULL;
	}
	else if (tok == WHILE)
	{
		advance();
		if (tok == '(')
		{
			advance();
			if (l=expr())
			{
				if (tok == ')')
				{
					advance();
					m = statement();
					past retu = new_node("statement",l,m,NULL);
					return retu;
				}
				return NULL;
			}
			return NULL;
		}
		return NULL;
	}
	else if (tok == RETURN)
	{
		advance();
		if (tok == ';')
		{
			advance();
			past retu = new_node("statement",NULL,NULL,NULL);
			return retu;
		}
		else if ( l = expr())
		{
			if (tok == ';')
			{
				advance();
				past retu = newAstNode();
				strcpy(retu->nodeType,"statement");
				retu->left = l;
				return retu;
			}
			return NULL;
		}
		return NULL;
	}
	else if (tok == PRINT)
	{
		advance();
		if (tok == ';')
		{
			advance();
			past retu = new_node("statement",NULL,NULL,NULL);
			return retu;
		}
		else if (l = expr_list())
		{
			if (tok == ';')
			{
				advance();
				past retu = new_node("statement",l,NULL,NULL);
				return retu;
			}
			return NULL;
		}
		return NULL;
	}
	else if (tok == SCAN)
	{
		advance();
		if (l = id_list())
		{
			if (tok == ';')
			{
				advance();
				past retu = new_node("statement",l,NULL,NULL);
				return retu;
			}
			return NULL;
		}
		return NULL;
	}
	else if (l = expression_statement())
	{
			past retu = new_node("statement",l,NULL,NULL);
			return retu;
	}
	return NULL;
}

past statement_list()
{
	past l, r;
	l = statement();
	r = l;
	while (r)
	{
		r = statement();
		l = new_node("statement", l, r ,NULL);
	}
	return l;
}

past expression_statement()
{
	past l;
	if (tok == ';')
	{
		advance();
		past retu = new_node("expression_statement",NULL,NULL,NULL);
		return retu;
	}
	else if (l = expr())
	{
		if (tok == ';')
		{
			advance();
			past retu = new_node("expression_statement",l,NULL,NULL);
			return retu;
		}
		return NULL;
	}
	return NULL;
}

past expr()
{
	return cmp_expr();
}

past cmp_expr()
{
	past l, r;
	l = add_expr();
	while (tok == CMP)
	{
		advance();
		r = add_expr();
		l = new_node("cmp_expr", l, r , NULL);
	}
	return l;
}

past add_expr()
{
	past l, r;
	l = mul_expr();
	while (tok == '+' || tok == '-')
	{
		advance();
		r = mul_expr();
		l - new_node("add_expr", l, r , NULL);
	}
	return l;
}

past mul_expr()
{
	past l, r;
	if (tok == '-')
		advance();
	l = primary_expr();
	while (tok == '*' || tok == '/' || tok == '%')
	{
		advance();
		r = primary_expr();
		l = new_node("mul_expr", l, r , NULL);
	}
	return l;
}



past primary_expr()
{
	past l,m, r;
	if (tok == STRING)
	{
		past retu = new_string();
		advance();
		return retu;
	}
	else if (tok == NUMBER)
	{
		past retu = new_number();
		advance();
		return retu;
	}
	else if (tok == ID)
	{
		l = new_id();
		advance();
		if (tok == '(')
		{
			advance();
			if (tok == ')')
			{
				advance();
				past retu = newAstNode();
				strcpy(retu->nodeType, "primary_expr");
				retu->left = l;
				return retu;
			}
			else if (r = expr_list())
			{
				if (tok == ')')
				{
					advance();
					past retu = new_node("primary_expr", l, r,NULL);
					return retu;
				}
				return NULL;
			}
			return NULL;
		}
		else if (tok == ASSIGN)
		{
			advance();
			if (r = expr())
			{
				past retu = new_node("primary_expr", l, r,NULL);
				return retu;
			}
			return NULL;
		}
		else if (tok == '=')
		{
			advance();
			r = expr();
			past retu = new_node("primary_expr", l, r,NULL);
			return retu;
		}
		else if (tok == '[')
		{
			advance();
			if (m = expr())
			{
				if (tok == ']')
				{
					advance();
					if (tok == '=')
					{
						advance();
						if (r = expr())
						{
							past retu = newAstNode();
							strcpy(retu->nodeType, "primary_expr");
							retu->left = l;
							retu->middle = m;
							retu->right = r;
							return retu;
						}
						return NULL;
					}
					past retu = new_node("primary_expr", l, m , NULL);
				}
				return NULL;
			}
			return NULL;
		}
		past retu = new_id();
		return retu;
	}
	else if (tok == '(')
	{
		advance();
		l = expr();
		if (tok == ')')
			advance();
		past retu = new_node("primary_expr", l, NULL, NULL);
		return retu;
	}
	return NULL;
}

past expr_list()
{
	past l, r;
	l = expr();
	while (tok == ',')
	{
		advance();
		r = expr();
		l = new_node("expr_list", l, r , NULL);
	}
	return l;
}

past id_list()
{
	past l, r;
	if (tok == ID)
	{
		l = new_id();
		advance();
	}
	while (tok == ',')
	{
		if (tok == ID)
		{
			r = new_id();
			advance();
		}
		l = new_node("id_list", l, r , NULL);
	}
	return l;
}

void showAst(past node, int nest)
{
	if(node == NULL)
		return;

	int i = 0;
	for(i = 0; i < nest; i ++)
		printf("  ");
		printf("%s\n",node->nodeType);
	showAst(node->left, nest+1);
	showAst(node->middle, nest+1);
	showAst(node->right,nest + 1);

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
	past rr = program();
	showAst(rr,0);
	return 0;
}

