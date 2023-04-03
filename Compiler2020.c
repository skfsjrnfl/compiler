#define _CRT_SECURE_NO_WARNINGS
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parser.h"

int lineno = 0;
FILE* source;
FILE* listing;
FILE* code;
int Error = false;

int main(int argc, char** argv) {
	TreeNode* syntaxTree;
	char pgm[20];
	
	if (argc != 2) {
		printf("Wrong execution\n");
		printf("Please type complier2020 input.txt\n");
		exit(1);
	}
	strcpy(pgm, argv[1]);
	if (strchr(pgm, '.') == NULL)
		strcat(pgm,".txt");
	source = fopen(pgm, "r");

	
	if (source == NULL) {
		printf("Can't open input file\n");
		exit(1);
	}
	
	listing = stdout;
	fprintf(listing, "debug %s\n\n", pgm);
	printf("Token list\n\n");
	syntaxTree = parser(); 
	printf("Tree Nodes in Post order\n\n");
	printTree(syntaxTree);

	char* symbolfile;
	int fnlen = strcspn(pgm, ".");
	symbolfile = (char*)calloc(fnlen + 8, sizeof(char));
	strncpy(symbolfile, pgm,fnlen);
	strcat(symbolfile, ".symbol");
	FILE* sf = fopen(symbolfile, "w+");
	printSymbol(sf);
	fclose(sf);
	fclose(source);

 }