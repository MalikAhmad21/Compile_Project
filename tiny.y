%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
void yyerror(const char *s);
ASTNode* root;
%}

%union {
    int ival;
    char* sval;
    struct ASTNode* node;
}

%token <sval> IDENTIFIER
%token <ival> NUMBER
%token IF ELSE INT
%token EQ ASSIGN
%token PLUS MINUS MULT DIV
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON
%token ERROR

%type <node> program stmt block expr decl stmt_list

%start program

%%

program:
    stmt_list                    { root = $1; }
    ;

stmt_list:
    stmt                         { $$ = createStmtList($1, NULL); }
  | stmt stmt_list               { $$ = createStmtList($1, $2); }
    ;

stmt:
    decl                         { $$ = $1; }
  | expr SEMICOLON               { $$ = $1; }
  | IF LPAREN expr RPAREN block ELSE block
                                { $$ = createIfElseNode($3, $5, $7); }
    ;

block:
    LBRACE stmt_list RBRACE      { $$ = $2; }
    ;

decl:
    INT IDENTIFIER ASSIGN expr SEMICOLON
                                { $$ = createAssignNode($2, $4); }
    ;

expr:
    expr PLUS expr              { $$ = createBinaryNode("+", $1, $3); }
  | expr MINUS expr             { $$ = createBinaryNode("-", $1, $3); }
  | expr MULT expr              { $$ = createBinaryNode("*", $1, $3); }
  | expr DIV expr               { $$ = createBinaryNode("/", $1, $3); }
  | IDENTIFIER                  { $$ = createVarNode($1); }
  | NUMBER                      { $$ = createNumNode($1); }
  | LPAREN expr RPAREN          { $$ = $2; }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}
