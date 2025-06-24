%{
#include "tiny.tab.h"
#include <string.h>
%}

%%
[ \t\n]+                  ; // Skip whitespace
"if"                     return IF;
"else"                   return ELSE;
"int"                    return INT;

[0-9]+                   { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*   { yylval.sval = strdup(yytext); return IDENTIFIER; }

"=="                     return EQ;
"="                      return ASSIGN;
"("                      return LPAREN;
")"                      return RPAREN;
"{"                      return LBRACE;
"}"                      return RBRACE;
"+"                      return PLUS;
"-"                      return MINUS;
"*"                      return MULT;
"/"                      return DIV;
";"                      return SEMICOLON;

.                        { printf("Unknown character: %s\n", yytext); return ERROR; }

%%
int yywrap() {
    return 1;
}
