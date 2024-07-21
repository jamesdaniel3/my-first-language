#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "parser.h"
#include "helpers.h"

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

    return 0;
}