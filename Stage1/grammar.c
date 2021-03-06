/*Team : 
 * Rishabh Joshi 2014A7PS102P
 * Deep Vyas 2014A7PS248P
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "parserDef.h"

// Gammar struct
GrammarNode grammar[GRAMMAR_LEN][GRAMMAR_LEN];


// Global parsing table
int parsingTable[noNonTerminals][noTerminals];

// ffset is the first or follow set
FirstAndFollow ffset;

char nt[51][100] = {
    "PROGRAMNT", ///
    "MODULEDECLARATIONS",	"OTHERMODULES",	"DRIVERMODULE",
    "MODULEDECLARATION",	"MODULENT",////
    "MODULEDEF",	"INPUTPLIST",	"RET",	"OUTPUTPLIST",	"N1",
    "N2",	"DATATYPE",	"RANGE",	"TYPE",	"STATEMENTS",	"STATEMENT",
    "IOSTMT",	"VAR",	"WHICHID",	"SIMPLESTMT",	"ASSIGNMENTSTMT",
    "WHICHSTMT",	"LVALUEIDSTMT",	"LVALUEARRSTMT",	"INDEX",
    "MODULEREUSESTMT",	"OPTIONAL",	"IDLIST",	"N3",	"EXPRESSION",
    "ARITHMETICORBOOLEANEXPR",	"ANYTERM",	"N7",	"N8",
    "ARITHMETICEXPR",	"N4",	"TERM",	"N5",	"FACTOR",	"LOGICALOP",
    "RELATIONALOP",	"OP1",	"OP2",	"DECLARESTMT",	"CONDITIONALSTMT",
	"CASESTMTS",	"N9",	"VALUE",	"DEFAULTNT", /////
	"ITERATIVESTMT"};

char ter[57][100] = {
"INTEGER", 
"REAL", 
"BOOLEAN", 
"OF",
"ARRAY",
"START",
"END",
"DECLARE",
"MODULE",
"DRIVER",
"PROGRAM",
"GET_VALUE",
"PRINT",
"USE",
"WITH",
"PARAMETERS",
"TRUE",
"FALSE",
"TAKES",
"INPUT",
"RETURNS",
"AND",
"OR",
"FOR",
"IN",
"SWITCH",
"CASE",
"BREAK",
"DEFAULT",
"WHILE",
"PLUS",
"MINUS",
"MUL",
"DIV",
"LT",
"LE",
"GE",
"GT",
"EQ",
"NE",
"DEF",
"ENDDEF",
"COLON",
"RANGEOP",
"SEMICOL",
"COMMA",
"ASSIGNOP",
"SQBO",
"SQBC",
"BO",
"BC",
"COMMENTMARK",
"ID",
"DRIVERDEF",
"DRIVERENDDEF",
"EPSILON",
"EOF"
};

FirstAndFollow ffset;

#define size_nt 51
#define sz_t 57

void populateFirstSet(){
	FILE *fp = fopen("first.txt", "r");

	char *buffer;
	size_t size = 132;
	buffer = (char *)malloc(size * sizeof(char));

	char *token;
	const char s[2] = " ";
	while(fgets(buffer, size, fp)!= NULL){
		buffer[strlen(buffer)-1] = '\0';
		token = strtok(buffer, s);
		int i = 0;
		if (token == NULL)
			break;
		for (i = 0; i < size_nt; i++){
			if (strcmp(token, nt[i]) == 0)
				break;
		}
        // printf("\nfirst of %s : ", nt[i]);
		int k = 0;
        // i is of index into ffset.first[i][j++].t = nexttokenvalue
		while(token != NULL){   
			token = strtok(NULL, s);
        //    printf("-%s-", token);
        //    fflush(stdout);
			if(token == NULL) break;
			int j = 0;
			for (j = 0; j < sz_t; j++){
				if (strcmp(token, ter[j]) == 0)
					break;
			}
			ffset.first[i][k++].t = j;
            // printf("%s ,", t[j]);
		}
		ffset.first[i][k++].grammarindex = -1;
	}
}
void populateFollowSet(){
FILE *fp = fopen("follow.txt", "r");

char *buffer;
size_t size = 320;
buffer = (char *)malloc(size * sizeof(char));

char *token;
const char s[2] = " ";
while(fgets(buffer,size,fp)!=NULL){
	// printf("%s",buffer);
	buffer[strlen(buffer)-1] = '\0';
	token = strtok(buffer, s);
	int i = 0;
	if(token==NULL) break;
	for (i=0; i < size_nt;i++){
		if(strcmp(token,nt[i])==0) break;
	}
	int k=0;
	// printf("\nFollow of %s : ",nt[i]);
	while(token != NULL){
		token=strtok(NULL, s);
		if(token==NULL) break;
		int j=0;
		for(j=0;j<sz_t;j++){
			if(strcmp(token,ter[j])==0) break;
		}
		if(strcmp(token,"EOF")==0) ffset.follow[i][k++].t=60;
		else ffset.follow[i][k++].t=j;
		// printf("%s ,",ter[j]);
	}
	ffset.follow[i][k++].grammarindex=-1;
	// printf("\n");
	}
}
void createParsingTable(){
	int i=0,j=0;
	int epsilon_flag=0;
	for(i=0;i<noNonTerminals;i++){
		for(j=0;j<maxFirstSet;j++){
			if(ffset.first[i][j].grammarindex!=-1){
				if(ffset.first[i][j].t==EPSILON){
					epsilon_flag=1;
				}
				/*See whether to put or not*/
				parsingTable[i][ffset.first[i][j].t]=ffset.first[i][j].grammarindex;
			}
		}
		if(epsilon_flag==1){
			for(j=0;j<maxFollowSet;j++){
				parsingTable[i][ffset.follow[i][j].t]=ffset.follow[i][j].grammarindex;
			}
		}
	}
}

void populateGrammar(){
	grammar[0][0].ele.non_term = PROGRAMNT;
	grammar[0][0].t = NONTERMINAL;
	grammar[0][0].len = 4;
	grammar[0][1].ele.non_term = MODULEDECLARATIONS;
	grammar[0][1].t = NONTERMINAL;
	grammar[0][2].ele.non_term = OTHERMODULES;
	grammar[0][2].t = NONTERMINAL;
	grammar[0][3].ele.non_term = DRIVERMODULE;
	grammar[0][3].t = NONTERMINAL;
	grammar[0][4].ele.non_term = OTHERMODULES;
	grammar[0][4].t = NONTERMINAL;

	grammar[1][0].ele.non_term = MODULEDECLARATIONS;
	grammar[1][0].t = NONTERMINAL;
	grammar[1][0].len = 2;
	grammar[1][1].ele.non_term = MODULEDECLARATION;
	grammar[1][1].t = NONTERMINAL;
	grammar[1][2].ele.non_term = MODULEDECLARATIONS;
	grammar[1][2].t = NONTERMINAL;

  grammar[2][0].ele.non_term = MODULEDECLARATIONS;
	grammar[2][0].t = NONTERMINAL;
	grammar[2][0].len = 1;
	grammar[2][1].ele.term = EPSILON;
	grammar[2][1].t = TERMINAL;
	
	grammar[3][0].ele.non_term = MODULEDECLARATION;
	grammar[3][0].t = NONTERMINAL;
	grammar[3][0].len = 4;
	grammar[3][1].ele.term = DECLARE;
	grammar[3][1].t = TERMINAL;
	grammar[3][2].ele.term = MODULE;
	grammar[3][2].t = TERMINAL;
	grammar[3][3].ele.term = ID;
	grammar[3][3].t = TERMINAL;
	grammar[3][4].ele.term = SEMICOL;
	grammar[3][4].t = TERMINAL;

	grammar[4][0].ele.non_term = DRIVERMODULE;
	grammar[4][0].t = NONTERMINAL;
	grammar[4][0].len = 5;
	grammar[4][1].ele.term = DRIVERDEF;
	grammar[4][1].t = TERMINAL;
	grammar[4][2].ele.term = DRIVER;
	grammar[4][2].t = TERMINAL;
	grammar[4][3].ele.term = PROGRAM;
	grammar[4][3].t = TERMINAL;
	grammar[4][4].ele.term = DRIVERENDDEF;
	grammar[4][4].t = TERMINAL;
	grammar[4][5].ele.non_term = MODULEDEF;
	grammar[4][5].t = NONTERMINAL;

	grammar[5][0].ele.non_term = MODULENT;
	grammar[5][0].t = NONTERMINAL;
	grammar[5][0].len = 12;
	grammar[5][1].ele.term = DEF;
	grammar[5][1].t = TERMINAL;
	grammar[5][2].ele.term = MODULE;
	grammar[5][2].t = TERMINAL;
	grammar[5][3].ele.term = ID;
	grammar[5][3].t = TERMINAL;
	grammar[5][4].ele.term = ENDDEF;
	grammar[5][4].t = TERMINAL;
	grammar[5][5].ele.term = TAKES;
	grammar[5][5].t = TERMINAL;
	grammar[5][6].ele.term = INPUT;
	grammar[5][6].t = TERMINAL;
	grammar[5][7].ele.term = SQBO;
	grammar[5][7].t = TERMINAL;
	grammar[5][8].ele.non_term = INPUTPLIST;
	grammar[5][8].t = NONTERMINAL;
	grammar[5][9].ele.term = SQBC;
	grammar[5][9].t = TERMINAL;
	grammar[5][10].ele.term = SEMICOL;
	grammar[5][10].t = TERMINAL;
	grammar[5][11].ele.non_term = RET;
	grammar[5][11].t = NONTERMINAL;
	grammar[5][12].ele.non_term = MODULEDEF;
	grammar[5][12].t = NONTERMINAL;

	grammar[6][0].ele.non_term = RET;
	grammar[6][0].t = NONTERMINAL;
	grammar[6][0].len = 5;
	grammar[6][1].ele.term = RETURNS;
	grammar[6][1].t = TERMINAL;
	grammar[6][2].ele.term = SQBO;
	grammar[6][2].t = TERMINAL;
	grammar[6][3].ele.non_term = OUTPUTPLIST;
	grammar[6][3].t = NONTERMINAL;
	grammar[6][4].ele.term = SQBC;
	grammar[6][4].t = TERMINAL;
	grammar[6][5].ele.term = SEMICOL;
	grammar[6][5].t = TERMINAL;

	grammar[7][0].ele.non_term = RET;
	grammar[7][0].t = NONTERMINAL;
	grammar[7][0].len = 1;
	grammar[7][1].ele.term = EPSILON;
	grammar[7][1].t = TERMINAL;
	
	grammar[8][0].ele.non_term = INPUTPLIST;
	grammar[8][0].t = NONTERMINAL;
	grammar[8][0].len = 4;
	grammar[8][1].ele.term = ID;
	grammar[8][1].t = TERMINAL;
	grammar[8][2].ele.term = COLON;
	grammar[8][2].t = TERMINAL;
	grammar[8][3].ele.non_term = DATATYPE;
	grammar[8][3].t = NONTERMINAL;
	grammar[8][4].ele.non_term = N1;
	grammar[8][4].t = NONTERMINAL;

	grammar[9][0].ele.non_term = N1;
	grammar[9][0].t = NONTERMINAL;
	grammar[9][0].len = 5;
	grammar[9][1].ele.term = COMMA;
	grammar[9][1].t = TERMINAL;
	grammar[9][2].ele.term = ID;
	grammar[9][2].t = TERMINAL;
	grammar[9][3].ele.term = COLON;
	grammar[9][3].t = TERMINAL;
	grammar[9][4].ele.non_term = DATATYPE;
	grammar[9][4].t = NONTERMINAL;
	grammar[9][5].ele.non_term = N1;
	grammar[9][5].t = NONTERMINAL;

	grammar[10][0].ele.non_term = N1;
	grammar[10][0].t = NONTERMINAL;
	grammar[10][0].len = 1;
	grammar[10][1].ele.term = EPSILON;
	grammar[10][1].t = TERMINAL;

	grammar[11][0].ele.non_term = OUTPUTPLIST;
	grammar[11][0].t = NONTERMINAL;
	grammar[11][0].len = 4;
	grammar[11][1].ele.term = ID;
	grammar[11][1].t = TERMINAL;
	grammar[11][2].ele.term = COLON;
	grammar[11][2].t = TERMINAL;
	grammar[11][3].ele.non_term = TYPE;
	grammar[11][3].t = NONTERMINAL;
	grammar[11][4].ele.non_term = N2;
	grammar[11][4].t = NONTERMINAL;

	grammar[12][0].ele.non_term = N2;
	grammar[12][0].t = NONTERMINAL;
	grammar[12][0].len = 5;
	grammar[12][1].ele.term = COMMA;
	grammar[12][1].t = TERMINAL;
	grammar[12][2].ele.term = ID;
	grammar[12][2].t = TERMINAL;
	grammar[12][3].ele.term = COLON;
	grammar[12][3].t = TERMINAL;
	grammar[12][4].ele.non_term = TYPE;
	grammar[12][4].t = NONTERMINAL;
	grammar[12][5].ele.non_term = N2;
	grammar[12][5].t = NONTERMINAL;

	grammar[13][0].ele.non_term = N2;
	grammar[13][0].t = NONTERMINAL;
	grammar[13][0].len = 1;
	grammar[13][1].ele.term = EPSILON;
	grammar[13][1].t = TERMINAL;

	grammar[14][0].ele.non_term = DATATYPE;
	grammar[14][0].t = NONTERMINAL;
	grammar[14][0].len = 1;
	grammar[14][1].ele.term = INTEGER;
	grammar[14][1].t = TERMINAL;

	grammar[15][0].ele.non_term = DATATYPE;
	grammar[15][0].t = NONTERMINAL;
	grammar[15][0].len = 1;
	grammar[15][1].ele.term = REAL;
	grammar[15][1].t = TERMINAL;

	grammar[16][0].ele.non_term = DATATYPE;
	grammar[16][0].t = NONTERMINAL;
	grammar[16][0].len = 1;
	grammar[16][1].ele.term = BOOLEAN;
	grammar[16][1].t = TERMINAL;

	grammar[17][0].ele.non_term = DATATYPE;
	grammar[17][0].t = NONTERMINAL;
	grammar[17][0].len = 6;
	grammar[17][1].ele.term = ARRAY;
	grammar[17][1].t = TERMINAL;
	grammar[17][2].ele.term = SQBO;
	grammar[17][2].t = TERMINAL;
	grammar[17][3].ele.non_term = RANGE;
	grammar[17][3].t = NONTERMINAL;
	grammar[17][4].ele.term = SQBC;
	grammar[17][4].t = TERMINAL;
	grammar[17][5].ele.term = OF;
	grammar[17][5].t = TERMINAL;
	grammar[17][6].ele.non_term = TYPE;
	grammar[17][6].t = NONTERMINAL;

	grammar[18][0].ele.non_term = TYPE;
	grammar[18][0].t = NONTERMINAL;
	grammar[18][0].len = 1;
	grammar[18][1].ele.term = INTEGER;
	grammar[18][1].t = TERMINAL;

	grammar[19][0].ele.non_term = TYPE;
	grammar[19][0].t = NONTERMINAL;
	grammar[19][0].len = 1;
	grammar[19][1].ele.term = REAL;
	grammar[19][1].t = TERMINAL;

	grammar[20][0].ele.non_term = TYPE;
	grammar[20][0].t = NONTERMINAL;
	grammar[20][0].len = 1;
	grammar[20][1].ele.term = BOOLEAN;
	grammar[20][1].t = TERMINAL;

	grammar[21][0].ele.non_term = MODULEDEF;
	grammar[21][0].t = NONTERMINAL;
	grammar[21][0].len = 3;
	grammar[21][1].ele.term = START;
	grammar[21][1].t = TERMINAL;
	grammar[21][2].ele.non_term = STATEMENTS;
	grammar[21][2].t = NONTERMINAL;
	grammar[21][3].ele.term = END;
	grammar[21][3].t = TERMINAL;

	grammar[22][0].ele.non_term = STATEMENTS;
	grammar[22][0].t = NONTERMINAL;
	grammar[22][0].len = 2;
	grammar[22][1].ele.non_term = STATEMENT;
	grammar[22][1].t = NONTERMINAL;
	grammar[22][2].ele.non_term = STATEMENTS;
	grammar[22][2].t = NONTERMINAL;
	
	grammar[23][0].ele.non_term = STATEMENTS;
	grammar[23][0].t = NONTERMINAL;
	grammar[23][0].len = 1;
	grammar[23][1].ele.term = EPSILON;
	grammar[23][1].t = TERMINAL;	

	grammar[24][0].ele.non_term = STATEMENT;
	grammar[24][0].t = NONTERMINAL;
	grammar[24][0].len = 1;
	grammar[24][1].ele.non_term = IOSTMT;
	grammar[24][1].t = NONTERMINAL;

	grammar[25][0].ele.non_term = STATEMENT;
	grammar[25][0].t = NONTERMINAL;
	grammar[25][0].len = 1;
	grammar[25][1].ele.non_term = SIMPLESTMT;
	grammar[25][1].t = NONTERMINAL;

	grammar[26][0].ele.non_term = STATEMENT;
	grammar[26][0].t = NONTERMINAL;
	grammar[26][0].len = 1;
	grammar[26][1].ele.non_term = DECLARESTMT;
	grammar[26][1].t = NONTERMINAL;

	grammar[27][0].ele.non_term = STATEMENT;
	grammar[27][0].t = NONTERMINAL;
	grammar[27][0].len = 1;
	grammar[27][1].ele.non_term = CONDITIONALSTMT;
	grammar[27][1].t = NONTERMINAL;

	grammar[28][0].ele.non_term = STATEMENT;
	grammar[28][0].t = NONTERMINAL;
	grammar[28][0].len = 1;
	grammar[28][1].ele.non_term = ITERATIVESTMT;
	grammar[28][1].t = NONTERMINAL;

	grammar[29][0].ele.non_term = IOSTMT;
	grammar[29][0].t = NONTERMINAL;
	grammar[29][0].len = 5;
	grammar[29][1].ele.term = GET_VALUE;
	grammar[29][1].t = TERMINAL;
	grammar[29][2].ele.term = BO;
	grammar[29][2].t = TERMINAL;
	grammar[29][3].ele.term = ID;
	grammar[29][3].t = TERMINAL;
	grammar[29][4].ele.term = BC;
	grammar[29][4].t = TERMINAL;
	grammar[29][5].ele.term = SEMICOL;
	grammar[29][5].t = TERMINAL;

	grammar[30][0].ele.non_term = IOSTMT;
	grammar[30][0].t = NONTERMINAL;
	grammar[30][0].len = 5;
	grammar[30][1].ele.term = PRINT;
	grammar[30][1].t = TERMINAL;
	grammar[30][2].ele.term = BO;
	grammar[30][2].t = TERMINAL;
	grammar[30][3].ele.non_term = VAR;
	grammar[30][3].t = NONTERMINAL;
	grammar[30][4].ele.term = BC;
	grammar[30][4].t = TERMINAL;
	grammar[30][5].ele.term = SEMICOL;
	grammar[30][5].t = TERMINAL;

	grammar[31][0].ele.non_term = VAR;
	grammar[31][0].t = NONTERMINAL;
	grammar[31][0].len = 1;
	grammar[31][1].ele.term = INTEGER;
	grammar[31][1].t = TERMINAL;	

	grammar[32][0].ele.non_term = VAR;
	grammar[32][0].t = NONTERMINAL;
	grammar[32][0].len = 1;
	grammar[32][1].ele.term = REAL;
	grammar[32][1].t = TERMINAL;

	grammar[33][0].ele.non_term = VAR;
	grammar[33][0].t = NONTERMINAL;
	grammar[33][0].len = 2;
	grammar[33][1].ele.term = ID;
	grammar[33][1].t = TERMINAL;
	grammar[33][2].ele.non_term = WHICHID;
	grammar[33][2].t = NONTERMINAL;
	
	grammar[34][0].ele.non_term = WHICHID;
	grammar[34][0].t = NONTERMINAL;
	grammar[34][0].len = 1;
	grammar[34][1].ele.term = EPSILON;
	grammar[34][1].t = TERMINAL;

	grammar[35][0].ele.non_term = WHICHID;
	grammar[35][0].t = NONTERMINAL;
	grammar[35][0].len = 3;
	grammar[35][1].ele.term = SQBO;
	grammar[35][1].t = TERMINAL;
	grammar[35][2].ele.term = ID;
	grammar[35][2].t = TERMINAL;
	grammar[35][3].ele.term = SQBC;
	grammar[35][3].t = TERMINAL;

	grammar[36][0].ele.non_term = SIMPLESTMT;
	grammar[36][0].t = NONTERMINAL;
	grammar[36][0].len = 1;
	grammar[36][1].ele.non_term = ASSIGNMENTSTMT;
	grammar[36][1].t = NONTERMINAL;

	grammar[37][0].ele.non_term = SIMPLESTMT;
	grammar[37][0].t = NONTERMINAL;
	grammar[37][0].len = 1;
	grammar[37][1].ele.non_term = MODULEREUSESTMT;
	grammar[37][1].t = NONTERMINAL;

	grammar[38][0].ele.non_term = ASSIGNMENTSTMT;
	grammar[38][0].t = NONTERMINAL;
	grammar[38][0].len = 3;
	grammar[38][1].ele.term = ID;
	grammar[38][1].t = TERMINAL;
	grammar[38][2].ele.non_term = WHICHSTMT;
	grammar[38][2].t = NONTERMINAL;

	grammar[39][0].ele.non_term = WHICHSTMT;
	grammar[39][0].t = NONTERMINAL;
	grammar[39][0].len = 1;
	grammar[39][1].ele.non_term = LVALUEIDSTMT;
	grammar[39][1].t = NONTERMINAL;

	grammar[40][0].ele.non_term = WHICHSTMT;
	grammar[40][0].t = NONTERMINAL;
	grammar[40][0].len = 1;
	grammar[40][1].ele.non_term = LVALUEARRSTMT;
	grammar[40][1].t = NONTERMINAL;

	grammar[41][0].ele.non_term = LVALUEIDSTMT;
	grammar[41][0].t = NONTERMINAL;
	grammar[41][0].len = 3;
	grammar[41][1].ele.term = ASSIGNOP;
	grammar[41][1].t = TERMINAL;
	grammar[41][2].ele.non_term = EXPRESSION;
	grammar[41][2].t = NONTERMINAL;
	grammar[41][3].ele.term = SEMICOL;
	grammar[41][3].t = TERMINAL;

	grammar[42][0].ele.non_term = LVALUEARRSTMT;
	grammar[42][0].t = NONTERMINAL;
	grammar[42][0].len = 6;
	grammar[42][1].ele.term = SQBO;
	grammar[42][1].t = TERMINAL;
	grammar[42][2].ele.non_term = INDEX;
	grammar[42][2].t = NONTERMINAL;
	grammar[42][3].ele.term = SQBC;
	grammar[42][3].t = TERMINAL;
	grammar[42][4].ele.term = ASSIGNOP;
	grammar[42][4].t = TERMINAL;
	grammar[42][5].ele.non_term = EXPRESSION;
	grammar[42][5].t = NONTERMINAL;
	grammar[42][6].ele.term = SEMICOL;
	grammar[42][6].t = TERMINAL;

	grammar[43][0].ele.non_term = INDEX;
	grammar[43][0].t = NONTERMINAL;
	grammar[43][0].len = 1;
	grammar[43][1].ele.term = INTEGER;
	grammar[43][1].t = TERMINAL;

	grammar[44][0].ele.non_term = INDEX;
	grammar[44][0].t = NONTERMINAL;
	grammar[44][0].len = 1;
	grammar[44][1].ele.term = ID;
	grammar[44][1].t = TERMINAL;

	grammar[45][0].ele.non_term = MODULEREUSESTMT;
	grammar[45][0].t = NONTERMINAL;
	grammar[45][0].len = 8;
	grammar[45][1].ele.non_term = OPTIONAL;
	grammar[45][1].t = NONTERMINAL;
	grammar[45][2].ele.term = USE;
	grammar[45][2].t = TERMINAL;
	grammar[45][3].ele.term = MODULE;
	grammar[45][3].t = TERMINAL;
	grammar[45][4].ele.term = ID;
	grammar[45][4].t = TERMINAL;
	grammar[45][5].ele.term = WITH;
	grammar[45][5].t = TERMINAL;
	grammar[45][6].ele.term = PARAMETERS;
	grammar[45][6].t = TERMINAL;
	grammar[45][7].ele.non_term = IDLIST;
	grammar[45][7].t = NONTERMINAL;
	grammar[45][8].ele.term = SEMICOL;
	grammar[45][8].t = TERMINAL;

	grammar[46][0].ele.non_term = OPTIONAL;
	grammar[46][0].t = NONTERMINAL;
	grammar[46][0].len = 1;
	grammar[46][1].ele.term = EPSILON;
	grammar[46][1].t = TERMINAL;

	grammar[47][0].ele.non_term = OPTIONAL;
	grammar[47][0].t = NONTERMINAL;
	grammar[47][0].len = 4;
	grammar[47][1].ele.term = SQBO;
	grammar[47][1].t = TERMINAL;
	grammar[47][2].ele.non_term = IDLIST;
	grammar[47][2].t = NONTERMINAL;
	grammar[47][3].ele.term = SQBC;
	grammar[47][3].t = TERMINAL;
	grammar[47][4].ele.term = ASSIGNOP;
	grammar[47][4].t = TERMINAL;
	
	grammar[48][0].ele.non_term = IDLIST;
	grammar[48][0].t = NONTERMINAL;
	grammar[48][0].len = 2;
	grammar[48][1].ele.term = ID;
	grammar[48][1].t = TERMINAL;
	grammar[48][2].ele.non_term = N3;
	grammar[48][2].t = NONTERMINAL;

	grammar[49][0].ele.non_term = N3;
	grammar[49][0].t = NONTERMINAL;
	grammar[49][0].len = 1;
	grammar[49][1].ele.term = EPSILON;
	grammar[49][1].t = TERMINAL;	

	grammar[50][0].ele.non_term = N3;
	grammar[50][0].t = NONTERMINAL;
	grammar[50][0].len = 3;
	grammar[50][1].ele.term = COMMA;
	grammar[50][1].t = TERMINAL;
	grammar[50][2].ele.term = ID;
	grammar[50][2].t = TERMINAL;
	grammar[50][3].ele.non_term = N3;
	grammar[50][3].t = NONTERMINAL;
	
	grammar[51][0].ele.non_term = EXPRESSION;
	grammar[51][0].t = NONTERMINAL;
	grammar[51][0].len = 1;
	grammar[51][1].ele.non_term = ARITHMETICORBOOLEANEXPR;
	grammar[51][1].t = NONTERMINAL;

	grammar[52][0].ele.non_term = EXPRESSION;
	grammar[52][0].t = NONTERMINAL;
	grammar[52][0].len = 4;
	grammar[52][1].ele.term = MINUS;
	grammar[52][1].t = TERMINAL;
	grammar[52][2].ele.term = BO;
	grammar[52][2].t = TERMINAL;
	grammar[52][3].ele.non_term = ARITHMETICEXPR;
	grammar[52][3].t = NONTERMINAL;
	grammar[52][4].ele.term = BC;
	grammar[52][4].t = TERMINAL;
	
	grammar[53][0].ele.non_term = ARITHMETICORBOOLEANEXPR;
	grammar[53][0].t = NONTERMINAL;
	grammar[53][0].len = 2;
	grammar[53][1].ele.non_term = ANYTERM;
	grammar[53][1].t = NONTERMINAL;
	grammar[53][2].ele.non_term = N7;
	grammar[53][2].t = NONTERMINAL;
	
	grammar[54][0].ele.non_term = N7;
	grammar[54][0].t = NONTERMINAL;
	grammar[54][0].len = 1;
	grammar[54][1].ele.term = EPSILON;
	grammar[54][1].t = TERMINAL;	

	grammar[55][0].ele.non_term = N7;
	grammar[55][0].t = NONTERMINAL;
	grammar[55][0].len = 3;
	grammar[55][1].ele.non_term = LOGICALOP;
	grammar[55][1].t = NONTERMINAL;
	grammar[55][2].ele.non_term = ANYTERM;
	grammar[55][2].t = NONTERMINAL;
	grammar[55][3].ele.non_term = N7;
	grammar[55][3].t = NONTERMINAL;
	
	grammar[56][0].ele.non_term = ANYTERM;
	grammar[56][0].t = NONTERMINAL;
	grammar[56][0].len = 2;
	grammar[56][1].ele.non_term = ARITHMETICEXPR;
	grammar[56][1].t = NONTERMINAL;
	grammar[56][2].ele.non_term = N8;
	grammar[56][2].t = NONTERMINAL;
	
	grammar[57][0].ele.non_term = N8;
	grammar[57][0].t = NONTERMINAL;
	grammar[57][0].len = 1;
	grammar[57][1].ele.term = EPSILON;
	grammar[57][1].t = TERMINAL;	

	grammar[58][0].ele.non_term = N8;
	grammar[58][0].t = NONTERMINAL;
	grammar[58][0].len = 3;
	grammar[58][1].ele.non_term = RELATIONALOP;
	grammar[58][1].t = NONTERMINAL;
	grammar[58][2].ele.non_term = ARITHMETICEXPR;
	grammar[58][2].t = NONTERMINAL;
	grammar[58][3].ele.non_term = N8;
	grammar[58][3].t = NONTERMINAL;

	grammar[59][0].ele.non_term = ARITHMETICEXPR;
	grammar[59][0].t = NONTERMINAL;
	grammar[59][0].len = 2;
	grammar[59][1].ele.non_term = TERM;
	grammar[59][1].t = NONTERMINAL;
	grammar[59][2].ele.non_term = N4;
	grammar[59][2].t = NONTERMINAL;

	grammar[60][0].ele.non_term = N4;
	grammar[60][0].t = NONTERMINAL;
	grammar[60][0].len = 1;
	grammar[60][1].ele.term = EPSILON;
	grammar[60][1].t = TERMINAL;	

	grammar[61][0].ele.non_term = N4;
	grammar[61][0].t = NONTERMINAL;
	grammar[61][0].len = 3;
	grammar[61][1].ele.non_term = OP1;
	grammar[61][1].t = NONTERMINAL;
	grammar[61][2].ele.non_term = TERM;
	grammar[61][2].t = NONTERMINAL;
	grammar[61][3].ele.non_term = N4;
	grammar[61][3].t = NONTERMINAL;	
	
	grammar[62][0].ele.non_term = TERM;
	grammar[62][0].t = NONTERMINAL;
	grammar[62][0].len = 2;
	grammar[62][1].ele.non_term = FACTOR;
	grammar[62][1].t = NONTERMINAL;
	grammar[62][2].ele.non_term = N5;
	grammar[62][2].t = NONTERMINAL;

	grammar[63][0].ele.non_term = N5;
	grammar[63][0].t = NONTERMINAL;
	grammar[63][0].len = 1;
	grammar[63][1].ele.term = EPSILON;
	grammar[63][1].t = TERMINAL;	

	grammar[64][0].ele.non_term = N5;
	grammar[64][0].t = NONTERMINAL;
	grammar[64][0].len = 3;
	grammar[64][1].ele.non_term = OP2;
	grammar[64][1].t = NONTERMINAL;
	grammar[64][2].ele.non_term = FACTOR;
	grammar[64][2].t = NONTERMINAL;
	grammar[64][3].ele.non_term = N5;
	grammar[64][3].t = NONTERMINAL;	
	
	grammar[65][0].ele.non_term = FACTOR;
	grammar[65][0].t = NONTERMINAL;
	grammar[65][0].len = 1;
	grammar[65][1].ele.non_term = VAR;
	grammar[65][1].t = NONTERMINAL;	

	grammar[66][0].ele.non_term = OP1;
	grammar[66][0].t = NONTERMINAL;
	grammar[66][0].len = 1;
	grammar[66][1].ele.term = PLUS;
	grammar[66][1].t = TERMINAL;

	grammar[67][0].ele.non_term = OP1;
	grammar[67][0].t = NONTERMINAL;
	grammar[67][0].len = 1;
	grammar[67][1].ele.term = MINUS;
	grammar[67][1].t = TERMINAL;

	grammar[68][0].ele.non_term = OP2;
	grammar[68][0].t = NONTERMINAL;
	grammar[68][0].len = 1;
	grammar[68][1].ele.term = MUL;
	grammar[68][1].t = TERMINAL;

	grammar[69][0].ele.non_term = OP2;
	grammar[69][0].t = NONTERMINAL;
	grammar[69][0].len = 1;
	grammar[69][1].ele.term = DIV;
	grammar[69][1].t = TERMINAL;

	grammar[70][0].ele.non_term = LOGICALOP;
	grammar[70][0].t = NONTERMINAL;
	grammar[70][0].len = 1;
	grammar[70][1].ele.term = AND;
	grammar[70][1].t = TERMINAL;

	grammar[71][0].ele.non_term = LOGICALOP;
	grammar[71][0].t = NONTERMINAL;
	grammar[71][0].len = 1;
	grammar[71][1].ele.term = OR;
	grammar[71][1].t = TERMINAL;

	grammar[72][0].ele.non_term = RELATIONALOP;
	grammar[72][0].t = NONTERMINAL;
	grammar[72][0].len = 1;
	grammar[72][1].ele.term = LT;
	grammar[72][1].t = TERMINAL;

	grammar[73][0].ele.non_term = RELATIONALOP;
	grammar[73][0].t = NONTERMINAL;
	grammar[73][0].len = 1;
	grammar[73][1].ele.term = LE;
	grammar[73][1].t = TERMINAL;
	
	grammar[74][0].ele.non_term = RELATIONALOP;
	grammar[74][0].t = NONTERMINAL;
	grammar[74][0].len = 1;
	grammar[74][1].ele.term = GT;
	grammar[74][1].t = TERMINAL;

	grammar[75][0].ele.non_term = RELATIONALOP;
	grammar[75][0].t = NONTERMINAL;
	grammar[75][0].len = 1;
	grammar[75][1].ele.term = GE;
	grammar[75][1].t = TERMINAL;
	
	grammar[76][0].ele.non_term = RELATIONALOP;
	grammar[76][0].t = NONTERMINAL;
	grammar[76][0].len = 1;
	grammar[76][1].ele.term = EQ;
	grammar[76][1].t = TERMINAL;
	
	grammar[77][0].ele.non_term = RELATIONALOP;
	grammar[77][0].t = NONTERMINAL;
	grammar[77][0].len = 1;
	grammar[77][1].ele.term = NE;
	grammar[77][1].t = TERMINAL;

	grammar[78][0].ele.non_term = DECLARESTMT;
	grammar[78][0].t = NONTERMINAL;
	grammar[78][0].len = 5;
	grammar[78][1].ele.term = DECLARE;
	grammar[78][1].t = TERMINAL;
	grammar[78][2].ele.non_term = IDLIST;
	grammar[78][2].t = TERMINAL;
	grammar[78][3].ele.term = COLON;
	grammar[78][3].t = TERMINAL;
	grammar[78][4].ele.non_term = DATATYPE;
	grammar[78][4].t = NONTERMINAL;
	grammar[78][5].ele.term = SEMICOL;
	grammar[78][5].t = TERMINAL;
	
	grammar[79][0].ele.non_term = CONDITIONALSTMT;
	grammar[79][0].t = NONTERMINAL;
	grammar[79][0].len = 8;
	grammar[79][1].ele.term = SWITCH;
	grammar[79][1].t = TERMINAL;
	grammar[79][2].ele.term = BO;
	grammar[79][2].t = TERMINAL;
	grammar[79][3].ele.term = ID;
	grammar[79][3].t = TERMINAL;
	grammar[79][4].ele.term = BC;
	grammar[79][4].t = TERMINAL;
	grammar[79][5].ele.term = START;
	grammar[79][5].t = TERMINAL;
	grammar[79][6].ele.non_term = CASESTMTS;
	grammar[79][6].t = NONTERMINAL;
	grammar[79][7].ele.non_term = DEFAULTNT;
	grammar[79][7].t = NONTERMINAL;
	grammar[79][8].ele.term = END;
	grammar[79][8].t = TERMINAL;

	grammar[80][0].ele.non_term = CASESTMTS;
	grammar[80][0].t = NONTERMINAL;
	grammar[80][0].len = 7;
	grammar[80][1].ele.term = CASE;
	grammar[80][1].t = TERMINAL;
	grammar[80][2].ele.non_term = VALUE;
	grammar[80][2].t = NONTERMINAL;
	grammar[80][3].ele.term = COLON;
	grammar[80][3].t = TERMINAL;
	grammar[80][4].ele.non_term = STATEMENTS;
	grammar[80][4].t = NONTERMINAL;
	grammar[80][5].ele.term = BREAK;
	grammar[80][5].t = TERMINAL;
	grammar[80][6].ele.term = SEMICOL;
	grammar[80][6].t = TERMINAL;
	grammar[80][7].ele.non_term = N9;
	grammar[80][7].t = NONTERMINAL;

	grammar[81][0].ele.non_term = N9;
	grammar[81][0].t = NONTERMINAL;
	grammar[81][0].len = 7;
	grammar[81][1].ele.term = CASE;
	grammar[81][1].t = TERMINAL;
	grammar[81][2].ele.non_term = VALUE;
	grammar[81][2].t = NONTERMINAL;
	grammar[81][3].ele.term = COLON;
	grammar[81][3].t = TERMINAL;
	grammar[81][4].ele.non_term = STATEMENTS;
	grammar[81][4].t = NONTERMINAL;
	grammar[81][5].ele.term = BREAK;
	grammar[81][5].t = TERMINAL;
	grammar[81][6].ele.term = SEMICOL;
	grammar[81][6].t = TERMINAL;
	grammar[81][7].ele.non_term = N9;
	grammar[81][7].t = NONTERMINAL;
	
	grammar[82][0].ele.non_term = N9;
	grammar[82][0].t = NONTERMINAL;
	grammar[82][0].len = 1;
	grammar[82][1].ele.term = EPSILON;
	grammar[82][1].t = TERMINAL;	

	grammar[83][0].ele.non_term = VALUE;
	grammar[83][0].t = NONTERMINAL;
	grammar[83][0].len = 1;
	grammar[83][1].ele.term = INTEGER;
	grammar[83][1].t = TERMINAL;	

	grammar[84][0].ele.non_term = VALUE;
	grammar[84][0].t = NONTERMINAL;
	grammar[84][0].len = 1;
	grammar[84][1].ele.term = TRUE;
	grammar[84][1].t = TERMINAL;	

	grammar[85][0].ele.non_term = VALUE;
	grammar[85][0].t = NONTERMINAL;
	grammar[85][0].len = 1;
	grammar[85][1].ele.term = FALSE;
	grammar[85][1].t = TERMINAL;	

	grammar[86][0].ele.non_term = DEFAULTNT;
	grammar[86][0].t = NONTERMINAL;
	grammar[86][0].len = 5;
	grammar[86][1].ele.term = DEFAULT;
	grammar[86][1].t = TERMINAL;
	grammar[86][2].ele.term = COLON;
	grammar[86][2].t = TERMINAL;
	grammar[86][3].ele.non_term = STATEMENTS;
	grammar[86][3].t = NONTERMINAL;
	grammar[86][4].ele.term = BREAK;
	grammar[86][4].t = TERMINAL;
	grammar[86][5].ele.term = SEMICOL;
	grammar[86][5].t = TERMINAL;
	
	grammar[87][0].ele.non_term = DEFAULTNT;
	grammar[87][0].t = NONTERMINAL;
	grammar[87][0].len = 1;
	grammar[87][1].ele.term = EPSILON;
	grammar[87][1].t = TERMINAL;	

	grammar[88][0].ele.non_term = ITERATIVESTMT;
	grammar[88][0].t = NONTERMINAL;
	grammar[88][0].len = 9;
	grammar[88][1].ele.term = FOR;
	grammar[88][1].t = TERMINAL;
	grammar[88][2].ele.term = BO;
	grammar[88][2].t = TERMINAL;
	grammar[88][3].ele.term = ID;
	grammar[88][3].t = TERMINAL;
	grammar[88][4].ele.term = IN;
	grammar[88][4].t = TERMINAL;
	grammar[88][5].ele.non_term = RANGE;
	grammar[88][5].t = NONTERMINAL;
	grammar[88][6].ele.term = BC;
	grammar[88][6].t = TERMINAL;
	grammar[88][7].ele.term = START;
	grammar[88][7].t = TERMINAL;
	grammar[88][8].ele.non_term = STATEMENTS;
	grammar[88][8].t = NONTERMINAL;
	grammar[88][9].ele.term = END;
	grammar[88][9].t = TERMINAL;
	
	grammar[89][0].ele.non_term = ITERATIVESTMT;
	grammar[89][0].t = NONTERMINAL;
	grammar[89][0].len = 7;
	grammar[89][1].ele.term = WHILE;
	grammar[89][1].t = TERMINAL;
	grammar[89][2].ele.term = BO;
	grammar[89][2].t = TERMINAL;
	grammar[89][3].ele.non_term = ARITHMETICORBOOLEANEXPR;
	grammar[89][3].t = NONTERMINAL;
	grammar[89][4].ele.term = BC;
	grammar[89][4].t = TERMINAL;
	grammar[89][5].ele.term = START;
	grammar[89][5].t = TERMINAL;
	grammar[89][6].ele.non_term = STATEMENTS;
	grammar[89][6].t = NONTERMINAL;
	grammar[89][7].ele.term = END;
	grammar[89][7].t = TERMINAL;

	grammar[90][0].ele.non_term = RANGE;
	grammar[90][0].t = NONTERMINAL;
	grammar[90][0].len = 3;
	grammar[90][1].ele.term = INTEGER;
	grammar[90][1].t = TERMINAL;
	grammar[90][2].ele.term = RANGEOP;
	grammar[90][2].t = TERMINAL;
	grammar[90][3].ele.term = INTEGER;
	grammar[90][3].t = TERMINAL;

	grammar[91][0].ele.non_term = OTHERMODULES;
	grammar[91][0].t = NONTERMINAL;
	grammar[91][0].len = 2;
	grammar[91][1].ele.non_term = MODULENT;
	grammar[91][1].t = NONTERMINAL;
	grammar[91][2].ele.non_term = OTHERMODULES;
	grammar[91][2].t = NONTERMINAL;

	grammar[92][0].ele.non_term = OTHERMODULES;
	grammar[92][0].t = NONTERMINAL;
	grammar[92][0].len = 1;
	grammar[92][1].ele.term = EPSILON;
	grammar[92][1].t = TERMINAL;

	grammar[93][0].ele.non_term = ARITHMETICORBOOLEANEXPR;
	grammar[93][0].t = NONTERMINAL;
	grammar[93][0].len = 3;
	grammar[93][1].ele.term =BO;
	grammar[93][1].t = TERMINAL;
	grammar[93][2].ele.non_term = ARITHMETICORBOOLEANEXPR;
	grammar[93][2].t = NONTERMINAL;
	grammar[93][3].ele.term = BC;
	grammar[93][3].t = TERMINAL;

}

int main(){
	populateFirstSet();
	populateFollowSet();
	populateGrammar();
	printf("%d",ffset.first[OP1][0].t);
	return 0;
}

