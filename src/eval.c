#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defs.h"

const char *arithmetic_ops[] = {"+", "-", "*", "/"}; // 4
const char *comparison_ops[]   = {"=", "<", ">", "<=", ">="}; // 5

int str_in(const char *str, const char **list, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(str, list[i]) == 0) return 1;
    }
    return 0;
}

Token * evaluate(Nest *nest) {
    for (size_t i = 0; i < nest->data_array->count; i++) {
        if (nest->data_array->data[i].nest_ptr != NULL) {
            nest->data_array->data[i].token = evaluate(nest->data_array->data[i].nest_ptr);
            nest->data_array->data[i].token->new_token = 1;

        }
    }

    char *return_str = malloc(64 * sizeof(char));
    Token *return_token = malloc(sizeof(Token));
    return_token->new_token = 1;

    if (str_in(nest->op_symb->str, arithmetic_ops, 4) != 0) { // ARITHMETIC
        float result;

        for (size_t i = 0; i < nest->data_array->count; i++) {
            if (i == 0) {
                result = (float) atoi(nest->data_array->data[i].token->str);
                continue;
            }
            if (strcmp(nest->op_symb->str, "+") == 0) {
                result = (float) result + atoi(nest->data_array->data[i].token->str);
            } else if (strcmp(nest->op_symb->str, "-") == 0) {
                result = (float) result - atoi(nest->data_array->data[i].token->str);
            } else if (strcmp(nest->op_symb->str, "*") == 0) {
                result = (float) result * atoi(nest->data_array->data[i].token->str);
            } else if (strcmp(nest->op_symb->str, "/") == 0) {
                result = (float) result / atoi(nest->data_array->data[i].token->str);
            }
        }

        snprintf(return_str, 64, "%.3g", result);
        return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
        return_token->type = NUM;

    } else if (str_in(nest->op_symb->str, comparison_ops, 5) != 0) { // COMPARISON
        int result = 1;
        for (size_t i = 0; i < (nest->data_array->count - 1); i++) {
            if (strcmp(nest->op_symb->str, "=") == 0) {
                if (result && (atoi(nest->data_array->data[i].token->str) == atoi(nest->data_array->data[i+1].token->str))) {
                    result = 1;
                } else {
                    result = 0;
                }
            } else if (strcmp(nest->op_symb->str, "<") == 0) {
                if (result && (atoi(nest->data_array->data[i].token->str) < atoi(nest->data_array->data[i+1].token->str))) {
                    result = 1;
                } else {
                    result = 0;
                }
            } else if (strcmp(nest->op_symb->str, ">") == 0) {
                if (result && (atoi(nest->data_array->data[i].token->str) > atoi(nest->data_array->data[i+1].token->str))) {
                    result = 1;
                } else {
                    result = 0;
                }
            } else if (strcmp(nest->op_symb->str, "<=") == 0) {
                if (result && (atoi(nest->data_array->data[i].token->str) <= atoi(nest->data_array->data[i+1].token->str))) {
                    result = 1;
                } else {
                    result = 0;
                }
            } else if (strcmp(nest->op_symb->str, ">=") == 0) {
                if (result && (atoi(nest->data_array->data[i].token->str) >= atoi(nest->data_array->data[i+1].token->str))) {
                    result = 1;
                } else {
                    result = 0;
                }
            }
        }
        if (result) {
            strcpy(return_str, "#t");
            return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
            return_token->type = SYM;
        } else {
            strcpy(return_str, "#f");
            return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
            return_token->type = SYM;
        }
    } else if(strcmp(nest->op_symb->str, "if") == 0) { // if statement
        int result;
        if (strcmp(nest->data_array->data[0].token->str, "#t") == 0) {
            result = atoi(nest->data_array->data[1].token->str); // get 2nd element if true
        } else if (strcmp(nest->data_array->data[0].token->str, "#f") == 0) {
            result = atoi(nest->data_array->data[2].token->str); // get 3rd element if false
        } else {
            printf("Excepted boolean after 'if'\n");
            exit(1);
        }
        snprintf(return_str, 64, "%d", result);
        return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
        return_token->type = NUM;
    } else if(strcmp(nest->op_symb->str, "and") == 0) { // and statement
        if (strcmp(nest->data_array->data[0].token->str, "#t") == 0 && strcmp(nest->data_array->data[1].token->str, "#t") == 0) {
            strcpy(return_str, "#t");
        } else {
            strcpy(return_str, "#f");
        }
        return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
        return_token->type = SYM;
    } else if(strcmp(nest->op_symb->str, "or") == 0) { // or statement
        if (strcmp(nest->data_array->data[0].token->str, "#t") == 0 || strcmp(nest->data_array->data[1].token->str, "#t") == 0) {
            strcpy(return_str, "#t");
        } else {
            strcpy(return_str, "#f");
        }
        return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
        return_token->type = SYM;
    } else if(strcmp(nest->op_symb->str, "not") == 0) { // not statement
        if (strcmp(nest->data_array->data[0].token->str, "#t") == 0) {
            strcpy(return_str, "#f");
        } else if (strcmp(nest->data_array->data[0].token->str, "#f") == 0) {
            strcpy(return_str, "#t");
        }
        return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
        return_token->type = SYM;
    } else {
        printf("Uknown OP\n");
        exit(1);
    }

    return_token->str = return_str;
    return return_token;
}