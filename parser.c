#include "parser.h"
#include "lexer.h"
#include "symbol_table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
            case TOKEN_DISPLAY:
                // Expect an opening parenthesis
                token = get_next_token(file);
                if (token.type != TOKEN_LPAREN) {
                    fprintf(stderr, "Error: Expected '(' after 'display' on line %d\n", line_number);
                    exit(1);
                }
                
                // Get the argument
                token = get_next_token(file);
                if (token.type == TOKEN_IDENTIFIER) {
                    double value = get_variable_value(symbolTable, token.value);
                    printf("%f\n", value);
                } else if (token.type == TOKEN_INT || token.type == TOKEN_FLOAT) {
                    printf("%s\n", token.value);
                } else {
                    fprintf(stderr, "Error: Invalid argument to display on line %d\n", line_number);
                    exit(1);
                }
                
                // Expect a closing parenthesis
                token = get_next_token(file);
                if (token.type != TOKEN_RPAREN) {
                    fprintf(stderr, "Error: Expected ')' after display argument on line %d\n", line_number);
                    exit(1);
                }
                
                // Expect a semicolon
                token = get_next_token(file);
                if (token.type != TOKEN_SEMICOLON) {
                    fprintf(stderr, "Error: Missing semicolon after display statement on line %d\n", line_number);
                    exit(1);
                }
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