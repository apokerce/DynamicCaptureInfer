# Makefile for Dynamic Capture Infer

CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra
LIBS := `pkg-config --cflags --libs opencv4` -pthread

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET := output

# Default target
all: $(TARGET)

# Link objects to create executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

# Compile source files to objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)/*
	rm -f $(TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild