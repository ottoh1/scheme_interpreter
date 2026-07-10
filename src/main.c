#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"

#define MAX_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc == 4 && strcmp(argv[1], "--test") == 0) { // ./scheme --test tests.scm tests.expected
        FILE *scm = fopen(argv[2], "r");
        FILE *exp = fopen(argv[3], "r");
        if (scm == NULL || exp == NULL) {
            printf("Could not find test files\n");
            exit(1);
        }

        char line[MAX_LENGTH];
        char expected[MAX_LENGTH];
        int passed = 0;
        int failed = 0;
        int test_num = 0;

        while (fgets(line, sizeof(line), scm)) {
            if (line[0] == ';' || line[0] == '\n') {continue;}

            if (fgets(expected, sizeof(expected), exp) == NULL) {
                printf("Expected file ran out of lines\n");
                break;
            }

            expected[strcspn(expected, "\n")] = '\0'; // remove newline characters

            TokenArray tokens = tokenize(line);
            Nest *nest_ptr = parse(&tokens);
            Token *result = evaluate(nest_ptr);

            test_num++;
            if (strcmp(result->str, expected) == 0) {
                passed++;
            } else {
                failed++;
                printf("Failed test %d: %s", test_num, line);
                printf("expected: %s\n", expected);
                printf("actual: %s\n", result->str);
            }
        }

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
        char line[MAX_LENGTH];
        while (fgets(line, sizeof(line), f)) {
            if (line[0] == ';' || line[0] == '\n') continue;
            TokenArray tokens = tokenize(line);
            Nest *nest_ptr = parse(&tokens);
            Token *result = evaluate(nest_ptr);
            printf("%s\n", result->str);
        }
        fclose(f);

    } else { // use stdin
        char input[MAX_LENGTH];
        while (1) {
            printf("> ");
            if (fgets(input, MAX_LENGTH, stdin) == NULL) { // Ctrl+D
                printf("\n");
                break;
            }
            if (input[0] == '\n') continue;
            TokenArray tokens = tokenize(input);
            Nest *nest_ptr = parse(&tokens);
            Token *result = evaluate(nest_ptr);
            printf("%s\n", result->str);
        }
    }

    return 0;
}