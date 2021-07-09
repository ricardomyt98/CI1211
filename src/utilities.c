#include "utilities.h"

#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "compilador.tab.h"

extern char *yytext;

enum yytokentype flex_printer(Symbol symbol, enum yytokentype token_type, char *token_identifier) {  // Symbol - Flex | yytokentype - bison
    if (symbol == S_ASSIGNMENT) {
        strncpy(comp_struct.assignment_temp, token, TOKEN_SIZE);
    }

    strncpy(token, yytext, TOKEN_SIZE);
    token_print_formatter(token_identifier);

    if (symbol == S_IDENTIFIER) {
        strncpy(comp_struct.last_identifier, token, TOKEN_SIZE);
    }

    return token_type;
}

void token_print_formatter(char *token_identifier) {
    int white_space_formatter = 20 - strlen(token_identifier);

    printf("%s", token_identifier);
    for (int i = 0; i < white_space_formatter; i++) {
        printf(" ");
    }
    printf("# %s\n", token);
}

/*
simbolo = simb_identificador;
strncpy (token, yytext, TOKEN_SIZE);
IMPRIME(" ident  ");
return IDENT;
*/
