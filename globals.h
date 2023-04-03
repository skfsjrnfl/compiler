#ifndef _GLOBALS_H_
#define _GLOBALS_H_


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXRESERVED 7
typedef enum { false, true }bool;

typedef enum {
	ENDFILE, ERROR,
	IF, THEN, ELSE, WHILE, EXIT, INT, CHAR,
	NUM, WORD,
	PLUS, TIMES, OVER, EQUAL, ASSIGN, SEMI, LSMALL, RSMALL, LMIDDLE, RMIDDLE
}TokenType;

extern FILE* source;
extern FILE* listing;
extern FILE* code;

extern int lineno;

typedef enum { PROGK, WORDK, BLOCKK, DECLSK, DECLK, VTYPEK, SLISTK, STATK_IF,STATK_WHILE,STATK_EXIT,STATK_ASSIGN, CONDK, EXPRK, TERMK, FACTK, NUMK }NodeType;

#define MAXCHILDREN 3

typedef struct _Node {
	struct _Node* child[MAXCHILDREN];
	NodeType NT;
	union {
		TokenType op;
		int number;
		char* words;
	}attribute;
}TreeNode;

typedef struct _Symbol {
	char* name;
	char* type;
	int scope;
	struct _Symbol* overlap;
}Symbol;

#endif

