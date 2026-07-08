#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "defs.h"

TokenArray tokenize(const char *input) {
    TokenArray arr;

    size_t str_len = strlen(input);

    Token *tokens_ptr = NULL;
    int token_num = 0;

    TokenType last_type = INVALID;
    int running_len = 0;
    char * str_ptr = NULL;

    for (size_t i = 0; i < str_len; i++) {

        char current_c = input[i];
        Token current_token;

        current_token.type = INVALID;
        switch (current_c) {
            case '(':
                current_token.type = LPAR;
                current_token.str = "(";
                break;
            case ')':
                current_token.type = RPAR;
                current_token.str = ")";
                break;
            case ' ':
            case '\t':
            case '\n':
                break;
            default:
                if (isdigit(current_c)) {
                    current_token.type = NUM;
                } else {
                    current_token.type = SYM;
                }
        }

        if (current_token.type == NUM || current_token.type == SYM) {
            if (current_token.type == last_type) {
                running_len++;
                str_ptr = realloc(str_ptr, (running_len + 1) * sizeof(char));
                str_ptr[running_len - 1] = current_c;
                str_ptr[running_len] = '\0';
                continue;
            } else {
                running_len++;
                str_ptr = malloc(2 * sizeof(char));
                str_ptr[0] = current_c;
                str_ptr[1] = '\0';
                current_token.str = str_ptr;
            }
        } else {
            running_len = 0;
        }

        if (current_token.type != INVALID) {
            token_num++;
            tokens_ptr = realloc(tokens_ptr, token_num * sizeof(Token));
            *(tokens_ptr + token_num - 1) = current_token;
        }

        last_type = current_token.type;
    }

    arr.tokens = tokens_ptr;
    arr.count = token_num;

    return arr;
}