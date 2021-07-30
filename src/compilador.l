%{

#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "compilador.tab.h"
#include "utilities.h"

int nl = 1;

%}

pulo_linha [\n]

descarte [\t ]+

identifier [a-zA-Z][a-zA-Z0-9]*

number [0-9]+

debug "(*DEBUG*)"

printsymbols "(*PRINT_SYMBOL_TABLE*)"

comentario "(*".*"*)"



%%

{pulo_linha} {nl++;}

{descarte}

{debug} { geraCodigo(NULL, "DEBUG"); }

{printsymbols} { print_symbol_table_elements(comp_struct.sym_table); }

{comentario}

program { return flex_printer(S_PROGRAM, T_PROGRAM, "program"); }
var { return flex_printer(S_VAR, T_VAR, "var"); }
var_type { return flex_printer(S_VAR_TYPE, T_VAR_TYPE, "var_type"); }
begin { return flex_printer(S_BEGIN, T_BEGIN, "begin"); }
end { return flex_printer(S_END, T_END, "end"); }

function { return flex_printer(S_FUNCTION, T_FUNCTION, "function"); }
procedure { return flex_printer(S_PROCEDURE, T_PROCEDURE, "procedure"); }
read { return flex_printer(S_READ, T_READ, "read"); }
write { return flex_printer(S_WRITE, T_WRITE, "write"); }

\, { return flex_printer(S_COMMA, T_COMMA, "comma"); }
\; { return flex_printer(S_SEMICOLON, T_SEMICOLON, "semicolon"); }
\: { return flex_printer(S_COLON, T_COLON, "colon"); }
\. { return flex_printer(S_DOT, T_DOT, "dot"); }
\( { return flex_printer(S_OPEN_PARENTHESIS, T_OPEN_PARENTHESIS, "open_parenthesis"); }
\) { return flex_printer(S_CLOSE_PARENTHESIS, T_CLOSE_PARENTHESIS, "close_parenthesis"); }

\:= { return flex_printer(S_ASSIGNMENT, T_ASSIGNMENT, "assignment"); }
\>= { return flex_printer(S_GREATER_EQ, T_GREATER_EQ, "greater_than_or_equal"); }
\<= { return flex_printer(S_LESS_EQ, T_LESS_EQ, "less_than_or_equal"); }
\> { return flex_printer(S_GREATER, T_GREATER, "greater"); }
\< { return flex_printer(S_LESS, T_LESS, "less"); }
\= { return flex_printer(S_EQ, T_EQ, "equal"); }
\!= { return flex_printer(S_NEQ, T_NEQ, "not_equal"); }

if { return flex_printer(S_IF, T_IF, "if"); }
then { return flex_printer(S_THEN, T_THEN, "then"); }
else { return flex_printer(S_ELSE, T_ELSE, "else"); }

do { return flex_printer(S_DO, T_DO, "do"); }
while { return flex_printer(S_WHILE, T_WHILE, "while"); }

repeat { return flex_printer(S_REPEAT, T_REPEAT, "repeat"); }
until { return flex_printer(S_UNTIL, T_UNTIL, "until"); }

and { return flex_printer(S_AND, T_AND, "and"); }
or { return flex_printer(S_OR, T_OR, "or"); }
not { return flex_printer(S_NOT, T_NOT, "not"); }
true { return flex_printer(S_TRUE, T_TRUE, "true"); }
false { return flex_printer(S_FALSE, T_FALSE, "false"); }

div { return flex_printer(S_DIV, T_DIV, "div"); }
\/ { return flex_printer(S_DIV, T_DIV, "div"); }
\+ { return flex_printer(S_SUM, T_SUM, "sum"); }
\- { return flex_printer(S_SUB, T_SUB, "sub"); }
\* { return flex_printer(S_MUL, T_MUL, "mul"); }

{number} { return flex_printer(S_NUMBER, T_NUMBER, "number"); }
{identifier} { return flex_printer(S_IDENTIFIER, T_IDENTIFIER, "identifier"); }

%%
