#ifndef __COMPILER__
#define __COMPILER__

#include "constants.h"
#include "symbol_table.h"

typedef enum Symbol {
    // Keywords
    S_PROGRAM,
    S_VAR,
    S_VAR_TYPE,
    S_BEGIN,
    S_END,

    // Identifier types
    S_PROCEDURE,
    S_FUNCTION,
    S_READ,
    S_WRITE,

    // Separators
    S_COMMA,
    S_SEMICOLON,
    S_COLON,
    S_DOT,
    S_OPEN_PARENTHESIS,
    S_CLOSE_PARENTHESIS,

    // Conditional and comparison
    S_IF,
    S_THEN,
    S_ELSE,

    // Loops
    S_DO,
    S_WHILE,

    // Logical symbols
    S_AND,
    S_OR,
    S_NOT,
    S_GREATER,
    S_GREATER_EQ,
    S_LESS,
    S_LESS_EQ,
    S_EQ,
    S_NEQ,
    S_TRUE,
    S_FALSE,

    // Math
    S_SUM,
    S_SUB,
    S_MUL,
    S_DIV,

    // Program
    S_ASSIGNMENT,
    S_NUMBER,
    S_IDENTIFIER
} Symbol;

typedef enum Operator {
    OPER_SUM,
    OPER_SUB,
    OPER_MUL,
    OPER_DIV,
    OPER_GEQ,
    OPER_GT,
    OPER_LE,
    OPER_LEQ,
    OPER_EQ,
    OPER_NEQ,
    OPER_AND,
    OPER_OR,
    OPER_NOT
} Operator;

typedef enum Factor {
    FACT_BOOL,
    FACT_INT
} Factor;

typedef enum Operator_factor {
    OPER,
    FACT
} Operator_factor;

typedef struct Operation_structure {
    Operator_factor category;
    Factor fact_type;
    Symbol fact_symbol;
    Operator oper_type;
} Operation_structure;

typedef enum Stack_type {
    WHILE,
    IF,
    BLOCK
} Stack_type;

typedef struct Stack_label {
    Stack_type type;
    int first_label;
    int second_label;
} Stack_label;

typedef struct Compiler_structure {
    Symbol_table sym_table;
    int lexicon_lvl;
    int variable_counter_list[MIN_SIZE];
    int parameter_counter_list[MIN_SIZE];
    char assignment_temp[TOKEN_SIZE];
    char last_identifier[TOKEN_SIZE];
    Operation_structure expression_elements[MIN_SIZE][MIN_SIZE];
    int expression_elements_num[MIN_SIZE];
    int current_expression;
    Stack_label stack[MIN_SIZE];
    int stack_head;
    Generic_symbol_structure *declarations[MIN_SIZE];
    int declarations_num;
    Generic_symbol_structure call_targets[MIN_SIZE];
    int call_parameters[MIN_SIZE];
    int current_call;
} Compiler_structure;

/* -------------------------------------------------------------------
 * global variables
 * ------------------------------------------------------------------- */

extern Symbol simbolo, relacao;
extern char token[TOKEN_SIZE];
extern int nivel_lexico;
extern int desloc;
extern int nl;
extern Compiler_structure comp_struct;

/* -------------------------------------------------------------------
 * global prototypes
 * ------------------------------------------------------------------- */

void geraCodigo(char *, char *);
int yylex();
void yyerror(const char *s);
void AMEM();
void DMEM();
void CRCT();
void ARMZ(char target[TOKEN_SIZE]);
void ARMI(char target[TOKEN_SIZE]);
void CRVL();
void CRVI();
void CREN();
void SOMA();
void SUBT();
void MULT();
void DIVI();
void CMMA();
void CMAG();
void CMME();
void CMEG();
void CMIG();
void CMDG();
void CONJ();
void DISJ();
void NEGA();
void DSVF1();
void DSVF2();
void DSVS1();
void DSVS2();
void ENPR();
void RTPR();
void CHPR();
void LEIT();
void IMPR();
Compiler_structure variable_initializer();
void add_variable_to_symbol_table();
void add_procedure_to_symbol_table();
void add_function_to_symbol_table();
void add_procedure_param_cop_to_symbol_table();
void add_procedure_param_ref_to_symbol_table();
Symbol_variable_type check_token_variable_type(char token[TOKEN_SIZE]);
void compiler_set_symbol_type();
void compiler_set_function_type();
void init_expression();
void finish_expression();
void add_expression_element(Operation_structure operation);
Factor evaluate_expression();
void evaluate_assignment_expression();
void evaluate_conditional_expression();
int get_num_label();
void push_stack(Stack_type type);
void pop_stack();
void print_label(int label);
void print_current_declaration_label();
void print_first_stack_label();
void print_second_stack_label();
void allocate_print_while_stack_label();
void free_print_while_stack_label();
void compiler_set_symbol_offset();
void begin_procedure();
void end_procedure();
void begin_function();
void end_function();
void verify_if_call_parameters_match();
void begin_call();
void end_call();
void increment_call_parameter();
void evaluate_parameter();
void store();
void store_for_read();
void store_for_assignment();
void load();

#endif