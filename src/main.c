#include <stdio.h>

#include "defs.h"

#define MAX_LENGTH 1024

int main(void) {
    char input[MAX_LENGTH];

    while (1) {
        printf("> ");

        if (fgets(input, MAX_LENGTH, stdin) == NULL) {
            break;
        }
        
        TokenArray tokens = tokenize(input);

        printf("Got %zu tokens\n", tokens.count);

        Nest* nest_ptr = parse(&tokens);

        printf("Nest's operation symbol is %s\n", nest_ptr->op_symb->str);

        Token *eval = evaluate(nest_ptr);

        printf("%s\n", eval->str);

    }

    return 0;
}