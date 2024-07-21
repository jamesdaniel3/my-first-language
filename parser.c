#include "parser.h"
#include "lexer.h"
#include "symbol_table.h"
#include <string.h>
#include <stdlib.h>

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