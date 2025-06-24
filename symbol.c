#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "symbol.h"
#include "ast.h"

Symbol* symbolTable = NULL;

void initSymbolTable() {
    symbolTable = NULL;
}

void addSymbol(char* name) {
    if (symbolExists(name)) return;

    Symbol* sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->value = 0;
    sym->next = symbolTable;
    symbolTable = sym;
}

int symbolExists(char* name) {
    Symbol* curr = symbolTable;
    while (curr) {
        if (strcmp(curr->name, name) == 0)
            return 1;
        curr = curr->next;
    }
    return 0;
}

void printSymbolTable() {
    printf("Symbol Table:\n");
    printf("------------------------\n");
    Symbol* sym = symbolTable;
    while (sym) {
        printf("Name: %s | Value: %d\n", sym->name, sym->value);
        sym = sym->next;
    }
    printf("------------------------\n");
}

void freeSymbolTable() {
    Symbol* curr = symbolTable;
    while (curr) {
        Symbol* temp = curr;
        curr = curr->next;
        free(temp->name);
        free(temp);
    }
    symbolTable = NULL;
}

void generateSymbolTableDot(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("fopen");
        return;
    }

    fprintf(file, "digraph SymbolTable {\n");
    fprintf(file, "  node [shape=record];\n");

    Symbol* sym = symbolTable;
    int id = 0;
    while (sym) {
        fprintf(file, "  sym%d [label=\"{<f0> Name: %s | <f1> Value: %d}\"];\n", id, sym->name, sym->value);
        sym = sym->next;
        id++;
    }

    fprintf(file, "}\n");
    fclose(file);
}


static FILE* irFile = NULL;
static int tempVarCounter = 0;

void generateIR(ASTNode* node);

void writeIR(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(irFile, format, args);
    va_end(args);
}

void generateIntermediateCode(ASTNode* node) {
    irFile = fopen("ir.dot", "w");
    if (!irFile) {
        printf("Error: Cannot open ir.dot\n");
        return;
    }

    fprintf(irFile, "digraph IR {\n");
    fprintf(irFile, "  node [shape=box];\n");
    generateIR(node);
    fprintf(irFile, "}\n");
    fclose(irFile);

    
    printf("Intermediate Code (IR):\n");
    FILE* irRead = fopen("ir.dot", "r");
    if (irRead) {
        char line[256];
        while (fgets(line, sizeof(line), irRead)) {
            printf("%s", line);
        }
        fclose(irRead);
    }
}

void generateIR(ASTNode* node) {
    static int blockId = 0;
    if (!node) return;

    switch (node->type) {
        case NODE_ASSIGN:
            if (node->assign.expr->type == NODE_BINOP) {
                char left[32], right[32], temp[128];
                sprintf(left, "%s", node->assign.expr->binop.left->var_name);
                sprintf(right, "%s", node->assign.expr->binop.right->var_name);
                snprintf(temp, sizeof(temp), "%s = %s %s %s",
                    node->assign.var,
                    left,
                    node->assign.expr->binop.op,
                    right);
                fprintf(irFile, "  B%d [label=\"%s\"];\n", blockId++, temp);
            } else if (node->assign.expr->type == NODE_NUM) {
                fprintf(irFile, "  B%d [label=\"%s = %d\"];\n",
                        blockId++,
                        node->assign.var,
                        node->assign.expr->num_value);
            }
            break;

        case NODE_IFELSE:
            fprintf(irFile, "  B%d [label=\"if ", blockId++);
            generateIR(node->ifelse.cond);
            fprintf(irFile, "\"];\n");

            fprintf(irFile, "  B%d [label=\"then\"];\n", blockId++);
            generateIR(node->ifelse.then_branch);

            fprintf(irFile, "  B%d [label=\"else\"];\n", blockId++);
            generateIR(node->ifelse.else_branch);
            break;

        case NODE_VAR:
            fprintf(irFile, "%s", node->var_name);
            break;

        case NODE_NUM:
            fprintf(irFile, "%d", node->num_value);
            break;

        case NODE_STMTLIST:
            generateIR(node->stmtlist.stmt);
            generateIR(node->stmtlist.next);
            break;

        default:
            break;
    }
}
