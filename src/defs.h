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
} Token;

typedef struct {
    Token *tokens; // ptr to tokens
    size_t count;
} TokenArray;

TokenArray tokenize(const char *input);

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
    size_t relative_pos;
    Token *op_symb;
    DataArray *data_array;
    size_t tokens_covered;
};


Nest* parse(const TokenArray *input);

// eval

Token* evaluate(Nest *nest);

#endif