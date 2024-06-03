# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -pthread `pkg-config --cflags opencv4` -lwiringPi
LDFLAGS = -lpthread -lwiringPi `pkg-config --libs opencv4`

# Source Files
SRC = main.cpp client_network.c modified_linetracing.c

# Object Files
OBJ = main.o client_network.o modified_linetracing.o

# Executable
TARGET = Client

# Default Target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile main.cpp
main.o: main.cpp client_network.h modified_linetracing.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compile client_network.c
client_network.o: client_network.c client_network.h
	$(CXX) $(CXXFLAGS) -c client_network.c

# Compile modified_linetracing.c
modified_linetracing.o: modified_linetracing.c modified_linetracing.h
	$(CXX) $(CXXFLAGS) -c modified_linetracing.c

# Clean up the build
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
