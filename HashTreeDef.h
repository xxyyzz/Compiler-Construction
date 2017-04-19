#ifndef _HASHTREEDEF_
#define _HASHTREEDEF_
#include "./Stage1/parserDef.h"
#include "./Stage1/lexerDef.h"
#include "ASTDef.h"
#define NOC 20
struct HashTableNode{
	char key[LEXEME_SIZE];
	char function_name[LEXEME_SIZE];
	tokenInfo* tokenptr;
	ASTNode *ast_node;
	int datatype;
	int startscope;
	int endscope;
	int offset;
	/*These parameters are for use in modules.*/
	ASTNode* input_plist[NOC];
	ASTNode* output_plist[NOC];
	int defined;
	/*End*/
	struct HashTableNode *next;
};

typedef struct HashTableNode HashTableNode;

typedef struct{
	HashTableNode *head;
}HashElement;

struct HashTreeNode{
	char table_name[LEXEME_SIZE];
	char function_name[LEXEME_SIZE];
	HashElement *table;
	struct HashTreeNode* childQ[NOC];
	int tail;
	int curr_offset;
	struct HashTreeNode *parent;
};

typedef struct HashTreeNode HashTreeNode;

HashTreeNode* initTree();
HashTreeNode* addchild2(HashTreeNode *node);
int hashKey2(char *key);
HashTableNode* find2(char *key,HashTreeNode *node,int res);
int addKey2(ASTNode *ast_node,HashTreeNode *node);
void add_plist(HashTableNode *ele,ASTNode *node);
#endif
