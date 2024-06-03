# Define the compilers
CC = gcc
CXX = g++

# Define the flags for the compiler
CFLAGS = -Wall -pthread
CXXFLAGS = -Wall -pthread

# Define the OpenCV and wiringPi libraries
OPENCV_LIBS = `pkg-config --cflags --libs opencv4`
WIRINGPI_LIBS = -lwiringPi

# Define the source files and object files
SRCS = main.cpp modified_linetracing.c  
OBJS = main.o modified_linetracing.o 
# Define the header files
HDRS = client_network.h modified_linetracing.h qrCode_C.h

# Define the output executable
TARGET = Client

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(OPENCV_LIBS) $(WIRINGPI_LIBS)

# Rule to build object files from source files
main.o: main.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) $(OPENCV_LIBS) -c $< -o $@

modified_linetracing.o: modified_linetracing.c modified_linetracing.h
	$(CC) $(CFLAGS) $(WIRINGPI_LIBS) -c $< -o $@

client_network.o: client_network.c client_network.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
