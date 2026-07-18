#define _POSIX_C_SOURCE 200809L
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

Token * evaluate(Nest *nest, Environment *env) {
    if (strcmp(nest->op_symb->str, "lambda") == 0) {
        Token *tkn = malloc(sizeof(Token));
        tkn->type = LAMBDA_MARKER;
        char *temp = "lambda";
        tkn->str = malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(tkn->str, temp);
        tkn->new_token = 1;
        tkn->lambda_params = nest->data_array->data[0].nest_ptr;
        tkn->lambda_body = nest->data_array->data[1].nest_ptr;
        return tkn; // don't eval
    }

    if (strcmp(nest->op_symb->str, "if") == 0) { // only evalue the correct nest
        // evaluate the condition
        Token *cond;
        if (nest->data_array->data[0].nest_ptr != NULL) {
            cond = evaluate(nest->data_array->data[0].nest_ptr, env);
        } else {
            // condition is a token -> resolve if it's a variable
            Token *cond_tkn = nest->data_array->data[0].token;
            Variable *v = (cond_tkn->type == SYM) ? lookup_variable(env, cond_tkn->str) : NULL;
            cond = malloc(sizeof(Token));
            cond->new_token = 1;
            if (v != NULL && v->type == TOKEN) {
                cond->type = v->token->type;
                cond->str = strdup(v->token->str);
            } else {
                cond->type = cond_tkn->type;
                cond->str = strdup(cond_tkn->str);
            }
        }

        // pick the branch
        size_t branch = (strcmp(cond->str, "#f") == 0) ? 2 : 1;
        free_token(cond);

        // evaluate only the chosen branch
        if (nest->data_array->data[branch].nest_ptr != NULL) {
            return evaluate(nest->data_array->data[branch].nest_ptr, env);
        } else {
            Token *branch_tkn = nest->data_array->data[branch].token;
            Variable *v = (branch_tkn->type == SYM) ? lookup_variable(env, branch_tkn->str) : NULL;
            Token *out = malloc(sizeof(Token));
            out->new_token = 1;
            if (v != NULL && v->type == TOKEN) {
                out->type = v->token->type;
                out->str = strdup(v->token->str);
            } else {
                out->type = branch_tkn->type;
                out->str = strdup(branch_tkn->str);
            }
            return out;
        }
    }

    // eval inner nests
    for (size_t i = 0; i < nest->data_array->count; i++) {
        if (nest->data_array->data[i].nest_ptr != NULL) {
            nest->data_array->data[i].token = evaluate(nest->data_array->data[i].nest_ptr, env);
            nest->data_array->data[i].token->new_token = 1;
        }
    }

    // set variables
    for (size_t i = 0; i < nest->data_array->count; i++) {
        if (strcmp(nest->op_symb->str, "define") == 0) {break;}
        if (nest->data_array->data[i].token->type == SYM) {
            Variable *v = lookup_variable(env, nest->data_array->data[i].token->str);
            if (v != NULL && v->type == TOKEN) {
                free_token(nest->data_array->data[i].token);
                nest->data_array->data[i].token = malloc(sizeof(Token));
                nest->data_array->data[i].token->type = v->token->type;
                nest->data_array->data[i].token->str = malloc(sizeof(char) * (strlen(v->token->str) + 1));
                strcpy(nest->data_array->data[i].token->str, v->token->str);
                nest->data_array->data[i].token->new_token = 1;
            }
        }
    }

    char *return_str = malloc(64 * sizeof(char));
    Token *return_token = malloc(sizeof(Token));
    return_token->new_token = 1;

    if (str_in(nest->op_symb->str, arithmetic_ops, 4) != 0) { // ARITHMETIC
        float result;

        for (size_t i = 0; i < nest->data_array->count; i++) {
            if (i == 0) {
                result = atof(nest->data_array->data[i].token->str);
                continue;
            }
            if (strcmp(nest->op_symb->str, "+") == 0) {
                result = result + atof(nest->data_array->data[i].token->str);
            } else if (strcmp(nest->op_symb->str, "-") == 0) {
                result = result - atof(nest->data_array->data[i].token->str);
            } else if (strcmp(nest->op_symb->str, "*") == 0) {
                result = result * atof(nest->data_array->data[i].token->str);
            } else if (strcmp(nest->op_symb->str, "/") == 0) {
                result = result / atof(nest->data_array->data[i].token->str);
            }
        }

        snprintf(return_str, 64, "%.3g", result);
        return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
        return_token->type = NUM;

    } else if (str_in(nest->op_symb->str, comparison_ops, 5) != 0) { // COMPARISON
        float result = 1;
        for (size_t i = 0; i < (nest->data_array->count - 1); i++) {
            if (strcmp(nest->op_symb->str, "=") == 0) {
                if (result && (atof(nest->data_array->data[i].token->str) == atof(nest->data_array->data[i+1].token->str))) {
                    result = 1;
                } else {
                    result = 0;
                }
            } else if (strcmp(nest->op_symb->str, "<") == 0) {
                if (result && (atof(nest->data_array->data[i].token->str) < atof(nest->data_array->data[i+1].token->str))) {
                    result = 1;
                } else {
                    result = 0;
                }
            } else if (strcmp(nest->op_symb->str, ">") == 0) {
                if (result && (atof(nest->data_array->data[i].token->str) > atof(nest->data_array->data[i+1].token->str))) {
                    result = 1;
                } else {
                    result = 0;
                }
            } else if (strcmp(nest->op_symb->str, "<=") == 0) {
                if (result && (atof(nest->data_array->data[i].token->str) <= atof(nest->data_array->data[i+1].token->str))) {
                    result = 1;
                } else {
                    result = 0;
                }
            } else if (strcmp(nest->op_symb->str, ">=") == 0) {
                if (result && (atof(nest->data_array->data[i].token->str) >= atof(nest->data_array->data[i+1].token->str))) {
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
    } else if(strcmp(nest->op_symb->str, "and") == 0) { // AND
        if (strcmp(nest->data_array->data[0].token->str, "#t") == 0 && strcmp(nest->data_array->data[1].token->str, "#t") == 0) {
            strcpy(return_str, "#t");
        } else {
            strcpy(return_str, "#f");
        }
        return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
        return_token->type = SYM;
    } else if(strcmp(nest->op_symb->str, "or") == 0) { // OR
        if (strcmp(nest->data_array->data[0].token->str, "#t") == 0 || strcmp(nest->data_array->data[1].token->str, "#t") == 0) {
            strcpy(return_str, "#t");
        } else {
            strcpy(return_str, "#f");
        }
        return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
        return_token->type = SYM;
    } else if(strcmp(nest->op_symb->str, "not") == 0) { // NOT
        if (strcmp(nest->data_array->data[0].token->str, "#t") == 0) {
            strcpy(return_str, "#f");
        } else if (strcmp(nest->data_array->data[0].token->str, "#f") == 0) {
            strcpy(return_str, "#t");
        }
        return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
        return_token->type = SYM;
    } else if(strcmp(nest->op_symb->str, "define") == 0) { // DEFINE VARIABLE
        char variable_name[64] = {0};
        strcpy(variable_name, nest->data_array->data[0].token->str);
        int found = 0;
        for (size_t i = 0; i < env->count; i++) {
            if (strcmp(variable_name, env->variables[i].name) == 0) {
                found = 1;
                if (env->variables[i].token != NULL) {free_token(env->variables[i].token);}
                env->variables[i].type = TOKEN;

                size_t token_str_len = strlen(nest->data_array->data[1].token->str);
                env->variables[i].token = malloc(sizeof(Token));
                env->variables[i].token->new_token = 1;
                env->variables[i].token->str = malloc(sizeof(char) * (token_str_len + 1));
                strcpy(env->variables[i].token->str, nest->data_array->data[1].token->str);
                env->variables[i].token->type = nest->data_array->data[1].token->type;
                
                return_str = realloc(return_str, sizeof(char) * (token_str_len + 1));
                strcpy(return_str, env->variables[i].token->str);
                return_token->type = env->variables[i].token->type;
                break;
            }
        }
        if (found == 0) {
            env->count++;
            env->variables = realloc(env->variables, env->count * sizeof(Variable));
            env->variables[env->count - 1].name = malloc(sizeof(char) * (strlen(variable_name) + 1));
            strcpy(env->variables[env->count - 1].name, variable_name);

            if (nest->data_array->data[1].token->type == LAMBDA_MARKER) {
                env->variables[env->count - 1].type = LAMBDA;
                env->variables[env->count - 1].token = NULL;
                env->variables[env->count - 1].lambda_params = copy_nest(nest->data_array->data[1].token->lambda_params);
                env->variables[env->count - 1].lambda_body  = copy_nest(nest->data_array->data[1].token->lambda_body);
                
                strcpy(return_str, "lambda");
                return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
                return_token->type = SYM;
            } else {
                env->variables[env->count - 1].type = TOKEN;
                env->variables[env->count - 1].token = malloc(sizeof(Token));
                env->variables[env->count - 1].token->new_token = 1;
                size_t token_str_len = strlen(nest->data_array->data[1].token->str);
                env->variables[env->count - 1].token->str = malloc(sizeof(char) * (token_str_len + 1));
                strcpy(env->variables[env->count - 1].token->str, nest->data_array->data[1].token->str);
                env->variables[env->count - 1].token->type = nest->data_array->data[1].token->type;

                return_str = realloc(return_str, sizeof(char) * (token_str_len + 1));
                strcpy(return_str, env->variables[env->count - 1].token->str);
                return_token->type = env->variables[env->count - 1].token->type;
            }
        }
    } else if (nest->op_symb->type == NUM) {
        strcpy(return_str, nest->op_symb->str);
        return_str = realloc(return_str, sizeof(char) * (strlen(return_str) + 1));
        return_token->type = NUM;
    } else if (nest->op_symb->type == SYM) {
        Variable *var = lookup_variable(env, nest->op_symb->str);

        if (var != NULL && var->type == LAMBDA) {
            Environment *call_env = build_environment(env); // create env just for this execution

            size_t nparams = var->lambda_params->data_array->count + 1;
            call_env->variables = malloc(nparams * sizeof(Variable));
            call_env->count = nparams;

            // make first param from op_symb
            call_env->variables[0].name = strdup(var->lambda_params->op_symb->str);
            call_env->variables[0].type = TOKEN;
            call_env->variables[0].token = malloc(sizeof(Token));
            call_env->variables[0].token->new_token = 1;
            call_env->variables[0].token->type = nest->data_array->data[0].token->type;
            call_env->variables[0].token->str = strdup(nest->data_array->data[0].token->str);

            // rest of tokens from data
            for (size_t p = 1; p < nparams; p++) {
                call_env->variables[p].name = strdup(var->lambda_params->data_array->data[p-1].token->str);
                call_env->variables[p].type = TOKEN;
                call_env->variables[p].token = malloc(sizeof(Token));
                call_env->variables[p].token->new_token = 1;
                call_env->variables[p].token->type = nest->data_array->data[p].token->type;
                call_env->variables[p].token->str = strdup(nest->data_array->data[p].token->str);
            }

            Nest *body_copy = copy_nest(var->lambda_body);
            Token *result = evaluate(body_copy, call_env);
            free_nest(body_copy);
            strcpy(return_str, result->str);
            return_str = realloc(return_str, strlen(return_str) + 1);
            return_token->type = result->type;
            free_token(result);
            free_env(call_env);
        } else if (var != NULL && var->type == TOKEN) { // just return the variable's value
            strcpy(return_str, var->token->str);
            return_str = realloc(return_str, strlen(return_str) + 1);
            return_token->type = var->token->type;
        } else { // just return the variable (couldn't find value)
            strcpy(return_str, nest->op_symb->str);
            return_str = realloc(return_str, strlen(return_str) + 1);
            return_token->type = SYM;
        }
    } else {
        printf("Runtime error: unknown operator '%s'\n", nest->op_symb->str);
        longjmp(repl_recover, 1);
    }

    return_token->str = return_str;
    return return_token;
}