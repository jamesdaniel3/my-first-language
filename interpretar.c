#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKEN_LENGTH 1000
#define MAX_TOKENS 1000

typedef enum {
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_SEMICOLON,
    TOKEN_UNKNOWN,
    TOKEN_EOL,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

typedef struct {
    Token tokens[MAX_TOKENS];
    int count;
} TokenArray;

// Function prototypes
Token get_next_token(FILE *file);
void parse_and_store_tokens(FILE *file, TokenArray *tokenArray);
bool is_digit(char c);
const char* token_type_to_string(TokenType type);

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

    TokenArray tokenArray = {0};  // Initialize the token array
    parse_and_store_tokens(file, &tokenArray);

    fclose(file);

    // Print stored tokens (for demonstration)
    for (int i = 0; i < tokenArray.count; i++) {
        printf("Token %d: Type %s, Value '%s'\n", i, token_type_to_string(tokenArray.tokens[i].type), tokenArray.tokens[i].value);
    }

    return 0;
}

Token get_next_token(FILE *file) {
    Token token;
    char c;
    int i = 0;

    // Skip whitespace
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

void parse_and_store_tokens(FILE *file, TokenArray *tokenArray) {
    Token token;
    bool expect_semicolon = false;
    int line_number = 1;  // Initialize line number counter

    while ((token = get_next_token(file)).type != TOKEN_EOF) {
        switch (token.type) {
            case TOKEN_INT:
            case TOKEN_FLOAT:
                expect_semicolon = true;
                break;
            case TOKEN_SEMICOLON:
                if (!expect_semicolon) {
                    fprintf(stderr, "Error: Unexpected semicolon on line %d\n", line_number);
                    exit(1);
                }
                expect_semicolon = false;
                break;
            case TOKEN_EOL:
                if (expect_semicolon) {
                    fprintf(stderr, "Error: Missing semicolon at line end on line %d\n", line_number);
                    exit(1);
                }
                line_number++;  // Increment line number counter
                expect_semicolon = false;  // Reset expectation after newline
                break;
            case TOKEN_UNKNOWN:
                fprintf(stderr, "Error: Unknown token '%s' on line %d\n", token.value, line_number);
                exit(1);
            default:
                fprintf(stderr, "Error: Unexpected token type on line %d\n", line_number);
                exit(1);
        }

        // Store the token in the array
        if (tokenArray->count < MAX_TOKENS) {
            tokenArray->tokens[tokenArray->count++] = token;
        } else {
            fprintf(stderr, "Error: Too many tokens\n");
            exit(1);
        }
    }

    if (expect_semicolon) {
        fprintf(stderr, "Error: Missing semicolon on line %d\n", line_number);
        exit(1);
    }
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}