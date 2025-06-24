#ifndef SYMBOL_H
#define SYMBOL_H

#include "ast.h"  

typedef struct Symbol {
    char* name;
    int value;
    struct Symbol* next;
} Symbol;

extern Symbol* symbolTable;

void initSymbolTable();
void addSymbol(char* name);
int symbolExists(char* name);
void printSymbolTable();
void freeSymbolTable();
void generateSymbolTableDot(const char* filename);
void generateIntermediateCode(ASTNode* node);  

#endif
