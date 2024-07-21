#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdbool.h>

#define MAX_TOKEN_LENGTH 1000
#define MAX_TOKENS 1000
#define MAX_VARIABLES 100

typedef enum {
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_SEMICOLON,
    TOKEN_UNKNOWN,
    TOKEN_EOL,
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
} TokenArray;

typedef struct {
    char name[MAX_TOKEN_LENGTH];
    double value;
} Variable;

typedef struct {
    Variable variables[MAX_VARIABLES];
    int count;
} SymbolTable;

#endif 