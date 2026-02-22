# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Output executable name
TARGET = context_switch.bin

# Source files
SRCS = src/context_switch.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile the source files into object files
%.o: %.c src/context_switch.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
