%{

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "symbol_table.h"
#include "utilities.h"

Compiler_structure comp_struct;

%}

%token
    // Keywords
    T_PROGRAM
    T_VAR
    T_VAR_TYPE
    T_BEGIN
    T_END

    // Identifier types
    T_FUNCTION
    T_PROCEDURE
    T_READ
    T_WRITE

    // Separators
    T_COMMA
    T_SEMICOLON
    T_COLON
    T_DOT
    T_OPEN_PARENTHESIS
    T_CLOSE_PARENTHESIS

    // Conditional and comparison
    T_IF
    T_THEN
    T_ELSE

    // Loops
    T_DO
    T_WHILE
    T_REPEAT
    T_UNTIL

    // Logical symbols
    T_AND
    T_OR
    T_NOT
    T_GREATER
    T_GREATER_EQ
    T_LESS
    T_LESS_EQ
    T_EQ
    T_NEQ
    T_TRUE
    T_FALSE

    // Math
    T_SUM
    T_SUB
    T_MUL
    T_DIV

    // Program
    T_ASSIGNMENT
    T_NUMBER
    T_IDENTIFIER

%nonassoc LOWER_THAN_ELSE
%nonassoc T_ELSE

%%

r_program: { comp_struct = variable_initializer(); }
           { geraCodigo (NULL, "INPP"); }
           T_PROGRAM T_IDENTIFIER T_OPEN_PARENTHESIS r_identifier_list T_CLOSE_PARENTHESIS T_SEMICOLON
           r_block T_DOT
           { geraCodigo (NULL, "PARA"); }
           //{ print_symbol_table_elements(comp_struct.sym_table); } // to debug
;

r_identifier_list: r_identifier_list T_COMMA r_identifier | r_identifier
;

r_identifier: T_IDENTIFIER { }
;

r_block: r_var { AMEM(); push_stack(BLOCK); DSVS1(); }
         r_procedure_and_functions
         { print_first_stack_label(); pop_stack(); }
         r_compound_command
         { DMEM(); }
;

r_var: T_VAR r_var_declaration { }
     | %empty
;

r_var_declaration: r_var_declaration_list T_SEMICOLON r_var_declaration
                 | r_var_declaration_list T_SEMICOLON
;

r_var_declaration_list: r_var_identifier_list T_COLON r_var_type { compiler_set_symbol_type(); }
;

r_var_identifier_list: r_var_identifier_list T_COMMA T_IDENTIFIER { add_variable_to_symbol_table(); }
                     | T_IDENTIFIER { add_variable_to_symbol_table(); }
;

r_var_type: T_IDENTIFIER
;

r_procedure_and_functions: r_procedure_declaration T_SEMICOLON r_procedure_and_functions
                         | r_function_declaration T_SEMICOLON r_procedure_and_functions
                         | r_procedure_declaration
                         | r_function_declaration
                         | %empty
;

r_function_declaration: T_FUNCTION T_IDENTIFIER { begin_function(); print_current_declaration_label(); ENPR(); } r_function_param_declaration T_COLON T_IDENTIFIER { compiler_set_function_type(); } T_SEMICOLON r_block { RTPR(); end_function(); }
;

r_function_param_declaration: T_OPEN_PARENTHESIS r_function_param_list T_CLOSE_PARENTHESIS { compiler_set_symbol_offset(); }
                            | %empty
;

r_function_param_list: r_function_params | r_function_params T_SEMICOLON r_function_param_list
                     | %empty
;

r_function_params: r_function_param_cop_identifier_list T_COLON r_function_params_type
                  | T_VAR r_function_param_ref_identifier_list T_COLON r_function_params_type
;

r_function_param_cop_identifier_list: T_IDENTIFIER { add_procedure_param_cop_to_symbol_table(); } T_COMMA r_function_param_cop_identifier_list
                                    | T_IDENTIFIER { add_procedure_param_cop_to_symbol_table(); }
;

r_function_params_type: T_IDENTIFIER { compiler_set_symbol_type(); }
;

r_function_param_ref_identifier_list: T_IDENTIFIER { add_procedure_param_ref_to_symbol_table(); } T_COMMA r_function_param_ref_identifier_list
                                    | T_IDENTIFIER { add_procedure_param_ref_to_symbol_table(); }
;

r_procedure_declaration: T_PROCEDURE T_IDENTIFIER { begin_procedure(); print_current_declaration_label(); ENPR(); } r_procedure_param_declaration T_SEMICOLON r_block { RTPR(); end_procedure(); }
;

r_procedure_param_declaration: T_OPEN_PARENTHESIS r_procedure_param_list T_CLOSE_PARENTHESIS { compiler_set_symbol_offset(); }
                             | %empty
;

r_procedure_param_list: r_procedure_params | r_procedure_params T_SEMICOLON r_procedure_param_list
                      | %empty
;

r_procedure_params: r_procedure_param_cop_identifier_list T_COLON r_procedure_params_type
                  | T_VAR r_procedure_param_ref_identifier_list T_COLON r_procedure_params_type
;

r_procedure_param_cop_identifier_list: T_IDENTIFIER { add_procedure_param_cop_to_symbol_table(); } T_COMMA r_procedure_param_cop_identifier_list
                                     | T_IDENTIFIER { add_procedure_param_cop_to_symbol_table(); }
;

r_procedure_param_ref_identifier_list: T_IDENTIFIER { add_procedure_param_ref_to_symbol_table(); } T_COMMA r_procedure_param_ref_identifier_list
                                     | T_IDENTIFIER { add_procedure_param_ref_to_symbol_table(); }
;

r_procedure_params_type: T_IDENTIFIER { compiler_set_symbol_type(); }
;

r_compound_command: T_BEGIN r_commands T_END
;

r_commands: r_command T_SEMICOLON r_commands | r_command | r_command T_SEMICOLON | %empty
;

r_command: r_assignment | r_while_loop | r_if | r_procedure_call | r_function_call | r_read | r_write | r_repeat_loop
;

r_repeat_loop: T_REPEAT { push_stack(REPEAT); print_first_stack_label(); } r_repeat_commands T_UNTIL { init_expression(); } r_expression { evaluate_conditional_expression(); DSVF1(); pop_stack(); }
;

r_repeat_commands: r_commands | r_compound_command
;

r_read: T_READ T_OPEN_PARENTHESIS r_read_identifiers T_CLOSE_PARENTHESIS
;

r_read_identifiers: T_IDENTIFIER { LEIT(); store_for_read(); }
                  | T_IDENTIFIER { LEIT(); store_for_read(); } T_COMMA r_read_identifiers
;

r_write: T_WRITE T_OPEN_PARENTHESIS { init_expression(); } r_write_identifier_list { finish_expression(); } T_CLOSE_PARENTHESIS
;

r_write_identifier_list: r_write_identifier T_COMMA r_write_identifier_list
                       | r_write_identifier
;

r_write_identifier: T_IDENTIFIER { load(); IMPR(); }
                  | T_NUMBER { CRCT(); IMPR(); }
                  | r_function_call { IMPR(); }
;

r_if: T_IF { push_stack(IF); init_expression(); } r_expression { evaluate_conditional_expression(); DSVF1(); } T_THEN r_if_then { print_first_stack_label(); } r_else { print_second_stack_label(); pop_stack(); }
;

r_else: T_ELSE r_if_else
      | %empty %prec LOWER_THAN_ELSE

r_if_then: r_compound_command { DSVS2(); }
         | r_command { DSVS2(); }
;

r_if_else: r_compound_command
         | r_command
;

r_while_loop: T_WHILE { allocate_print_while_stack_label(); init_expression(); } r_expression { evaluate_conditional_expression(); DSVF2(); } T_DO r_while_statements { DSVS1(); free_print_while_stack_label(); }
;

r_while_statements: r_compound_command | r_command
;

r_assignment: T_IDENTIFIER T_ASSIGNMENT { init_expression(); } r_expression { evaluate_assignment_expression(); store_for_assignment(); }
;

r_expression: r_expression T_GREATER r_sub_expression { CMMA(); }
            | r_expression T_GREATER_EQ r_sub_expression { CMAG(); }
            | r_expression T_LESS r_sub_expression { CMME(); }
            | r_expression T_LESS_EQ r_sub_expression { CMEG(); }
            | r_expression T_EQ r_sub_expression { CMIG(); }
            | r_expression T_NEQ r_sub_expression { CMDG(); }
            | r_sub_expression
;

r_sub_expression: r_sub_expression T_SUM r_term { SOMA(); }
                | r_sub_expression T_SUB r_term { SUBT(); }
                | r_sub_expression T_OR r_term { DISJ(); }
                | r_term
;

r_term: r_term T_MUL r_factor { MULT(); }
      | r_term T_DIV r_factor { DIVI(); }
      | r_term T_AND r_factor { CONJ(); }
      | r_factor
;

r_factor: T_NUMBER { CRCT(); }
        | T_TRUE { CRCT(); }
        | T_FALSE { CRCT(); }
        | T_IDENTIFIER { load(); }
        | r_function_call
        | T_NOT r_factor { NEGA(); }
        | T_OPEN_PARENTHESIS r_expression T_CLOSE_PARENTHESIS
;

r_procedure_call: T_IDENTIFIER { begin_call(); } r_procedure_call_parameters { CHPR(); end_call(); }
;

r_procedure_call_parameters: T_OPEN_PARENTHESIS  r_procedure_call_parameter_list T_CLOSE_PARENTHESIS
                           | %empty
;

r_procedure_call_parameter_list: r_procedure_call_parameter T_COMMA r_procedure_call_parameter_list
                               | r_procedure_call_parameter
                               | %empty
;

r_procedure_call_parameter: { init_expression(); } r_expression { evaluate_parameter(); increment_call_parameter(); }
;

r_function_call: T_IDENTIFIER { begin_call(); } r_function_call_parameters { CHPR(); end_call(); }
;

r_function_call_parameters: T_OPEN_PARENTHESIS  r_function_call_parameter_list T_CLOSE_PARENTHESIS
                          | %empty
;

r_function_call_parameter_list: r_function_call_parameter T_COMMA r_function_call_parameter_list
                              | r_function_call_parameter
                              | %empty
;

r_function_call_parameter: { init_expression(); } r_expression { evaluate_parameter(); increment_call_parameter(); }
;

%%

int main(int argc, char** argv) {
    FILE* fp;
    extern FILE* yyin;

    if (argc < 2 || argc > 2) {
        printf("usage compilador <arq>a %d\n", argc);
        return (-1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("usage compilador <arq>b\n");
        return (-1);
    }

// Inicia a Tabela de Símbolos
    yyin = fp;
    yyparse();

    return 0;
}
