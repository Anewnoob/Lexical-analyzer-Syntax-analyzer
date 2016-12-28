%{
#include <stdio.h>
#include "ast.h"
int yylex(void);
%}

%union{
	int	iValue;
	char cValue[20];
	struct _ast* Past;
};

%nonassoc ')'
%nonassoc ELSE

%token <iValue> NUMBER
%token <cValue> ID STRING 
%token <cValue> STR INT VOID WHILE RETURN PRINT SCAN IF ELSE CMP ASSIGN
%type  <Past>program	external_declaration function_definition declaration init_declarator_list init_declarator  intstr_list initializer declarator direct_declarator parameter_list parameter type statement compound_statement begin_scope end_scope statement_list expression_statement selection_statement iteration_statement jump_statement print_statement scan_statement expr assign_expr cmp_expr add_expr mul_expr primary_expr expr_list id_list

%%
total:program											{showAst($1,0);}
		;

program: external_declaration   program    				{$$=newExpr("program",$1,$2,NULL);}
		|												{}
		;

external_declaration: function_definition				{$$=newExpr("external_declaration",$1,NULL,NULL);}
	| declaration										{$$=newExpr("external_declaration",$1,NULL,NULL);}
	;	

function_definition: type declarator compound_statement	{$$=newExpr("function_definition",$1,$2,$3);}
	;

declaration: type init_declarator_list ';'				{$$=newExpr("declaration",$1,$2,NULL);}
	;

init_declarator_list: init_declarator				{$$=newExpr("init_declarator_list",$1,NULL,NULL);}
	| init_declarator_list ',' init_declarator		{$$=newExpr("init_declarator_list",$1,$3,NULL);}
	;

init_declarator: declarator						{$$=newExpr("init_declarator",$1,NULL,NULL);}
	| declarator '=' add_expr					{$$=newExpr("init_declarator",$1,$3,NULL);}
	| declarator '=' '{' intstr_list '}'		{$$=newExpr("init_declarator",$1,$4,NULL);}
	;

intstr_list: initializer						{$$=newExpr("intstr_list",$1,NULL,NULL);}
	| intstr_list ',' initializer				{$$=newExpr("intstr_list",$1,$3,NULL);}
	;

initializer: NUMBER								{$$=newNumber($1);}
	| STRING									{$$=newString($1);}
	;

declarator: direct_declarator					{$$=newExpr("declarator",$1,NULL,NULL);}
	;

direct_declarator: ID								{$$=newID($1);}
	| direct_declarator '(' parameter_list ')'		{$$=newExpr("direct_declarator",$1,$3,NULL);}
	| direct_declarator '(' ')'						{$$=newExpr("direct_declarator",$1,NULL,NULL);}
	| ID '[' expr ']'								{$$=newExpr("direct_declarator",newID($1),$3,NULL);}
	| ID '[' ']'									{$$=newExpr("direct_declarator",newID($1),NULL,NULL);}
	;

parameter_list: parameter						{$$=newExpr("parameter_list",$1,NULL,NULL);}
	| parameter_list ',' parameter				{$$=newExpr("parameter_list",$1,$3,NULL);}
	;

parameter: type ID								{$$=newExpr("parameter",$1,newID($2),NULL);}
	;

type: INT										{$$=newKey("int");}
	| STR										{$$=newKey("str");}
	| VOID										{$$=newKey("void");}
	;

statement: compound_statement					{$$=newExpr("statement",$1,NULL,NULL);}
	| expression_statement						{$$=newExpr("statement",$1,NULL,NULL);}
	| selection_statement						{$$=newExpr("statement",$1,NULL,NULL);}
	| iteration_statement						{$$=newExpr("statement",$1,NULL,NULL);}
	| jump_statement							{$$=newExpr("statement",$1,NULL,NULL);}
	| print_statement							{$$=newExpr("statement",$1,NULL,NULL);}
	| scan_statement							{$$=newExpr("statement",$1,NULL,NULL);}
	| declaration								{$$=newExpr("statement",$1,NULL,NULL);}
	;

compound_statement: begin_scope end_scope		{$$=newExpr("compound_statement",$1,$2,NULL);}
	| begin_scope statement_list end_scope		{$$=newExpr("compound_statement",$1,$2,$3);}
	;

begin_scope: '{'								{$$=newExpr("begin_scope",NULL,NULL,NULL);}
	;

end_scope: '}'									{$$=newExpr("end_scope",NULL,NULL,NULL);}
	;
	
statement_list: statement						{$$=newExpr("statement_list",$1,NULL,NULL);}
	| statement_list statement					{$$=newExpr("statement_list",$1,$2,NULL);}
	;

expression_statement: ';'								{$$=newExpr("expression_statement",NULL,NULL,NULL);}
	| expr ';'											{$$=newExpr("expression_statement",$1,NULL,NULL);}
	;

selection_statement: IF '(' expr ')' statement			{$$=newExpr("selection_statement",newKey("if"),$3,$5);}

	| IF '(' expr ')' statement ELSE statement			{$$=newExpr("selection_statement",$3,$5,$7);}
	;

iteration_statement: WHILE '(' expr ')' statement		{$$=newExpr("iteration_statement",newKey("while"),$3,$5);}
	;

jump_statement: RETURN ';'								{$$=newExpr("jump_statement",NULL,NULL,NULL);}
	| RETURN expr ';'									{$$=newExpr("jump_statement",newKey("return"),NULL,NULL);}
	;

print_statement: PRINT ';'						{$$=newExpr("print_statement",NULL,NULL,NULL);}
	| PRINT expr_list ';'						{$$=newExpr("print_statement",newKey("print"),$2,NULL);}
	;

scan_statement: SCAN id_list ';'				{$$=newExpr("scan_statement",newKey("scan"),$2,NULL);}
	;

expr: assign_expr								{$$=newExpr("expr",$1,NULL,NULL);}
	;

assign_expr: cmp_expr							{$$=newExpr("assign_expr",$1,NULL,NULL);}
	| ID ASSIGN assign_expr						{$$=newExpr("assign_expr",newID($1),newKey("assign"),$3);}
	| ID '=' assign_expr						{$$=newExpr("assign_expr",newID($1),$3,NULL);}
	| ID '[' expr ']' '=' assign_expr			{$$=newExpr("assign_expr",newID($1),$3,$6);}

cmp_expr: add_expr								{$$=newExpr("cmp_expr",$1,NULL,NULL);}
	| cmp_expr CMP add_expr						{$$=newExpr("cmp_expr",$1,newKey("cmp"),$3);}			
	;

add_expr: mul_expr								{$$=newExpr("add_expr",$1,NULL,NULL);}
	| add_expr '+' mul_expr						{$$=newExpr("add_expr",$1,$3,NULL);}
	| add_expr '-' mul_expr						{$$=newExpr("add_expr",$1,$3,NULL);}
	;

mul_expr: primary_expr							{$$=newExpr("mul_expr",$1,NULL,NULL);}
	| mul_expr '*' primary_expr					{$$=newExpr("mul_expr",$1,$3,NULL);}
	| mul_expr '/' primary_expr					{$$=newExpr("mul_expr",$1,$3,NULL);}
	| mul_expr '%' primary_expr					{$$=newExpr("mul_expr",$1,$3,NULL);}
	| '-' primary_expr							{$$=newExpr("mul_expr",NULL,$2,NULL);}
	;

primary_expr: ID '(' expr_list ')'				{$$=newExpr("primary_expr",newID($1),$3,NULL);}					
	|ID '(' ')'									{$$=newExpr("primary_expr",newID($1),NULL,NULL);}
	| '(' expr ')'								{$$=newExpr("primary_expr",$2,NULL,NULL);}
	| ID										{$$=newExpr("primary_expr",newID($1),NULL,NULL);}
	| initializer								{$$=newExpr("primary_expr",$1,NULL,NULL);}
	| ID '[' expr ']'							{$$=newExpr("primary_expr",newID($1),$3,NULL);}
	;

expr_list: expr									{$$=newExpr("expr_list",$1,NULL,NULL);}
	| expr_list ',' expr						{$$=newExpr("expr_list",$1,$3,NULL);}
	;

id_list: ID										{$$=newID($1);}
	| id_list ',' ID							{$$=newExpr("id_list",$1,newID($3),NULL);}
	
%%

