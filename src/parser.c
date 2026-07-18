#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defs.h"

Nest *parse(const TokenArray *token_arr) {
    size_t count = token_arr->count;
    Nest *nest = calloc(1, sizeof(Nest));
    nest->tokens_covered = 0;

    nest->data_array = calloc(1, sizeof(DataArray));
    nest->data_array->count = 0;

    nest->data_array->data = calloc(1, sizeof(Data));

    Arg exp_next = START;

    for (size_t i = 0; i < count; i++) {
        Token token = token_arr->tokens[i];
        if (token.type == LPAR) {
            if (exp_next == START) {
                exp_next = OP;
                continue;
            } else if (exp_next == DATA || exp_next == DATA_OR_END) {
                TokenArray *remaining_arr = malloc(sizeof(TokenArray));
                remaining_arr->tokens = &token_arr->tokens[i];
                remaining_arr->count = count - i;
                Nest *inner_nest = parse(remaining_arr);
                free(remaining_arr);


                nest->data_array->count++;
                nest->data_array->data = realloc(nest->data_array->data, nest->data_array->count * sizeof(Data));
                nest->data_array->data[nest->data_array->count - 1].nest_ptr = inner_nest;
                nest->data_array->data[nest->data_array->count - 1].token = NULL;

                nest->tokens_covered += inner_nest->tokens_covered;
                i += inner_nest->tokens_covered;

                exp_next = DATA_OR_END;
            } else if (exp_next == OP) {
                printf("Syntax error: expected an operator after '(', found another '('\n");
                longjmp(repl_recover, 1);
            }
        } else if (token.type == SYM) {
            if (exp_next == OP) {
                nest->op_symb = &token_arr->tokens[i];
                exp_next = DATA_OR_END;
            } else if (exp_next == DATA || exp_next == DATA_OR_END) {
                nest->data_array->count++;
                nest->data_array->data = realloc(nest->data_array->data, nest->data_array->count * sizeof(Data));
                nest->data_array->data[nest->data_array->count - 1].nest_ptr = NULL;
                nest->data_array->data[nest->data_array->count - 1].token = &token_arr->tokens[i];
                nest->data_array->data[nest->data_array->count - 1].token->new_token = 0;
                exp_next = DATA_OR_END;
            } else if (exp_next == START) {
                nest->op_symb = &token_arr->tokens[i];
                exp_next = END;
            } else {
                printf("Syntax error: unexpected symbol '%s'\n", token.str);
                longjmp(repl_recover, 1);
            }
        } else if (token.type == NUM) {
            if (exp_next == DATA || exp_next == DATA_OR_END) {
                nest->data_array->count++;
                nest->data_array->data = realloc(nest->data_array->data, nest->data_array->count * sizeof(Data));
                nest->data_array->data[nest->data_array->count - 1].nest_ptr = NULL;
                nest->data_array->data[nest->data_array->count - 1].token = &token_arr->tokens[i];
                nest->data_array->data[nest->data_array->count - 1].token->new_token = 0;
                exp_next = DATA_OR_END;
            } else if (exp_next == START || exp_next == OP) {
                nest->op_symb = &token_arr->tokens[i];
                exp_next = END;
            } else {
                printf("Syntax error: unexpected number '%s'\n", token.str);
                longjmp(repl_recover, 1);
            }
        } else if (token.type == RPAR) {
            if (exp_next == DATA_OR_END || exp_next == END) {
                nest->tokens_covered++;
                return nest;
            } else if (exp_next == START || exp_next == OP || exp_next == DATA) {
                printf("Syntax error: unexpected ')' — expression is incomplete\n");
                longjmp(repl_recover, 1);
            }
        }

        nest->tokens_covered++;
    }

    return nest;
}