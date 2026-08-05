#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"

jmp_buf repl_recover;

#define MAX_LENGTH 4096

int read_expression(FILE *f, char *expr, size_t expr_size);

int main(int argc, char *argv[]) {
    if (argc == 4 && strcmp(argv[1], "--test") == 0) { // ./scheme --test tests.scm tests.expected
        FILE *scm = fopen(argv[2], "r");
        FILE *exp = fopen(argv[3], "r");
        if (scm == NULL || exp == NULL) {
            printf("Could not find test files\n");
            exit(1);
        }

        char expr[MAX_LENGTH];
        char expected[MAX_LENGTH];
        int passed = 0;
        int failed = 0;
        int test_num = 0;
        Environment *env = calloc(1, sizeof(Environment));

        while (1) {
            if (setjmp(repl_recover) != 0) {
                // evaluation error — count as a failed test, move on
                test_num++;
                failed++;
                printf("Failed test %d (error): %s", test_num, expr);
                continue;
            }

            if (!read_expression(scm, expr, sizeof(expr))) break;

            if (fgets(expected, sizeof(expected), exp) == NULL) {
                printf("Expected file ran out of lines\n");
                break;
            }

            expected[strcspn(expected, "\n")] = '\0';

            TokenArray *tokens = tokenize(expr);
            Nest *nest_ptr = parse(tokens);
            Token *result = evaluate(nest_ptr, env);

            test_num++;
            if (strcmp(result->str, expected) == 0) {
                passed++;
            } else {
                failed++;
                printf("Failed test %d: %s", test_num, expr);
                printf("expected: %s\n", expected);
                printf("actual: %s\n", result->str);
            }
            free_nest(nest_ptr);
            free_token_arr(tokens);
            free_token(result);
        }
        free_env(env);

        printf("Passed: %d\n", passed);
        printf("Failed: %d\n", failed);
        fclose(scm);
        fclose(exp);

    } else if (argc == 2) { // ./scheme file.scm
        FILE *f = fopen(argv[1], "r");
        if (f == NULL) {
            printf("Could not open file: %s\n", argv[1]);
            exit(1);
        }
        char expr[MAX_LENGTH];
        Environment *env = calloc(1, sizeof(Environment));
        while (1) {

            if (setjmp(repl_recover) != 0) {
                continue; // error already printed by the thrower; skip to next line
            }

            if (!read_expression(f, expr, sizeof(expr))) break;

            TokenArray *tokens = tokenize(expr);
            Nest *nest_ptr = parse(tokens);
            Token *result = evaluate(nest_ptr, env);

            if (!(strcmp(nest_ptr->op_symb->str, "define") == 0)) { // skip printing for defines
                printf("%s\n", result->str);
            }

            free_nest(nest_ptr);
            free_token_arr(tokens);
            free_token(result);
        }
        fclose(f);
        free_env(env);

    } else { // REPL
        char expr[MAX_LENGTH];
        Environment *env = calloc(1, sizeof(Environment));

        while (1) {
            printf("> ");
            if (setjmp(repl_recover) != 0) {
                continue;
            }

            if (!read_expression(stdin, expr, sizeof(expr))) {
                printf("\n");
                break;
            }

            TokenArray *tokens = tokenize(expr);
            Nest *nest_ptr = parse(tokens);
            Token *result = evaluate(nest_ptr, env);
            printf("%s\n", result->str);
            free_nest(nest_ptr);
            free_token_arr(tokens);
            free_token(result);
        }
        free_env(env);
    }

    return 0;
}

// Reads lines from file until parentheses close completely
int read_expression(FILE *f, char *expr, size_t expr_size) {
    char line[MAX_LENGTH / 4];
    int current_nest = 0;
    expr[0] = '\0';

    while (fgets(line, sizeof(line), f)) {
        if (current_nest == 0 && (line[0] == ';' || line[0] == '\n')) {
            continue; // skip comments and empty lines if mid-expression
        }

        if (strlen(expr) + strlen(line) >= expr_size) {
            printf("Error: expression too long\n");
            longjmp(repl_recover, 1);
        }
        strcat(expr, line);

        for (char *c = line; *c != '\0'; c++) { // update current_nest based on '(' and ')'
            if (*c == '(') current_nest++;
            if (*c == ')') current_nest--;
        }

        if (current_nest < 0) { // too many ')'
            printf("Syntax Error: unexpected ')'\n");
            longjmp(repl_recover, 1);
        }
        if (current_nest == 0 && strlen(expr) > 0) {
            return 1;
        }
    }

    return 0; // EOF
}