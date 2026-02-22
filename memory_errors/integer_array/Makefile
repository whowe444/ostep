# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Output executable name
TARGET = integer_array.bin

# Source files
SRCS = src/integer_array.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile the source files into object files
%.o: %.c src/integer_array.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
