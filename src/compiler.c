#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "constants.h"
#include "compiler.h"

/* -------------------------------------------------------------------
 * global variables
 * ------------------------------------------------------------------- */
Symbol simbolo, relacao;
char token[TOKEN_SIZE];
FILE *fp = NULL;

void geraCodigo(char *rot, char *comando) {
    if (fp == NULL) {
        fp = fopen("MEPA", "w");
    }

    if (rot == NULL) {
        fprintf(fp, "     %s\n", comando);
        fflush(fp);
    } else {
        fprintf(fp, "%s: %s\n", rot, comando);
        fflush(fp);
    }
}

int imprimeErro(char *erro) {
    fprintf(stderr, "Erro na linha %d - %s\n", nl, erro);
    exit(-1);
}

void AMEM() {
    char str[100];

    sprintf(str, "AMEM %d", comp_struct.variable_counter_list[comp_struct.lexicon_lvl]);

    geraCodigo(NULL, str);
}

void AMEM_function() {
    geraCodigo(NULL, "AMEM 1");
}

void DMEM() {
    char str[100];

    sprintf(str, "DMEM %d", comp_struct.variable_counter_list[comp_struct.lexicon_lvl]);

    comp_struct.variable_counter_list[comp_struct.lexicon_lvl] = 0;

    geraCodigo(NULL, str);
}

void CRCT() {
    const char *value = token;
    Factor value_type = FACT_INT;

    if (strncmp(value, "true", TOKEN_SIZE) == 0) {
        value = "1";
        value_type = FACT_BOOL;
    }

    if (strncmp(value, "false", TOKEN_SIZE) == 0) {
        value = "0";
        value_type = FACT_BOOL;
    }

    char str[100];
    sprintf(str, "CRCT %s", value);
    geraCodigo(NULL, str);

    add_expression_element((Operation_structure){ .category = FACT, .fact_type = value_type, .fact_symbol = S_NUMBER });
}

void ARMZ(char target[TOKEN_SIZE]) {
    Generic_symbol_structure token_var_struct = search_in_symbol_table(target, comp_struct.sym_table);

    if (token_var_struct.category == SYMBOL_FUNCTION) {
        if (comp_struct.declarations_num <= 0) {
            printf("You can only assign a value to a function inside its body!\n");
            exit(-25);
        } else {
            Generic_symbol_structure *current_pof = comp_struct.declarations[comp_struct.declarations_num - 1];
            if (token_var_struct.id != current_pof->id) {
                printf("You can only assign a value to a function inside its body!\n");
                exit(-24);
            }
        }
    }

    char str[100];

    int offset = token_var_struct.category == SYMBOL_FUNCTION ? token_var_struct.function_offset : token_var_struct.variable_offset;
    sprintf(str, "ARMZ %d, %d", token_var_struct.lexicon_lvl, offset);
    geraCodigo(NULL, str);
}

void ARMI(char target[TOKEN_SIZE]) {
    Generic_symbol_structure token_var_struct = search_in_symbol_table(target, comp_struct.sym_table);
    char str[100];

    int offset = token_var_struct.category == SYMBOL_FUNCTION ? token_var_struct.function_offset : token_var_struct.variable_offset;
    sprintf(str, "ARMI %d, %d", token_var_struct.lexicon_lvl, offset);
    geraCodigo(NULL, str);
}

void CRVL() {
    Generic_symbol_structure token_var_struct = search_in_symbol_table(comp_struct.last_identifier, comp_struct.sym_table);
    char str[100];

    sprintf(str, "CRVL %d, %d", token_var_struct.lexicon_lvl, token_var_struct.variable_offset);
    geraCodigo(NULL, str);

    Factor fact_type = token_var_struct.variable_type == VAR_INT ? FACT_INT : FACT_BOOL;
    add_expression_element((Operation_structure){ .category = FACT, .fact_type = fact_type, .fact_symbol = S_IDENTIFIER });
}

void CRVI() {
    Generic_symbol_structure token_var_struct = search_in_symbol_table(comp_struct.last_identifier, comp_struct.sym_table);
    char str[100];

    sprintf(str, "CRVI %d, %d", token_var_struct.lexicon_lvl, token_var_struct.variable_offset);
    geraCodigo(NULL, str);

    Factor fact_type = token_var_struct.variable_type == VAR_INT ? FACT_INT : FACT_BOOL;
    add_expression_element((Operation_structure){ .category = FACT, .fact_type = fact_type, .fact_symbol = S_IDENTIFIER });
}

void CREN() {
    Generic_symbol_structure token_var_struct = search_in_symbol_table(comp_struct.last_identifier, comp_struct.sym_table);
    char str[100];

    sprintf(str, "CREN %d, %d", token_var_struct.lexicon_lvl, token_var_struct.variable_offset);
    geraCodigo(NULL, str);

    Factor fact_type = token_var_struct.variable_type == VAR_INT ? FACT_INT : FACT_BOOL;
    add_expression_element((Operation_structure){ .category = FACT, .fact_type = fact_type, .fact_symbol = S_IDENTIFIER });
}

void SOMA() {
    geraCodigo(NULL, "SOMA");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_SUM });
}

void SUBT() {
    geraCodigo(NULL, "SUBT");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_SUB });
}

void MULT() {
    geraCodigo(NULL, "MULT");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_MUL });
}

void DIVI() {
    geraCodigo(NULL, "DIVI");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_DIV });
}

void CMMA() {
    geraCodigo(NULL, "CMMA");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_GT });
}

void CMAG() {
    geraCodigo(NULL, "CMAG");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_GEQ });
}

void CMME() {
    geraCodigo(NULL, "CMME");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_LE });
}

void CMEG() {
    geraCodigo(NULL, "CMEG");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_LEQ });
}

void CMIG() {
    geraCodigo(NULL, "CMIG");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_EQ });
}

void CMDG() {
    geraCodigo(NULL, "CMDG");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_NEQ });
}

void CONJ() {
    geraCodigo(NULL, "CONJ");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_AND });
}

void DISJ() {
    geraCodigo(NULL, "DISJ");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_OR });
}

void NEGA() {
    geraCodigo(NULL, "NEGA");

    add_expression_element((Operation_structure){ .category = OPER, .oper_type = OPER_NOT });
}

void DSVF1() {
    char str[100];

    sprintf(str, "DSVF R%02d", comp_struct.stack[comp_struct.stack_head].first_label);
    geraCodigo(NULL, str);
}

void DSVF2() {
    char str[100];

    sprintf(str, "DSVF R%02d", comp_struct.stack[comp_struct.stack_head].second_label);
    geraCodigo(NULL, str);
}

void DSVS1() {
    char str[100];

    sprintf(str, "DSVS R%02d", comp_struct.stack[comp_struct.stack_head].first_label);
    geraCodigo(NULL, str);
}

void DSVS2() {
    char str[100];

    sprintf(str, "DSVS R%02d", comp_struct.stack[comp_struct.stack_head].second_label);
    geraCodigo(NULL, str);
}

void ENPR() {
    char str[100];

    sprintf(str, "ENPR %d", comp_struct.lexicon_lvl);
    geraCodigo(NULL, str);
}

void RTPR() {
    Generic_symbol_structure *procedure = comp_struct.declarations[comp_struct.declarations_num - 1];
    char str[100];

    sprintf(str, "RTPR %d, %d", procedure->lexicon_lvl, procedure->num_parameters);
    geraCodigo(NULL, str);

    comp_struct.declarations_num--;
}

void CHPR() {
    DEBUG("current_call: %d\n", comp_struct.current_call);
    Generic_symbol_structure procedure_struct = search_in_symbol_table(comp_struct.call_targets[comp_struct.current_call].label, comp_struct.sym_table);
    char str[100];

    sprintf(str, "CHPR R%02d, %d", procedure_struct.MEPA_label, comp_struct.lexicon_lvl);
    geraCodigo(NULL, str);
}

void LEIT() {
    geraCodigo(NULL, "LEIT");
}

void IMPR() {
    geraCodigo(NULL, "IMPR");
}

Compiler_structure variable_initializer() {
    Compiler_structure new_comp_struct = {
        .sym_table = create_symbol_table(),
        .lexicon_lvl = 0,
        .current_expression = -1,
        .stack_head = -1,
        .declarations_num = 0,
        .current_call = -1,
    };

    memset(new_comp_struct.expression_elements_num, 0, MIN_SIZE * sizeof(int));
    memset(new_comp_struct.variable_counter_list, 0, MIN_SIZE * sizeof(int));
    memset(new_comp_struct.parameter_counter_list, 0, MIN_SIZE * sizeof(int));

    return new_comp_struct;
}

void add_variable_to_symbol_table() {
    add_element_to_symbol_table(&(comp_struct.sym_table), create_symbol_variable(comp_struct.last_identifier, VAR_UNDEFINED, comp_struct.lexicon_lvl, comp_struct.variable_counter_list[comp_struct.lexicon_lvl]));

    // Increment the number of variables in that lexicon level.
    comp_struct.variable_counter_list[comp_struct.lexicon_lvl]++;
}

void add_procedure_to_symbol_table() {
    Generic_symbol_structure p = create_symbol_procedure(comp_struct.last_identifier, comp_struct.lexicon_lvl);
    comp_struct.declarations[comp_struct.declarations_num] = add_element_to_symbol_table(&(comp_struct.sym_table), p);
    comp_struct.declarations_num++;
}

void add_function_to_symbol_table() {
    Generic_symbol_structure f = create_symbol_function(comp_struct.last_identifier, comp_struct.lexicon_lvl, 0);
    comp_struct.declarations[comp_struct.declarations_num] = add_element_to_symbol_table(&(comp_struct.sym_table), f);
    comp_struct.declarations_num++;
}

void add_procedure_param_to_symbol_table(Symbol_param_category category) {
    Generic_symbol_structure p = create_symbol_parameter(comp_struct.last_identifier, comp_struct.lexicon_lvl, 0, category);

    // Increment the number of parameters in that lexicon level.
    comp_struct.parameter_counter_list[comp_struct.lexicon_lvl]++;

    Generic_symbol_structure *procedure = comp_struct.declarations[comp_struct.declarations_num - 1];
    procedure->parameters[procedure->num_parameters] = p;
    p.reference_in_procedure = &procedure->parameters[procedure->num_parameters];
    procedure->num_parameters++;

    add_element_to_symbol_table(&(comp_struct.sym_table), p);
}

void add_procedure_param_cop_to_symbol_table() {
    add_procedure_param_to_symbol_table(PARAM_COP);
}

void add_procedure_param_ref_to_symbol_table() {
    add_procedure_param_to_symbol_table(PARAM_REF);
}

Symbol_variable_type check_token_variable_type(char token[TOKEN_SIZE]) {
    Symbol_variable_type type = VAR_UNDEFINED;
    
    // if ("integer" == token) {}
    if (strncasecmp("integer", token, TOKEN_SIZE) == 0) {
        type = VAR_INT;
    }

    if (strncasecmp("boolean", token, TOKEN_SIZE) == 0) {
        type = VAR_BOOL;
    }

    return type;
}

void compiler_set_symbol_type() {
    Symbol_variable_type type = check_token_variable_type(token);
    set_symbol_type(&comp_struct.sym_table, type);
}

void compiler_set_function_type() {
    Symbol_variable_type type = check_token_variable_type(token);
    Generic_symbol_structure *fn = comp_struct.declarations[comp_struct.declarations_num - 1];
    fn->function_return_type = type;
}

void init_expression() {
    comp_struct.current_expression++;
    comp_struct.expression_elements_num[comp_struct.current_expression] = 0;
}

void finish_expression() {
    comp_struct.current_expression--;
}

void add_expression_element(Operation_structure operation) {
    comp_struct.expression_elements[comp_struct.current_expression][comp_struct.expression_elements_num[comp_struct.current_expression]] = operation;
    comp_struct.expression_elements_num[comp_struct.current_expression]++;
}

void print_operation(Operation_structure factor1, Operation_structure factor2, Operation_structure operator) {
    const char* fact_types[] = {
        "bool",
        "int"
    };
    const char* op_types[] = {
        "+",
        "-",
        "*",
        "/",
        ">=",
        ">",
        "<",
        "<=",
        "=",
        "!=",
        "and",
        "or",
        "not"
    };
    printf("op: %s %s %s\n", fact_types[factor1.fact_type], op_types[operator.oper_type], fact_types[factor2.fact_type]);
}

Factor evaluate_operation(Operation_structure factor1, Operation_structure factor2, Operation_structure operator) {
    // print_operation(factor1, factor2, operator);
    if (factor1.fact_type != factor2.fact_type) {
        printf("Invalid operation between INT and BOOL!!!\n");
        exit(-2);
    }

    if (factor1.fact_type == FACT_BOOL) {
        if (operator.oper_type == OPER_DIV || operator.oper_type == OPER_MUL || operator.oper_type == OPER_SUB || operator.oper_type == OPER_SUM ||
            operator.oper_type == OPER_GT  || operator.oper_type == OPER_GEQ || operator.oper_type == OPER_LE  || operator.oper_type == OPER_LEQ) { // remain the >, <, etc
            printf("Invalid operation, math operation with booleans factors!!!\n");
            exit(-3);
        }
        return FACT_BOOL;
    }

    if (factor1.fact_type == FACT_INT) {
        if (operator.oper_type == OPER_AND || operator.oper_type == OPER_OR || operator.oper_type == OPER_NOT) {
            printf("Invalid operation, boolean operation with math factors!!!\n");
            exit(-7);
        }

        if (operator.oper_type == OPER_DIV || operator.oper_type == OPER_MUL || operator.oper_type == OPER_SUB || operator.oper_type == OPER_SUM) {
            return FACT_INT;
        } else if (operator.oper_type == OPER_GT  || operator.oper_type == OPER_GEQ || operator.oper_type == OPER_LE ||
                   operator.oper_type == OPER_LEQ || operator.oper_type == OPER_EQ  || operator.oper_type == OPER_NEQ) { // ver se é OPER_GT (>), OPER_GTE (>=), etc
            return FACT_BOOL;
        }
    }

    printf("Unexpected Error!!!\n");
    exit(-4);
}

void print_expression_elements() {
    printf("[ ");
    for (int i = 0; i < comp_struct.expression_elements_num[comp_struct.current_expression]; i++) {
        Operation_structure operation = comp_struct.expression_elements[comp_struct.current_expression][i];
        char *element;
        if (operation.category == OPER) {
            switch (operation.oper_type) {
                case OPER_SUM:
                    element = "+";
                    break;
                case OPER_SUB:
                    element = "-";
                    break;
                case OPER_MUL:
                    element = "*";
                    break;
                case OPER_DIV:
                    element = "/";
                    break;
                case OPER_GEQ:
                    element = ">=";
                    break;
                case OPER_GT:
                    element = ">";
                    break;
                case OPER_LE:
                    element = "<";
                    break;
                case OPER_LEQ:
                    element = "<=";
                    break;
                case OPER_EQ:
                    element = "=";
                    break;
                case OPER_NEQ:
                    element = "!=";
                    break;
                case OPER_AND:
                    element = "and";
                    break;
                case OPER_OR:
                    element = "or";
                    break;
                case OPER_NOT:
                    element = "not";
                    break;
            }
        } else {
            switch (operation.fact_type) {
                case FACT_INT:
                    element = "int";
                    break;
                case FACT_BOOL:
                    element = "bool";
                    break;
            }
        }
        printf("%s, ", element);
    }
    printf(" ]\n");
}

Factor evaluate_expression() {
    int i = 0;
    // search for next operator
    for (i = 0; i < comp_struct.expression_elements_num[comp_struct.current_expression]; i++) {
        if (comp_struct.expression_elements[comp_struct.current_expression][i].category == OPER) {
            break;
        }
    }

    while (comp_struct.expression_elements_num[comp_struct.current_expression] > 1) {
        if (i == comp_struct.expression_elements_num[comp_struct.current_expression]) {
            // não tem operador?
            printf("Malformed expression!!!\n");
            exit(-8);
        }

        Operation_structure operator = comp_struct.expression_elements[comp_struct.current_expression][i];
        Factor operation_result;

        if (operator.oper_type == OPER_NOT) {
            // caso do NOT
            if (i - 1 < 0) {
                // não tem fatores o suficiente
                printf("NOT operation without operand!!!\n");
                exit(-9);
            }

            Operation_structure factor1 = comp_struct.expression_elements[comp_struct.current_expression][i - 1];
            if (factor1.category == OPER || factor1.fact_type == FACT_INT) {
                // NOT de INT ou operador
                printf("NOT operation of non-boolean!!!\n");
                exit(-10);
            }

            operation_result = FACT_BOOL;

            i -= 1;
            comp_struct.expression_elements[comp_struct.current_expression][i].category = FACT;
            comp_struct.expression_elements[comp_struct.current_expression][i].fact_type = operation_result;

            for (int j = i+1; j < comp_struct.expression_elements_num[comp_struct.current_expression] - 1; j++) {
                comp_struct.expression_elements[comp_struct.current_expression][j] = comp_struct.expression_elements[comp_struct.current_expression][j+1];
            }

            comp_struct.expression_elements_num[comp_struct.current_expression] -= 1;
        } else {
            if (i - 2 < 0) {
                // não tem fatores o suficiente
                printf("Insufficient operands for operation!!!\n");
                exit(-11);
            }

            Operation_structure factor2 = comp_struct.expression_elements[comp_struct.current_expression][i - 1];
            Operation_structure factor1 = comp_struct.expression_elements[comp_struct.current_expression][i - 2];
            operation_result = evaluate_operation(factor1, factor2, operator);

            i -= 2;
            comp_struct.expression_elements[comp_struct.current_expression][i].category = FACT;
            comp_struct.expression_elements[comp_struct.current_expression][i].fact_type = operation_result;

            for (int j = i+1; j < comp_struct.expression_elements_num[comp_struct.current_expression] - 2; j++) {
                comp_struct.expression_elements[comp_struct.current_expression][j] = comp_struct.expression_elements[comp_struct.current_expression][j+2];
            }

            comp_struct.expression_elements_num[comp_struct.current_expression] -= 2;
        }

        // search for next operator
        for (; i < comp_struct.expression_elements_num[comp_struct.current_expression]; i++) {
            if (comp_struct.expression_elements[comp_struct.current_expression][i].category == OPER) {
                break;
            }
        }
    }

    if (comp_struct.expression_elements_num[comp_struct.current_expression] <= 0) {
        // algo muito errado aconteceu!
        printf("Unexpected Error!!!\n");
        exit(-12);
    }

    return comp_struct.expression_elements[comp_struct.current_expression][0].fact_type;
}

void evaluate_assignment_expression() {
    Factor expression_result = evaluate_expression();
    finish_expression();
    Generic_symbol_structure assignment_target = search_in_symbol_table(comp_struct.assignment_temp, comp_struct.sym_table);

    Symbol_variable_type type = assignment_target.category == SYMBOL_FUNCTION ? assignment_target.function_return_type : assignment_target.variable_type;
    if (type == VAR_INT && expression_result == FACT_BOOL) {
        printf("Invalid assignment of BOOL into INT!\n");
        exit(-5);
    }

    if (type == VAR_BOOL && expression_result == FACT_INT) {
        printf("Invalid assignment of INT into BOOL!\n");
        exit(-6);
    }
}

void evaluate_conditional_expression() {
    Factor expression_result = evaluate_expression();
    finish_expression();
    if (expression_result == FACT_INT) {
        printf("Expected conditional expression, got INT.\n");
        exit(-13);
    }
}

int get_num_label() {
    static int num = 0;
    num++;
    return num - 1;
}

void push_stack(Stack_type type) {
    comp_struct.stack_head++;
    comp_struct.stack[comp_struct.stack_head] = (Stack_label){
        .type = type,
        .first_label = get_num_label(),
        .second_label = type != BLOCK ? get_num_label() : -1
    };
}

void pop_stack() {
    comp_struct.stack_head--;
}

void print_label(int label) {
    char str[4];

    sprintf(str, "R%02d", label);
    geraCodigo(str, "NADA");
}

void print_current_declaration_label() {
    print_label(comp_struct.declarations[comp_struct.declarations_num - 1]->MEPA_label);
}

void print_first_stack_label() {
    print_label(comp_struct.stack[comp_struct.stack_head].first_label);
}

void print_second_stack_label() {
    print_label(comp_struct.stack[comp_struct.stack_head].second_label);
}

void allocate_print_while_stack_label() {
    push_stack(WHILE);
    print_first_stack_label();
}

void free_print_while_stack_label() {
    print_second_stack_label();
    pop_stack();
}

void compiler_set_symbol_offset() {
    set_symbol_offset(&comp_struct.sym_table);
}

void begin_procedure() {
    comp_struct.lexicon_lvl++;
    comp_struct.variable_counter_list[comp_struct.lexicon_lvl] = 0;
    comp_struct.parameter_counter_list[comp_struct.lexicon_lvl] = 0;
    add_procedure_to_symbol_table();
}

void end_procedure() {
    comp_struct.lexicon_lvl--;
    delete_invisible_symbols(&(comp_struct.sym_table), comp_struct.lexicon_lvl);
}

void begin_function() {
    comp_struct.lexicon_lvl++;
    comp_struct.variable_counter_list[comp_struct.lexicon_lvl] = 0;
    comp_struct.parameter_counter_list[comp_struct.lexicon_lvl] = 0;
    add_function_to_symbol_table();
}

void end_function() {
    comp_struct.lexicon_lvl--;
    delete_invisible_symbols(&(comp_struct.sym_table), comp_struct.lexicon_lvl);
}

void verify_if_call_parameters_match() {
    if (comp_struct.call_targets[comp_struct.current_call].num_parameters != comp_struct.call_parameters[comp_struct.current_call]) {
        printf("Too few arguments for call of '%s': expected %d, got %d.\n", comp_struct.call_targets[comp_struct.current_call].label, comp_struct.call_targets[comp_struct.current_call].num_parameters, comp_struct.call_parameters[comp_struct.current_call]);
        exit(-15);
    }
}

void begin_call() {
    Generic_symbol_structure pof = search_in_symbol_table(comp_struct.last_identifier, comp_struct.sym_table);
    comp_struct.current_call++;
    comp_struct.call_targets[comp_struct.current_call] = pof;
    comp_struct.call_parameters[comp_struct.current_call] = 0;

    if (pof.category == SYMBOL_FUNCTION) {
        AMEM_function();

        bool is_inside_expression = comp_struct.current_expression >= 0;
        if (is_inside_expression) {
            Factor return_type = pof.function_return_type == VAR_INT ? FACT_INT : FACT_BOOL;
            add_expression_element((Operation_structure){ .category = FACT, .fact_type = return_type, .fact_symbol = S_NUMBER });
        }
    }
}

void end_call() {
    verify_if_call_parameters_match();
    comp_struct.current_call--;
}

void increment_call_parameter() {
    comp_struct.call_parameters[comp_struct.current_call]++;
}

void evaluate_parameter() {
    int current_parameter = comp_struct.call_parameters[comp_struct.current_call];
    Generic_symbol_structure current_call_target = comp_struct.call_targets[comp_struct.current_call];

    if (current_parameter >= current_call_target.num_parameters) {
        printf("Too many arguments for call of '%s': expected %d, got %d.\n", current_call_target.label, comp_struct.call_targets[comp_struct.current_call].num_parameters, comp_struct.call_parameters[comp_struct.current_call]);
        exit(-18);
    }

    Generic_symbol_structure current_call_parameter = current_call_target.parameters[current_parameter];
    
    if (current_call_parameter.param_category == PARAM_REF) {
        if (comp_struct.expression_elements_num[comp_struct.current_expression] > 1) {
            printf("You cannot use an expression to pass as a parameter by reference!\n");
            exit(-21);
        } else if (comp_struct.expression_elements_num[comp_struct.current_expression] == 1 && comp_struct.expression_elements[comp_struct.current_expression][0].fact_symbol == S_NUMBER) {
            printf("You cannot use a number to pass as a parameter by reference!\n");
            exit(-22);
        }
    }

    Factor expression_result = evaluate_expression();
    finish_expression();
    DEBUG("current_call: %d, current_parameter: %d\n", comp_struct.current_call, current_parameter);

    if (current_call_target.parameters[current_parameter].variable_type == VAR_INT && expression_result == FACT_BOOL) {
        printf("Invalid parameter type: expected INT, got BOOL.\n");
        exit(-16);
    }

    if (current_call_target.parameters[current_parameter].variable_type == VAR_BOOL && expression_result == FACT_INT) {
        printf("Invalid parameter type: expected BOOL, got INT.\n");
        exit(-17);
    }
}

void store(char target[TOKEN_SIZE]) {
    Generic_symbol_structure assignment_target = search_in_symbol_table(target, comp_struct.sym_table);
    switch (assignment_target.category) {
        case SYMBOL_FUNCTION:
        case SYMBOL_VAR:
            ARMZ(target);
            break;
        case SYMBOL_PARAMETER:
            switch (assignment_target.param_category) {
                case PARAM_REF:
                    ARMI(target);
                    break;
                case PARAM_COP:
                    ARMZ(target);
                    break;
                default:
                    printf("Unknown param category '%d'!\n", assignment_target.param_category);
                    exit(-20);
            }
            break;
        case SYMBOL_PROCEDURE:
            printf("You cannot assign a value to a procedure!\n");
            exit(-23);
            break;
        default:
            printf("Unknown symbol category '%d'!\n", assignment_target.category);
            exit(-19);
    }
}

void store_for_read() {
    DEBUG("Last identifier: '%s'\n", comp_struct.last_identifier);
    store(comp_struct.last_identifier);
}

void store_for_assignment() {
    DEBUG("Assignment temp: '%s'\n", comp_struct.assignment_temp);
    store(comp_struct.assignment_temp);
}

void load_outside_call(Generic_symbol_structure target) {
    switch (target.category) {
        case SYMBOL_VAR:
            CRVL();
            break;
        case SYMBOL_PARAMETER:
            switch (target.param_category) {
                case PARAM_REF:
                    CRVI();
                    break;
                case PARAM_COP:
                    CRVL();
                    break;
            }
            break;
    }
}

void load_inside_call(Generic_symbol_structure target) {
    int current_parameter = comp_struct.call_parameters[comp_struct.current_call];
    Generic_symbol_structure current_call_target = comp_struct.call_targets[comp_struct.current_call];
    Generic_symbol_structure current_call_parameter = current_call_target.parameters[current_parameter];

    switch (target.category) {
        case SYMBOL_VAR:
            switch (current_call_parameter.param_category) {
                case PARAM_REF:
                    CREN();
                    break;
                case PARAM_COP:
                    CRVL();
                    break;
            }
            break;
        case SYMBOL_PARAMETER:
            switch (target.param_category) {
                case PARAM_REF:
                    switch (current_call_parameter.param_category) {
                        case PARAM_REF:
                            CRVL();
                            break;
                        case PARAM_COP:
                            CRVI();
                            break;
                    }
                    break;
                case PARAM_COP:
                    switch (current_call_parameter.param_category) {
                        case PARAM_REF:
                            CREN();
                            break;
                        case PARAM_COP:
                            CRVL();
                            break;
                    }
                    break;
            }
            break;
    }
}

void load() {
    Generic_symbol_structure target = search_in_symbol_table(comp_struct.last_identifier, comp_struct.sym_table);

    // funções sem parametro apenas!
    if (target.category == SYMBOL_FUNCTION) {
        begin_call();
        CHPR();
        end_call();
        return;
    }

    bool is_inside_call = comp_struct.current_call >= 0;
    if (!is_inside_call) {
        load_outside_call(target);
    } else {
        load_inside_call(target);
    }
}
