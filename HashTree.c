// Batch Number - 38
// 2014A7PS102P - RISHABH JOSHI
// 2014A7PS248P - DEEP VYAS

#include <stdlib.h>
#include <string.h>
#include "HashTreeDef.h"
#define TABLE_SIZE2 20
#define INTEGEROFF 8	
#define REALOFF 8
#define BOOLEANOFF 8


HashTreeNode* initTree(){
	HashTreeNode *root;
	root= (HashTreeNode*)malloc(1*sizeof(HashTreeNode));
	root->parent=NULL;
	root->tail=0;
	root->curr_offset=0;
	strcpy(root->table_name,"GLOBAL");
	strcpy(root->function_name,"GLOBALF");
	root->table=(HashElement*)malloc(TABLE_SIZE2*sizeof(HashElement));
	int i;
	for(i=0;i<TABLE_SIZE2;i++){
		root->table[i].head=NULL;
	}
	return root;
}

HashTreeNode* addchild2(HashTreeNode *node){
	HashTreeNode *child;
	child= (HashTreeNode*)malloc(1*sizeof(HashTreeNode));
	child->parent=node;
	child->tail=0;
	child->table=(HashElement*)malloc(TABLE_SIZE2*sizeof(HashElement));
	child->curr_offset=0;
	int i;
	for(i=0;i<TABLE_SIZE2;i++){
		child->table[i].head=NULL;
	}
	node->childQ[node->tail]=child;
	node->tail++;
	return child;
}

int hashKey2(char *key){
	int res=0;
	int l=strlen(key);
	int i;
	for(i=l-1;i>=0;i--){
		res+=key[i]-'0';
	}
	return res%TABLE_SIZE2;
}

HashTableNode* find2(char *key,HashTreeNode *node,int res){
	HashElement *table=node->table;
	int hash = hashKey2(key);
	if(table[hash].head==NULL) {
		if(node->parent!=NULL&&res==0) return find2(key,node->parent,0);
		else return NULL;
	}
	else{
		HashTableNode *itr=table[hash].head;
		while(itr!=NULL){
//			if(itr->key==NULL) printf("Gadbad hai\n");
			if(strcmp(key,itr->key)==0){ 
				return itr;
			}
			itr=itr->next;
		}
		if(itr==NULL&&node->parent!=NULL&&res==0){
			return find2(key,node->parent,0);
		}
		else{
			return NULL;
		}
	}
}
int addKey2(ASTNode *ast_node,HashTreeNode *node){
	//printf("%s\n",key);
	if(find2(ast_node->tokenptr->lexeme.iden,node,1)!=NULL){
		return -1;
	}
	HashElement *table = (node->table);
	int hash = hashKey2(ast_node->tokenptr->lexeme.iden);
	// int hash=hashKey2(key);
	HashTableNode *tmp = (HashTableNode*)malloc(1*sizeof(HashTableNode));
	// strcpy(tmp->key,key);
	tmp->next=NULL;
	strcpy(tmp->key,ast_node->tokenptr->lexeme.iden);
	tmp->tokenptr=ast_node->tokenptr;
	tmp->ast_node=ast_node;
	tmp->startscope=ast_node->startscope;
	tmp->endscope=ast_node->endscope;
	tmp->datatype=ast_node->dtype;
    tmp->readOnly = 0;
    tmp->assignedFlag = 0;
	int datatemp = tmp->datatype;
	tmp->offset=node->curr_offset;
	int data_incr;
	if (tmp->datatype == 3)
		datatemp = tmp->ast_node->arrtype;
	if(datatemp==0){
		if(tmp->datatype==3)
			data_incr =  (tmp->ast_node->rrange- tmp->ast_node->lrange+1)*INTEGEROFF;
		else
			data_incr= INTEGEROFF;
	}
	else if(datatemp==1){
		if(tmp->datatype==3)
			data_incr =  (tmp->ast_node->rrange- tmp->ast_node->lrange+1)*REALOFF;
		else
			data_incr= REALOFF;
	}
	else if(datatemp==2){
		if(tmp->datatype==3)
			data_incr =  (tmp->ast_node->rrange- tmp->ast_node->lrange+1)*BOOLEANOFF;
		else
			data_incr= BOOLEANOFF;
	}
	//node->curr_offset+=data_incr;
    HashTreeNode *temptemp = node;
    char glbl[] = "GLOBAL";
    if(temptemp->parent != NULL)
        while(strcmp(temptemp->table_name, glbl)!= 0){
            temptemp->curr_offset += data_incr;
            temptemp = temptemp->parent;
        }
	tmp->next=table[hash].head;
	table[hash].head=tmp;
	return 0;
	// printf("%s\n", table[hash].head->key);
}
void add_plist(HashTableNode *ele,ASTNode *node){
	if(node->gnode.non_term!=MODULENT)
	{
		//printf("Error in adding i/o lists.\n");
	}
	int i=0;
	ASTNode *iplist=node->child->child;
	ASTNode *oplist=node->child->sibling->child;
	while(iplist!=NULL){
		ele->input_plist[i++]=iplist;
		iplist=iplist->sibling;
	}
	int j;
	for(j=i;j<NOC;j++){
		ele->input_plist[j]=NULL;
	}
	i=0;
	while(oplist!=NULL){
		ele->output_plist[i++]=oplist;
		oplist=oplist->sibling;
	}
	for(j=i;j<NOC;j++){
		ele->output_plist[j]=NULL;
	}
}

void printHashNode(HashTableNode *hnode,char *fname,int level){
	char type[20];
	int width; 	
	char scope[20];
	if(hnode->datatype==0){
		strcpy(type,"integer");
		width= INTEGEROFF;
	}
	else if(hnode->datatype==1){
		strcpy(type,"real");
		width=REALOFF;
	}
	else if(hnode->datatype==2){
		strcpy(type,"boolean");
		width=BOOLEANOFF;		
	}
	else if (hnode->datatype==3){
		char type2[20];
		int size=(hnode->ast_node->rrange-hnode->ast_node->lrange+1);
		if(hnode->ast_node->arrtype==0){
			strcpy(type2,"integer");
			width= INTEGEROFF*(size);
		}
		else if(hnode->ast_node->arrtype==1){
			strcpy(type2,"real");
			width= REALOFF*(size);
		}
		else if(hnode->ast_node->arrtype==2){
			strcpy(type2,"boolean");
			width=BOOLEANOFF*(size);		
		}	
		sprintf(type,"array(%d,%s)",size,type2);
	}
	sprintf(scope,"%d to %d",hnode->startscope,hnode->endscope);
	printf("%-20s %-20s %-20s %-20s %-20d %-20d %-20d\n",hnode->key,type,fname,scope,level,width,hnode->offset);
}

void printHashTree(HashTreeNode *htroot,int level){
    char msg1[]="IdenLexeme";
    char msg2[]="Type";
    char msg3[]="Module Name";
    char msg4[]="Line Number";
    char msg5[]="Scope Number";
    char msg6[]="Width";
    char msg7[]="Offset";
    if (level == 0)
	    printf("%-20s %-20s %-20s %-20s %-20s %-20s %-20s\n",msg1, msg2,
            msg3, msg4, msg5, msg6, msg7);
	if(htroot==NULL) return;
	HashElement *table=htroot->table;
	int i;
	HashTableNode *it;
	for(i=0;i<TABLE_SIZE2;i++){
		if(table[i].head!=NULL){
			it=table[i].head;
			while(it!=NULL){
				printHashNode(it,htroot->function_name,level);
				it=it->next;
			}
		}
	}
	for(i=0;i<htroot->tail;i++){
		printHashTree(htroot->childQ[i],level+1);
	}
}
