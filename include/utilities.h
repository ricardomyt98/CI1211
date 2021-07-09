#ifndef __UTILITIES__
#define __UTILITIES__

#include "compiler.h"

void token_print_formatter(char *token_identifier);
enum yytokentype flex_printer(Symbol symbol, enum yytokentype token_type, char *token_identifier);

#endif
