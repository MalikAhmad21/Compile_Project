#include <stdio.h>
#include "semantic.h"
#include "symbol.h"

void checkNode(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_ASSIGN:
            if (!symbolExists(node->assign.var)) {
                printf("Semantic Error: Variable '%s' not declared.\n", node->assign.var);
            }
            checkNode(node->assign.expr);
            break;
        case NODE_VAR:
            if (!symbolExists(node->var_name)) {
                printf("Semantic Error: Variable '%s' not declared.\n", node->var_name);
            }
            break;
        case NODE_BINOP:
            checkNode(node->binop.left);
            checkNode(node->binop.right);
            break;
        case NODE_IFELSE:
            checkNode(node->ifelse.cond);
            checkNode(node->ifelse.then_branch);
            checkNode(node->ifelse.else_branch);
            break;
        case NODE_STMTLIST:
            checkNode(node->stmtlist.stmt);
            checkNode(node->stmtlist.next);
            break;
        default:
            break;
    }
}

void performSemanticAnalysis(ASTNode* root) {
    initSymbolTable();
    ASTNode* cur = root;
    while (cur && cur->type == NODE_STMTLIST) {
        if (cur->stmtlist.stmt->type == NODE_ASSIGN) {
            addSymbol(cur->stmtlist.stmt->assign.var);
        }
        cur = cur->stmtlist.next;
    }
    checkNode(root);
}
