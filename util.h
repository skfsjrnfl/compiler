#ifndef _UTIL_H_
#define _UTIL_H_

void printToken(TokenType token, const char* tokenstring);

TreeNode* newStateNode(StateKind);
TreeNode* newExprNode(StateKind);

char* copystring(char*);
void printTree(TreeNode*);
#endif