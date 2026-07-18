#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defs.h"

Environment *build_environment(Environment *parent) {
    Environment *env = calloc(1, sizeof(Environment));
    env->parent = parent;

    return env;
}

Variable *lookup_variable(Environment *env, char *name) {
    while (env != NULL) {
        for (size_t i = 0; i < env->count; i++) {
            if (strcmp(env->variables[i].name, name) == 0) {
                return &env->variables[i];
            }
        }
        env = env->parent;
    }
    return NULL;
}

Nest *copy_nest(const Nest *src) { // copies the nest to be held as lambda_params or lambda_body
    if (src == NULL) return NULL;

    Nest *copy = calloc(1, sizeof(Nest));

    copy->op_symb = malloc(sizeof(Token));
    copy->op_symb->type = src->op_symb->type;
    copy->op_symb->str = strdup(src->op_symb->str);
    copy->op_symb->new_token = 1;

    copy->data_array = calloc(1, sizeof(DataArray));
    copy->data_array->count = src->data_array->count;
    copy->data_array->data = calloc(src->data_array->count, sizeof(Data));

    for (size_t i = 0; i < src->data_array->count; i++) {
        if (src->data_array->data[i].nest_ptr != NULL) {
            copy->data_array->data[i].nest_ptr = copy_nest(src->data_array->data[i].nest_ptr);
            copy->data_array->data[i].token = NULL;
        } else if (src->data_array->data[i].token != NULL) {
            Token *t = malloc(sizeof(Token));
            t->type = src->data_array->data[i].token->type;
            t->str = strdup(src->data_array->data[i].token->str);
            t->new_token = 1;
            copy->data_array->data[i].token = t;
            copy->data_array->data[i].nest_ptr = NULL;
        }
    }
    return copy;
}