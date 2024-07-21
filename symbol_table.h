#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "definitions.h"

void add_or_update_variable(SymbolTable *symbolTable, const char *name, double value);
double get_variable_value(SymbolTable *symbolTable, const char *name);

#endif 