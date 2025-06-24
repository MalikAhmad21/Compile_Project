#include <stdio.h>
#include "ast.h"
#include "semantic.h"
#include "codegen.h"
#include "symbol.h"

extern int yyparse();
extern ASTNode* root;

int main(int argc, char** argv) {
    if (yyparse() == 0) {
        printf("Parsing Successful!\n\n--- AST ---\n");
        printAST(root, 0);

        printf("\n--- Semantic Analysis ---\n");
        performSemanticAnalysis(root);

        printf("\n--- Symbol Table ---\n");
        printSymbolTable();

        printf("\n--- Intermediate Code (IR) ---\n");
        generateIntermediateCode(root);  

        printf("\n--- Generating Real Assembly Code ---\n");
        generateCode(root);  

        generateASTDot(root, "ast.dot");
        generateSymbolTableDot("symtable.dot");
    } else {
        printf("Parsing failed due to syntax errors.\n");
    }
    return 0;
}
