#include "globals.h"
#include "util.h"
#include "scan.h"

#define BUFLEN 256
typedef enum { START, INNUM, INWORD, INASSIGN,DONE } StateType;

char tokenString[MAXTOKENLEN + 1];

static char lineBuf[BUFLEN];
static int linepos = 0;
static int bufsize = 0;

static char getNextChar(void) {
	if (!(linepos < bufsize)) {
		lineno++;
		if (fgets(lineBuf, BUFLEN - 1, source)) {
			bufsize = strlen(lineBuf);
			linepos = 0;
			return lineBuf[linepos++];
		}
		else return EOF;
	}
	else return lineBuf[linepos++];
}

static void ungetNextChar(void) {
	linepos--;
}

static struct {
	char* str;
	TokenType tok;
}reservedWord[MAXRESERVED] = { {"IF",IF},{"THEN",THEN},{"ELSE",ELSE},{"WHILE",WHILE},{"EXIT",EXIT},{"int",INT},{"char",CHAR} };

static TokenType reservedLookup(char* s) {
	for (int i = 0; i < MAXRESERVED; i++) {
		if (!strcmp(s, reservedWord[i].str))
			return reservedWord[i].tok;
	}
	return WORD;
}

TokenType getToken(void) {
	int tokenStringIndex = 0;
	TokenType currentToken;
	StateType state = START;
	int save;
	while (state != DONE) {
		char c = getNextChar();
		save = true;
		switch (state) {
		case START:
			if (isdigit(c))
				state = INNUM;
			else if (isalpha(c))
				state = INWORD;
			else if (c == '=')
				state = INASSIGN;
			else if ((c == ' ') || (c == '\t') || (c == '\n'))
				save = false;
			else {
				state = DONE;
				switch (c){
				case EOF:
					save = false;
					currentToken = ENDFILE;
					break;
				case '=':
					currentToken = ASSIGN;
					break;
				case '>':
					currentToken = OVER;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '*':
					currentToken = TIMES;
					break;
				case '(':
					currentToken = LSMALL;
					break;
				case ')':
					currentToken = RSMALL;
					break;
				case '{':
					currentToken = LMIDDLE;
					break;
				case '}':
					currentToken = RMIDDLE;
					break;
				case ';':
					currentToken = SEMI;
					break;
				default:
					currentToken = ERROR;
					break;
				}
			}
			break;
		case INASSIGN:
			state = DONE;
			if (c == '=')
				currentToken = EQUAL;
			else {
				ungetNextChar();
				save = false;
				currentToken = ASSIGN;
			}
			break;
		case INNUM:
			if (!isdigit(c)) {
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = NUM;
			}
			break;
		case INWORD:
			if (!isalpha(c)) {
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = WORD;
			}
			break;
		case DONE:
		default:
			fprintf(listing, "Scanner Bug: state= %d\n", state);
			state = DONE;
			currentToken = ERROR;
			break;
		}
 		if ((save) && (tokenStringIndex <= MAXTOKENLEN)) {
			tokenString[tokenStringIndex++] = c;
		}
		if (state == DONE) {
			tokenString[tokenStringIndex] = '\0';
			if (currentToken == WORD)
				currentToken = reservedLookup(tokenString);
		}
	}
	fprintf(listing, "\t%d: ", lineno);
	printToken(currentToken, tokenString);
	return currentToken;
}