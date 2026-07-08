#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defs.h"

Token * evaluate(Nest *nest) {
    for (size_t i = 0; i < nest->data_array->count; i++) {
        if (nest->data_array->data[i].nest_ptr != NULL) {
            nest->data_array->data[i].token = evaluate(nest->data_array->data[i].nest_ptr);
        }
    }

    float result;

    if (strstr("+-*/", nest->op_symb->str) != NULL) { // arithmetic
        for (size_t i = 0; i < nest->data_array->count; i++) {
            if (i == 0) {
                result = (float) atof(nest->data_array->data[i].token->str);
                continue;
            }
            if (strcmp(nest->op_symb->str, "+") == 0) {
                result = (float) result + atof(nest->data_array->data[i].token->str);
            } else if (strcmp(nest->op_symb->str, "-") == 0) {
                result = (float) result - atof(nest->data_array->data[i].token->str);
            } else if (strcmp(nest->op_symb->str, "*") == 0) {
                result = (float) result * atof(nest->data_array->data[i].token->str);
            } else if (strcmp(nest->op_symb->str, "/") == 0) {
                result = (float) result / atof(nest->data_array->data[i].token->str);
            }
        }
    } else {
        printf("Uknown OP");
        exit(1);
    }

    char *return_str = malloc(64 * sizeof(char));
    snprintf(return_str, 64, "%.3g", result);
    return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));

    Token *return_token = malloc(sizeof(Token));
    return_token->str = return_str;
    return_token->type = NUM;

    return return_token;
}
