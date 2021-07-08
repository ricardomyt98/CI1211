#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

#include <stdbool.h>
#include "constants.h"

// Structures
typedef enum Generic_symbol_category {
    SYMBOL_VAR,
    SYMBOL_FUNCTION,
    SYMBOL_PROCEDURE,
    SYMBOL_PARAMETER
} Generic_symbol_category;

typedef enum Symbol_variable_type { VAR_UNDEFINED, VAR_INT, VAR_BOOL } Symbol_variable_type;

typedef enum Symbol_param_category { PARAM_REF, PARAM_COP } Symbol_param_category;

typedef struct Generic_symbol_structure Generic_symbol_structure;
struct Generic_symbol_structure {
    int id;

    // Symbol base information
    char label[TOKEN_SIZE];
    Generic_symbol_category category;
    int lexicon_lvl;

    // Symbol variable information
    Symbol_variable_type variable_type;
    int variable_offset;

    // Symbol parameter information
    Symbol_param_category param_category;
    Generic_symbol_structure *reference_in_procedure;

    // Symbol function information
    Symbol_variable_type function_return_type;
    int function_offset;

    // Symbol procedure and function information
    int MEPA_label;
    Generic_symbol_structure *parameters;
    int num_parameters;
};

typedef struct Symbol_table {
    Generic_symbol_structure* symbol_list;
    int symbol_list_allocation_size;
    int num_symbol_list_elements;
} Symbol_table;

// Functions
Symbol_table create_symbol_table();
Generic_symbol_structure create_symbol_variable(char* label, Symbol_variable_type type, int lexicon_lvl, int offset);
Generic_symbol_structure create_symbol_procedure(char* label, int lexicon_lvl);
Generic_symbol_structure create_symbol_function(char* label, int lexicon_lvl, int offset);
Generic_symbol_structure create_symbol_parameter(char* label, int lexicon_lvl, int offset, Symbol_param_category category);
Generic_symbol_structure *add_element_to_symbol_table(Symbol_table* sym_tab, Generic_symbol_structure element);
void set_symbol_type(Symbol_table *sym_tab, Symbol_variable_type type);
void set_symbol_offset(Symbol_table *sym_tab);
void print_symbol_table_elements(Symbol_table sym_tab);
bool search_if_exists(Symbol_table sym_tab, char *var_name, int lexicon_lvl);
Generic_symbol_structure search_in_symbol_table(char *var_name, Symbol_table sym_tab);
void delete_invisible_symbols(Symbol_table *sym_tab, int new_lexicon_lvl);

#endif
