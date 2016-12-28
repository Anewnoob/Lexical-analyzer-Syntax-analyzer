#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

void showAst(past node, int nest)
{
	if(node == NULL)
		return;
	int i = 0;
	for(i = 0; i < nest; i ++)
		printf(" ");
	if(strcmp(node->nodeType, "number") == 0)
		printf("%s %d\n", node->nodeType, node->ivalue);
	else if((strcmp(node->nodeType, "id") == 0)||(strcmp(node->nodeType, "key") == 0)||(strcmp(node->nodeType, "string") == 0))
		printf("%s %s\n", node->nodeType, node->svalue);
	else 
		printf("%s \n",node->nodeType);
	showAst(node->left,  nest+1);
	showAst(node->middle,nest+1);
	showAst(node->right, nest+1);
}


past newAstNode()
{
	past node = malloc(sizeof(ast));
	if(node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	return node;
}

past newNumber(int value)
{
	past var = newAstNode();
	strcpy(var->nodeType,"number");
	var->ivalue = value;
	return var;
}

past newID(char* id_name)
{
	past var = newAstNode();
	strcpy(var->nodeType,"id");
	strcpy(var->svalue,id_name);
	return var;
}

past newKey(char* key_name)
{
	past var = newAstNode();
	strcpy(var->nodeType,"key");
	strcpy(var->svalue,key_name);
	return var;
}

past newString(char* string_name)
{
	past var = newAstNode();
	strcpy(var->nodeType,"string");
	strcpy(var->svalue,string_name);
	return var;
}

past newExpr(char* type, past left,past middle,past right)
{
	past var = newAstNode();
	strcpy(var->nodeType,type);
	var->left = left;
	var->middle=middle;
	var->right = right;
	return var;
}
