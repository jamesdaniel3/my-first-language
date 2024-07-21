#include "lexer.h"
#include "helpers.h"
#include "lexer.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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