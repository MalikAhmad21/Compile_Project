#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUM,
    NODE_VAR,
    NODE_BINOP,
    NODE_ASSIGN,
    NODE_IFELSE,
    NODE_STMTLIST
} NodeType;

typedef struct ASTNode {
    NodeType type;

    union {
        int num_value;
        char* var_name;

        struct {
            char* op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binop;

        struct {
            char* var;
            struct ASTNode* expr;
        } assign;

        struct {
            struct ASTNode* cond;
            struct ASTNode* then_branch;
            struct ASTNode* else_branch;
        } ifelse;

        struct {
            struct ASTNode* stmt;
            struct ASTNode* next;
        } stmtlist;
    };
} ASTNode;

ASTNode* createNumNode(int value);
ASTNode* createVarNode(char* name);
ASTNode* createBinaryNode(char* op, ASTNode* left, ASTNode* right);
ASTNode* createAssignNode(char* name, ASTNode* expr);
ASTNode* createIfElseNode(ASTNode* cond, ASTNode* thenBranch, ASTNode* elseBranch);
ASTNode* createStmtList(ASTNode* stmt, ASTNode* next);
void printAST(ASTNode* node, int indent);
void generateASTDot(ASTNode* node, const char* filename);

#endif
