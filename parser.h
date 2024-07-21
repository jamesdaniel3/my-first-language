#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "definitions.h"

void parse_and_store_tokens(FILE *file, TokenArray *tokenArray, SymbolTable *symbolTable);

#endif 