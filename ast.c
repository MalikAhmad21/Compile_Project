#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* createNumNode(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_NUM;
    node->num_value = value;
    return node;
}

ASTNode* createVarNode(char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_VAR;
    node->var_name = strdup(name);
    return node;
}

ASTNode* createBinaryNode(char* op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BINOP;
    node->binop.op = strdup(op);
    node->binop.left = left;
    node->binop.right = right;
    return node;
}

ASTNode* createAssignNode(char* name, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGN;
    node->assign.var = strdup(name);
    node->assign.expr = expr;
    return node;
}

ASTNode* createIfElseNode(ASTNode* cond, ASTNode* thenBranch, ASTNode* elseBranch) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_IFELSE;
    node->ifelse.cond = cond;
    node->ifelse.then_branch = thenBranch;
    node->ifelse.else_branch = elseBranch;
    return node;
}

ASTNode* createStmtList(ASTNode* stmt, ASTNode* next) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_STMTLIST;
    node->stmtlist.stmt = stmt;
    node->stmtlist.next = next;
    return node;
}

void printIndent(int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
}

void printAST(ASTNode* node, int indent) {
    if (!node) return;

    printIndent(indent);
    switch (node->type) {
        case NODE_NUM:
            printf("Number: %d\n", node->num_value);
            break;
        case NODE_VAR:
            printf("Variable: %s\n", node->var_name);
            break;
        case NODE_BINOP:
            printf("Binary Operation: %s\n", node->binop.op);
            printAST(node->binop.left, indent + 1);
            printAST(node->binop.right, indent + 1);
            break;
        case NODE_ASSIGN:
            printf("Assignment: %s = \n", node->assign.var);
            printAST(node->assign.expr, indent + 1);
            break;
        case NODE_IFELSE:
            printf("If-Else Condition\n");
            printIndent(indent + 1); printf("Condition:\n");
            printAST(node->ifelse.cond, indent + 2);
            printIndent(indent + 1); printf("Then:\n");
            printAST(node->ifelse.then_branch, indent + 2);
            printIndent(indent + 1); printf("Else:\n");
            printAST(node->ifelse.else_branch, indent + 2);
            break;
        case NODE_STMTLIST:
            printAST(node->stmtlist.stmt, indent);
            printAST(node->stmtlist.next, indent);
            break;
    }
}

void printASTDot(ASTNode* node, FILE* out, int* id, int parent) {
    if (!node) return;

    int curr = (*id)++;
    char label[64];

    switch (node->type) {
        case NODE_NUM:
            sprintf(label, "Number\\n%d", node->num_value);
            break;
        case NODE_VAR:
            sprintf(label, "Variable\\n%s", node->var_name);
            break;
        case NODE_BINOP:
            sprintf(label, "BinOp\\n%s", node->binop.op);
            break;
        case NODE_ASSIGN:
            sprintf(label, "Assign\\n%s", node->assign.var);
            break;
        case NODE_IFELSE:
            sprintf(label, "IfElse");
            break;
        case NODE_STMTLIST:
            sprintf(label, "StmtList");
            break;
        default:
            sprintf(label, "Unknown");
            break;
    }

    fprintf(out, "  node%d [label=\"%s\"];\n", curr, label);
    if (parent >= 0)
        fprintf(out, "  node%d -> node%d;\n", parent, curr);

    switch (node->type) {
        case NODE_BINOP:
            printASTDot(node->binop.left, out, id, curr);
            printASTDot(node->binop.right, out, id, curr);
            break;
        case NODE_ASSIGN:
            printASTDot(node->assign.expr, out, id, curr);
            break;
        case NODE_IFELSE:
            printASTDot(node->ifelse.cond, out, id, curr);
            printASTDot(node->ifelse.then_branch, out, id, curr);
            printASTDot(node->ifelse.else_branch, out, id, curr);
            break;
        case NODE_STMTLIST:
            printASTDot(node->stmtlist.stmt, out, id, curr);
            printASTDot(node->stmtlist.next, out, id, curr);
            break;
        default:
            break;
    }
}

void generateASTDot(ASTNode* node, const char* filename) {
    FILE* out = fopen(filename, "w");
    if (!out) {
        perror("fopen");
        return;
    }

    fprintf(out, "digraph AST {\n");
    int id = 0;
    printASTDot(node, out, &id, -1);
    fprintf(out, "}\n");

    fclose(out);
}
