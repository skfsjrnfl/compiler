#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parser.h"



extern int Error;
static TokenType token;

int scopecounter = 0;
Symbol* Symboltable[256];
int symbolposition = 0;

int findSymbol(char* s);
void printSymbol(FILE* fp);

TreeNode* Vtype();
TreeNode* Word(void);
TreeNode* Block();
TreeNode* Decls();
TreeNode* Decl();
TreeNode* Slist();
TreeNode* Stat();
TreeNode* Expr();
TreeNode* Cond();
TreeNode* Prog();


static void syntaxError(char* message) {
	fprintf(listing, "\n>>> Syntax error at line %d: %s",lineno,message);
	Error = true;
}

static void match(TokenType expected) {
	if (token == expected) {
		token = getToken();
	}
	else {
		syntaxError("unexpected token ->");
		printToken(token, tokenString);
		fprintf(listing, "         ");
	}
}

TreeNode* Prog() {
	TreeNode* t = NULL;
	if (token == WORD) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		t->NT = PROGK;
		t->child[0] = Word();
		match(LSMALL);
		match(RSMALL);
		t->child[1] = Block();
		t->child[2] = NULL;
	}
	return t;
}

TreeNode* Word(void) {
	TreeNode* t=NULL;
	if (token == WORD) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		if (t != NULL) {
			t->attribute.words = copystring(tokenString);
			t->NT = WORDK;
			t->child[0] = NULL;
			t->child[1] = NULL;
			t->child[2] = NULL;
		}
	}
	else {
 		syntaxError("Unexpected token -> ");
		printToken(token, tokenString);
	}
	token = getToken();
	return t;
}
TreeNode* Num() {
	TreeNode* t = NULL;
	if (token == NUM) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		if (t != NULL) {
			t->attribute.number = atoi(tokenString);
			t->NT = NUMK;
			t->child[0] = NULL;
			t->child[1] = NULL;
			t->child[2] = NULL;
		}
		match(NUM);
	}
	else {
		syntaxError("Unexpected token -> ");
		printToken(token, tokenString);
	}
	return t;
}

TreeNode* Fact() {
	TreeNode* t = NULL;
	if (token == NUM) {
		t=(TreeNode*)malloc(sizeof(TreeNode));
		t->child[0]=Num();
		t->child[1] = NULL;
		t->child[2] = NULL;
		t->NT = FACTK;
	}
	else if (token == WORD) {
		t=(TreeNode*)malloc(sizeof(TreeNode));
		t->child[0]=Word();
		t->child[1] = NULL;
		t->child[2] = NULL;
		t->NT = FACTK;
	}
	else {
		syntaxError("Unexpected token -> ");
		printToken(token, tokenString);
	}
	return t;
}

TreeNode* Term() {
	TreeNode* t = NULL;
	if (token == NUM || token == WORD) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		t->NT = TERMK;
		t->child[0] = Fact();
		if (token == TIMES) {
			match(TIMES);
			t->child[1] = Fact();
		}
		else {
			t->child[1] = NULL;
		}
		t->child[2] = NULL;
	}
	return t;
}

TreeNode* Block() {
	TreeNode* t = NULL;
	if (token == LMIDDLE) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		scopecounter++;
		match(LMIDDLE);
		t->child[0] = Decls();
		t->child[1] = Slist();
		t->child[2] = NULL;
		t->NT = BLOCKK;
		match(RMIDDLE);
		scopecounter--;
	}else {
		syntaxError("Unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}


TreeNode* Decls() {
	TreeNode* t = NULL;
	if (token == INT || token == CHAR) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		t->NT = DECLSK;
		t->child[0] = Decl();
		t->child[2] = NULL;
		if (token == INT || token == CHAR) {
			t->child[1] = Decls();
		}
		else {
			t->child[1] = NULL;
		}
	}
	return t;
}

TreeNode* Decl() {
	TreeNode* t = NULL;
	if (token == INT || token == CHAR) {
		int p;
		Symbol* currentsym;
		t = (TreeNode*)malloc(sizeof(TreeNode));
		t->NT = DECLK;
		t->child[0] = Vtype();
		t->child[1] = Word();
		t->child[2] = NULL;
		match(SEMI);
		if ((p = findSymbol(t->child[1]->attribute.words)) != -1) {
			currentsym = Symboltable[p];
			while (currentsym->scope < scopecounter) {
				if (currentsym->overlap == NULL) {
					currentsym->overlap = malloc(sizeof(Symbol));
					currentsym = currentsym = currentsym->overlap;
					break;
				}
				currentsym = currentsym->overlap;
			}
			currentsym->name = t->child[1]->attribute.words;
			currentsym->type = t->child[0]->attribute.words;
			currentsym->scope = scopecounter;
			currentsym->overlap = NULL;

		}
		else {
			Symboltable[symbolposition] = malloc(sizeof(Symbol));
			Symboltable[symbolposition]->name = t->child[1]->attribute.words;
			Symboltable[symbolposition]->type = t->child[0]->attribute.words;
			Symboltable[symbolposition]->scope = scopecounter;
			Symboltable[symbolposition]->overlap = NULL;
			symbolposition++;
		}
	}
	else {
		syntaxError("Unexpected token -> ");
		printToken(token, tokenString);
	}
	return t;
}

TreeNode* Vtype() {
	TreeNode* t = NULL;	
	if (token == CHAR||token==INT) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		t->attribute.words = copystring(tokenString);
		token = getToken();
		t->NT = VTYPEK;
		t->child[0] = NULL;
		t->child[1] = NULL;
		t->child[2] = NULL;
	}
	else {
		syntaxError("Unexpected token -> ");
		printToken(token, tokenString);
	}
	
	return t;
}

TreeNode* Slist() {
	TreeNode* t = NULL;
	if (token == IF || token == WHILE || token == EXIT || token == WORD) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		t->NT = SLISTK;
		t->child[0] = Stat();
		if (token == IF || token == WHILE || token == EXIT || token == WORD) {
			t->child[1] = Slist();
			t->child[2] = NULL;
		}
		else {
			t->child[1] = NULL;
			t->child[2] = NULL;
		}
	}
	return t;
}

TreeNode* Stat() {
	TreeNode* t = NULL;
	if (token == IF) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		match(IF);
		t->child[0] = Cond();
		match(THEN);
		t->child[1] = Block();
		match(ELSE);
		t->child[2] = Block();
		t->NT = STATK_IF;
	}
	else if (token == WHILE) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		match(WHILE);
		t->child[0] = Cond();
		t->child[1] = Block();
		t->child[2] = NULL;
		t->NT = STATK_WHILE;
	}
	else if (token == WORD) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		t->child[0] = Word();
		match(ASSIGN);
		t->child[1] = Expr();
		match(SEMI);
		t->child[2] = NULL;
		t->NT = STATK_ASSIGN;
	}
	else if (token == EXIT) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		match(EXIT);
		t->child[0] = Expr();
		match(SEMI);
		t->child[1] = NULL;
		t->child[2] = NULL;
		t->NT = STATK_EXIT;
	}
	else {
		syntaxError("Unexpected token -> ");
		printToken(token, tokenString);
	}
	return t;
}

TreeNode* Expr() {
	TreeNode* t = NULL;
	t = (TreeNode*)malloc(sizeof(TreeNode));
	t->NT = EXPRK;
	t->child[0] = Term();
	if (token == PLUS) {
		match(PLUS);
		t->child[1] = Term();
	}
	else
		t->child[1] = NULL;
	t->child[2] = NULL;
	return t;
}

TreeNode* Cond() {
	TreeNode* t = NULL;
	if (token == WORD||token==NUM){
		t = (TreeNode*)malloc(sizeof(TreeNode));
		t->NT = CONDK;
		t->child[0] = Expr();
		if (token == OVER) {
			match(OVER);
			t->attribute.op = OVER;
		}
		else if (token == EQUAL) {
			match(EQUAL);
			t->attribute.op = EQUAL;
		}
		t->child[1] = Expr();
		t->child[2] = NULL;
	}
	return t;
}

TreeNode* parser(void) {
	TreeNode* t;
	token = getToken();
	t = Prog();
	if (token != ENDFILE) {
		syntaxError("Code End before EOF");
	}
	return t;
}

int findSymbol(char* s) {
	for (int i = 0; i < symbolposition; i++) {
		if (Symboltable[i]->name == s) {
			return i;
		}
	}
	return -1;
}

void printSymbol(FILE* fp) {
	Symbol* printsym;
	for (int i = 0; i < symbolposition; i++) {
		printsym = Symboltable[i];
		fprintf(fp, "Type: %s  Name: %s  Scope: %d\n", printsym->type, printsym->name, printsym->scope);
		while (printsym->overlap != NULL) {
			printsym = printsym->overlap;
			fprintf(fp, "Type: %s  Name: %s  Scope: %d\n", printsym->type, printsym->name, printsym->scope);
		}
	}
}