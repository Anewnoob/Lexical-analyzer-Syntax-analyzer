typedef struct _ast ast;
typedef struct _ast *past;

struct _ast{
	
	char nodeType[20];
	int ivalue;
	char svalue[20];

	struct _ast* left;
	struct _ast* middle;
	struct _ast* right;
};

past newNumber(int value);
past newID(char* id_name);
past newKey(char* key_name);
past newString(char* string_name);
past newExpr(char* type, past left,past middle,past right);
void showAst(past node, int nest);
