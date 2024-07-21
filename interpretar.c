#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

// Function prototypes
Token get_next_token(FILE *file);
void parse_and_store_tokens(FILE *file, TokenArray *tokenArray, SymbolTable *symbolTable);
bool is_digit(char c);
bool is_alpha(char c);
const char* token_type_to_string(TokenType type);
void add_or_update_variable(SymbolTable *symbolTable, const char *name, double value);
double get_variable_value(SymbolTable *symbolTable, const char *name);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    TokenArray tokenArray = {0};
    SymbolTable symbolTable = {0};
    parse_and_store_tokens(file, &tokenArray, &symbolTable);

    fclose(file);

    // Print stored tokens and variables (for demonstration)
    for (int i = 0; i < tokenArray.count; i++) {
        printf("Token %d: Type %s, Value '%s'\n", i, token_type_to_string(tokenArray.tokens[i].type), tokenArray.tokens[i].value);
    }
    for (int i = 0; i < symbolTable.count; i++) {
        printf("Variable: %s = %f\n", symbolTable.variables[i].name, symbolTable.variables[i].value);
    }

    return 0;
}

Token get_next_token(FILE *file) {
    Token token;
    char c;
    int i = 0;

    while ((c = fgetc(file)) != EOF && isspace(c) && c != '\n');

    if (c == EOF) {
        token.type = TOKEN_EOF;
        return token;
    }

    if (c == '\n') {
        token.type = TOKEN_EOL;
        token.value[0] = c;
        token.value[1] = '\0';
        return token;
    }

    if (is_digit(c)) {
        bool is_float = false;
        token.value[i++] = c;
        while ((c = fgetc(file)) != EOF && (is_digit(c) || c == '.')) {
            if (c == '.') is_float = true;
            token.value[i++] = c;
        }
        ungetc(c, file);
        token.value[i] = '\0';
        token.type = is_float ? TOKEN_FLOAT : TOKEN_INT;
    } else if (is_alpha(c)) {
        token.value[i++] = c;
        while ((c = fgetc(file)) != EOF && (is_alpha(c) || is_digit(c))) {
            token.value[i++] = c;
        }
        ungetc(c, file);
        token.value[i] = '\0';
        token.type = TOKEN_IDENTIFIER;
    } else if (c == '=') {
        token.type = TOKEN_ASSIGN;
        token.value[0] = c;
        token.value[1] = '\0';
    } else if (c == ';') {
        token.type = TOKEN_SEMICOLON;
        token.value[0] = c;
        token.value[1] = '\0';
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = c;
        token.value[1] = '\0';
    }

    return token;
}

void parse_and_store_tokens(FILE *file, TokenArray *tokenArray, SymbolTable *symbolTable) {
    Token token;
    int line_number = 1;
    char current_variable[MAX_TOKEN_LENGTH] = "";

    while ((token = get_next_token(file)).type != TOKEN_EOF) {
        switch (token.type) {
            case TOKEN_IDENTIFIER:
                strcpy(current_variable, token.value);
                break;
            case TOKEN_ASSIGN:
                if (strlen(current_variable) == 0) {
                    fprintf(stderr, "Error: Invalid assignment on line %d\n", line_number);
                    exit(1);
                }
                break;
            case TOKEN_INT:
            case TOKEN_FLOAT:
                if (strlen(current_variable) > 0) {
                    double value = atof(token.value);
                    add_or_update_variable(symbolTable, current_variable, value);
                    current_variable[0] = '\0';
                }
                break;
            case TOKEN_SEMICOLON:
                break;
            case TOKEN_EOL:
                line_number++;
                break;
            case TOKEN_UNKNOWN:
                fprintf(stderr, "Error: Unknown token '%s' on line %d\n", token.value, line_number);
                exit(1);
            default:
                fprintf(stderr, "Error: Unexpected token type on line %d\n", line_number);
                exit(1);
        }

        if (tokenArray->count < MAX_TOKENS) {
            tokenArray->tokens[tokenArray->count++] = token;
        } else {
            fprintf(stderr, "Error: Too many tokens\n");
            exit(1);
        }
    }
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        case TOKEN_EOF: return "EOF";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_ASSIGN: return "ASSIGN";
        default: return "UNKNOWN";
    }
}

void add_or_update_variable(SymbolTable *symbolTable, const char *name, double value) {
    for (int i = 0; i < symbolTable->count; i++) {
        if (strcmp(symbolTable->variables[i].name, name) == 0) {
            symbolTable->variables[i].value = value;
            return;
        }
    }
    if (symbolTable->count < MAX_VARIABLES) {
        strcpy(symbolTable->variables[symbolTable->count].name, name);
        symbolTable->variables[symbolTable->count].value = value;
        symbolTable->count++;
    } else {
        fprintf(stderr, "Error: Too many variables\n");
        exit(1);
    }
}

double get_variable_value(SymbolTable *symbolTable, const char *name) {
    for (int i = 0; i < symbolTable->count; i++) {
        if (strcmp(symbolTable->variables[i].name, name) == 0) {
            return symbolTable->variables[i].value;
        }
    }
    fprintf(stderr, "Error: Undefined variable '%s'\n", name);
    exit(1);
}