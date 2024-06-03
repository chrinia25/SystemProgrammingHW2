# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -pthread `pkg-config --cflags opencv4` -lwiringPi
LDFLAGS = -lpthread -lwiringPi `pkg-config --libs opencv4`

# Source Files
SRC = main.cpp modified_linetracing.c

# Object Files
OBJ = main.o modified_linetracing.o

# Executable
TARGET = Client

# Default Target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile main.cpp
main.o: main.cpp modified_linetracing.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compile modified_linetracing.c
modified_linetracing.o: modified_linetracing.c modified_linetracing.h
	$(CXX) $(CXXFLAGS) -c modified_linetracing.c

# Clean up the build
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
