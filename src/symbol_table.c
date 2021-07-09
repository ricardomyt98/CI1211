#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "compiler.h"
#include "symbol_table.h"

Symbol_table create_symbol_table(){
    Symbol_table new_symbol_table = {
        .symbol_list = NULL,
        .symbol_list_allocation_size = 0,
        .num_symbol_list_elements = 0
    };
    
    return new_symbol_table;
}

Generic_symbol_structure create_symbol_variable(char* label, Symbol_variable_type type, int lexicon_lvl, int offset) {
    DEBUG("START create_symbol_variable\n");
    Generic_symbol_structure new_symbol = {
        .lexicon_lvl = lexicon_lvl,
        .category = SYMBOL_VAR,
        .variable_type = VAR_UNDEFINED,
        .variable_offset = offset
    };    
    strncpy(new_symbol.label, label, TOKEN_SIZE);

    DEBUG("END create_symbol_variable\n");

    return new_symbol;
}

Generic_symbol_structure create_symbol_procedure(char* label, int lexicon_lvl) {
    Generic_symbol_structure new_symbol = {
        .lexicon_lvl = lexicon_lvl,
        .category = SYMBOL_PROCEDURE,
        .MEPA_label = get_num_label(),
        .parameters = (Generic_symbol_structure *)malloc(MIN_SIZE * sizeof(Generic_symbol_structure)),
        .num_parameters = 0
    };
    strncpy(new_symbol.label, label, TOKEN_SIZE);

    DEBUG(".parameters: %p\n", new_symbol.parameters);

    return new_symbol;
}

Generic_symbol_structure create_symbol_function(char* label, int lexicon_lvl, int offset) {
    Generic_symbol_structure new_symbol = {
        .lexicon_lvl = lexicon_lvl,
        .category = SYMBOL_FUNCTION,
        .MEPA_label = get_num_label(),
        .parameters = (Generic_symbol_structure *)malloc(MIN_SIZE * sizeof(Generic_symbol_structure)),
        .num_parameters = 0,
        .function_offset = offset,
        .function_return_type = VAR_UNDEFINED
    };
    strncpy(new_symbol.label, label, TOKEN_SIZE);

    return new_symbol;
}

Generic_symbol_structure create_symbol_parameter(char* label, int lexicon_lvl, int offset, Symbol_param_category category) {
    Generic_symbol_structure new_symbol = {
        .lexicon_lvl = lexicon_lvl,
        .category = SYMBOL_PARAMETER,
        .variable_type = VAR_UNDEFINED,
        .variable_offset = offset,
        .param_category = category,
        .reference_in_procedure = NULL
    };
    strncpy(new_symbol.label, label, TOKEN_SIZE);

    return new_symbol;
}

Generic_symbol_structure *add_element_to_symbol_table(Symbol_table* sym_table, Generic_symbol_structure sym_generic_struct) {
    static int id = 1;

    DEBUG("START add_element_to_symbol_table\n");
    DEBUG("adding symbol '%s'\n", sym_generic_struct.label);
    if (sym_table->symbol_list == NULL) {
        DEBUG("A\n");
        sym_table->symbol_list_allocation_size = 5;
        sym_table->symbol_list = (Generic_symbol_structure*)malloc(sizeof(Generic_symbol_structure) * sym_table->symbol_list_allocation_size);
    }

    if (sym_table->num_symbol_list_elements == sym_table->symbol_list_allocation_size) {
        DEBUG("B\n");
        sym_table->symbol_list_allocation_size <<= 1;
        sym_table->symbol_list = (Generic_symbol_structure*)realloc(sym_table->symbol_list, sizeof(Generic_symbol_structure) * sym_table->symbol_list_allocation_size);
    }

    DEBUG("C\n");
    if (search_if_exists(*sym_table, sym_generic_struct.label, sym_generic_struct.lexicon_lvl)) {
        printf("Duplicate declaration: Variable '%s' already exists!\n", sym_generic_struct.label);
        exit(-14);
    }

    sym_generic_struct.id = id++;
    sym_table->symbol_list[sym_table->num_symbol_list_elements] = sym_generic_struct;
    DEBUG("added .parameters: %p\n", sym_table->symbol_list[sym_table->num_symbol_list_elements].parameters);
    sym_table->num_symbol_list_elements++;
    DEBUG("END add_element_to_symbol_table\n");

    return &sym_table->symbol_list[sym_table->num_symbol_list_elements - 1];
}

void set_symbol_type(Symbol_table *sym_tab, Symbol_variable_type type) {
    int i = sym_tab->num_symbol_list_elements - 1;
    bool should_stop = false;

    while (i >= 0 && !should_stop) {
        switch (sym_tab->symbol_list[i].category) {
            case SYMBOL_VAR:
            case SYMBOL_PARAMETER:
                if (sym_tab->symbol_list[i].variable_type != VAR_UNDEFINED) {
                    should_stop = true;
                    break;
                }

                sym_tab->symbol_list[i].variable_type = type;

                if (sym_tab->symbol_list[i].category == SYMBOL_PARAMETER) {
                    sym_tab->symbol_list[i].reference_in_procedure->variable_type = type;
                }
                break;
            default:
                should_stop = true;
                break;
        }

        i--;
    }
}

void set_symbol_offset(Symbol_table *sym_tab) {
    int i = sym_tab->num_symbol_list_elements - 1;
    int offset = -4;

    while (i >= 0 && sym_tab->symbol_list[i].variable_offset == 0 && (sym_tab->symbol_list[i].category == SYMBOL_PARAMETER || sym_tab->symbol_list[i].category == SYMBOL_FUNCTION)) {
        if (sym_tab->symbol_list[i].category == SYMBOL_PARAMETER) {
            sym_tab->symbol_list[i].variable_offset = offset;
            sym_tab->symbol_list[i].reference_in_procedure->variable_offset = offset;
        } else if (sym_tab->symbol_list[i].category == SYMBOL_FUNCTION) {
            sym_tab->symbol_list[i].function_offset = offset;
        }
        offset--;
        i--;
    }
}

const char *variable_type_string[] = {
    "VAR_UNDEFINED",
    "VAR_INT",
    "VAR_BOOL"
};

const char *param_category_string[] = {
    "PARAM_REF",
    "PARAM_COP"
};

void print_symbol_table_element(Generic_symbol_structure element) {
    switch (element.category) {
        case SYMBOL_VAR:
            printf("Lexicon level: %d; variable: %s; offset: %d, type: %s\n", element.lexicon_lvl, element.label, element.variable_offset, variable_type_string[element.variable_type]);
            break;
        case SYMBOL_PROCEDURE:
            printf("Lexicon level: %d, procedure: %s, MEPA label: R%02d\n", element.lexicon_lvl, element.label, element.MEPA_label);
            break;
        case SYMBOL_FUNCTION:
            printf("Lexicon level: %d, function: %s, MEPA label: R%02d, offset: %d, return type: %s\n", element.lexicon_lvl, element.label, element.MEPA_label, element.function_offset, variable_type_string[element.function_return_type]);
            break;
        case SYMBOL_PARAMETER:
            printf("Lexicon level: %d, parameter: %s, offset: %d, param category: %s, type: %s\n", element.lexicon_lvl, element.label, element.variable_offset, param_category_string[element.param_category], variable_type_string[element.variable_type]);
            break;
    }
}

void print_symbol_table_elements(Symbol_table sym_tab) {
    for (int i = 0; i < sym_tab.num_symbol_list_elements; i++) {
        print_symbol_table_element(sym_tab.symbol_list[i]);
    }
}

bool search_if_exists(Symbol_table sym_tab, char *var_name, int lexicon_lvl) {
    int i = sym_tab.num_symbol_list_elements - 1;

    while (i >= 0) {
        Generic_symbol_structure element = sym_tab.symbol_list[i];
        if (strcmp(element.label, var_name) == 0 && element.lexicon_lvl == lexicon_lvl) { // found the element
            return true;
        }

        i--;
    }

    return false;
}

Generic_symbol_structure search_in_symbol_table(char *var_name, Symbol_table sym_tab) {
    int i = sym_tab.num_symbol_list_elements - 1;

    while (i >= 0) {
        if (strcmp(sym_tab.symbol_list[i].label, var_name) == 0) { // found the element
            return sym_tab.symbol_list[i];
        }

        i--;
    }

    printf("Variable %s not found in the symbol table!!!\n", var_name); // not found

    exit(-1);
}

void delete_invisible_symbols(Symbol_table *sym_tab, int new_lexicon_lvl) {
    int i;
    for (i = sym_tab->num_symbol_list_elements - 1; i >= 0; i--) {
        if (sym_tab->symbol_list[i].category == SYMBOL_PROCEDURE || sym_tab->symbol_list[i].category == SYMBOL_FUNCTION) {
            if (sym_tab->symbol_list[i].lexicon_lvl > new_lexicon_lvl + 1) {
                free(sym_tab->symbol_list[i].parameters);
            } else {
                break;
            }
        }

        if (sym_tab->symbol_list[i].lexicon_lvl <= new_lexicon_lvl) {
            break;
        }
    }
    sym_tab->num_symbol_list_elements = i + 1;
}
