#include <stdlib.h>
#include "defs.h"

void free_token(Token *token) {
    if (token != NULL) {
        if (token->new_token == 1) {
            free(token->str);
            free(token);
        }
    }
}

void free_token_arr(TokenArray *token_arr) {
    if (token_arr != NULL) {
        for (size_t i = 0; i < token_arr->count; i++) {
            free(token_arr->tokens[i].str);
        }
        free(token_arr->tokens);
        free(token_arr);
    }
}

void free_nest(Nest *nest);

void free_data(Data *data) {
    if (data != NULL) {
        if (data->token != NULL) {
            free_token(data->token);
        }
        if (data->nest_ptr != NULL) {
            free_nest(data->nest_ptr);
        }
    }
}

void free_data_array(DataArray *data_array) {
    if (data_array != NULL) {
        for (size_t i = 0; i < data_array->count; i++) {
            free_data(&data_array->data[i]);
        }
        free(data_array->data);
        free(data_array);
    }
}

void free_nest(Nest *nest) {
    if (nest != NULL) {
        free_token(nest->op_symb);
        free_data_array(nest->data_array);
        free(nest);
    }
}

void free_env(Environment *env) {
    if (env != NULL) {
        if (env->variables != NULL) {
            for (size_t i = 0; i < env->count; i++) {
                if (env->variables[i].type == LAMBDA) {
                    free_nest(env->variables[i].lambda_params);
                    free_nest(env->variables[i].lambda_body);
                } else if (env->variables[i].type == TOKEN) {
                    free_token(env->variables[i].token);
                }
                free(env->variables[i].name);
            }
        }
        free(env->variables);
        free(env);
    }
}