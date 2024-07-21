#ifndef HELPERS_H
#define HELPERS_H

#include <stdbool.h>
#include "definitions.h"

bool is_digit(char c);
bool is_alpha(char c);
const char* token_type_to_string(TokenType type);

#endif 