#include "symbol_table.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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