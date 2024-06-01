# Makefile for line tracer robot

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2 -lwiringPi

# Target executable
TARGET = linetracer

# Source files
SRCS = main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target to build the executable
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile the source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the build directory
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean
