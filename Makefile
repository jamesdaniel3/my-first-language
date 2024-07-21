# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Source files
SRCS = main.c helpers.c lexer.c parser.c symbol_table.c

# Header files
HEADERS = definitions.h helpers.h lexer.h parser.h symbol_table.h

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = interpreter

# Default target
all: $(EXEC)

# Rule to link object files and create the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean