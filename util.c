#define _CRT_SECURE_NO_WARNINGS
#include "globals.h"


void printToken(TokenType token, const char* tokenstring) {
	switch (token) {
	case IF: fprintf(listing, "IF\n");
		break;
	case THEN:fprintf(listing, "THEN\n");
		break;
	case ELSE:fprintf(listing, "ELSE\n");
		break;
	case WHILE:fprintf(listing, "WHILE\n");
		break;
	case EXIT:fprintf(listing, "EXIT\n");
		break;
	case PLUS: fprintf(listing, "+\n");
		break;
	case TIMES: fprintf(listing, "*\n");
		break;
	case OVER: fprintf(listing, ">\n");
		break;
	case EQUAL: fprintf(listing, "==\n");
		break;
	case ASSIGN: fprintf(listing, "=\n");
		break;
	case SEMI:fprintf(listing, ";\n");
		break;
	case LSMALL:fprintf(listing, "(\n");
		break;
	case RSMALL:fprintf(listing, ")\n");
		break;
	case LMIDDLE:fprintf(listing, "{\n");
		break;
	case RMIDDLE:fprintf(listing, "}\n");
		break;
	case INT: fprintf(listing, "int\n");
		break;
	case CHAR: fprintf(listing, "char\n");
		break;
	case NUM: fprintf(listing, "%s\n", tokenstring);
		break;
	case WORD: fprintf(listing, "%s\n", tokenstring);
		break;
	case ENDFILE: fprintf(listing, "EOF\n");
		break;
	case ERROR: fprintf(listing, "ERROR: %s\n", tokenstring);
		break;
	default: fprintf(listing, "Unknown token");
	}
}


char* copystring(char* s) {
	int n;
	char* t;
	if (s == NULL)
		return NULL;
	n = strlen(s) + 1;
	t = malloc(n);
	if (t == NULL)
		fprintf(listing, "Out of memory\n");
	else
		strcpy(t, s);
	return t;
}
char* enumtostring(NodeType nt) {
	char* s="";
	if (nt==PROGK) {
		s = "PROG";
	}
	else if (nt == WORDK) {
		s = "WORD";
	}
	else if (nt == BLOCKK) {
		s = "BLOCK";
	}
	else if (nt == DECLSK) {
		s = "DECLS";
	}
	else if (nt == DECLK) {
		s = "DECL";
	}
	else if (nt == VTYPEK) {
		s = "Vtype"; 
	}
	else if (nt == SLISTK) {
		s = "SLIST";
	}
	else if (nt == STATK_IF) {
		s = "IF";
	}
	else if (nt == STATK_WHILE) {
		s = "WHILE";
	}
	else if (nt == STATK_EXIT) {
		s = "EXIT";
	}
	else if (nt == STATK_ASSIGN) {
		s = "ASSIGN";
	}
	else if (nt == CONDK) {
		s = "COND";
	}
	else if (nt == EXPRK) {
		s = "EXPR";
	}
	else if (nt == TERMK) {
		s = "TERM";
	}
	else if (nt == FACTK) {
		s = "FACT";
	}
	else if (nt == NUMK) {
		s = "NUM";
	}
	return s;
}

void printTree(TreeNode* root) {
	fprintf(listing, "%s ", enumtostring(root->NT));
	if (root->child[0] != NULL) {
		printTree(root->child[0]);
	}
	if (root->child[1] != NULL) {
		printTree(root->child[1]);
	}
	if (root->child[2] != NULL) {
		printTree(root->child[2]);
	}
}

