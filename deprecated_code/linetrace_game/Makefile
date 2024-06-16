# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -O2

# Libraries to link against
LIBS = -lwiringPi

# Target Executable
TARGET = linetracer_test

# Source Files
SRCS = linetracer_test.c modified_linetracing.c

# Object Files
OBJS = $(SRCS:.c=.o)

# Header Files
HDRS = modified_linetracing.h

# Default Target
all: $(TARGET)

# Rule to create the target executable
$(TARGET): $(OBJS)
	@echo "Linking object files to create $(TARGET)"
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile source files into object files
%.o: %.c $(HDRS)
	@echo "Compiling $< to $@"
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
