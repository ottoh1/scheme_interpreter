#ifndef DEFS_H
#define DEFS_H

// lexer

typedef enum {
    LPAR,
    RPAR,
    NUM,
    SYM,
    END_OF_FILE,
    INVALID
} TokenType;

typedef struct {
    TokenType type;
    char *str;
    int new_token; // 1 = malloc'd sperately. 0 = points into token array
} Token;

typedef struct {
    Token *tokens; // ptr to tokens
    size_t count;
} TokenArray;

TokenArray *tokenize(const char *input);

// parser

typedef enum {
    START,
    OP,
    DATA,
    DATA_OR_END
} Arg;

typedef struct Nest Nest;
typedef struct Data Data;
typedef struct DataArray DataArray;

struct Data {
    Token *token;
    Nest *nest_ptr;
};

struct DataArray {
    Data *data;
    size_t count;
};

struct Nest {
    Token *op_symb;
    DataArray *data_array;
    size_t tokens_covered;
};


Nest* parse(const TokenArray *token_arr);

// eval

Token* evaluate(Nest *nest);

// free
void free_token(Token *token);
void free_token_arr(TokenArray *token_arr);
void free_nest(Nest *nest);

#endif