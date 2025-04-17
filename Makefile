# Compiler and libraries
CXX = g++
CXXFLAGS = -Wall -Wextra -Wconversion -pedantic -std=c++17 $(shell pkg-config --cflags opencv4) -isystem /usr/include/opencv4
CLIBS = $(shell pkg-config --libs opencv4) -L/home/tjempo/Documents/GIT/darknet -ldarknet -lcrypto -ltesseract
SRC_FOLDER = src
OUTPUT_FOLDER = $(SRC_FOLDER)/output
OUTPUT_FILE = $(OUTPUT_FOLDER)/app

# OS detection (if Windows, append .exe to output)
OS := $(shell uname -s)
ifeq ($(OS), Windows_NT)
	OUTPUT_FILE := $(OUTPUT_FOLDER)/app.exe
endif

# Find all source files in the src folder
SRC_FILES = $(wildcard $(SRC_FOLDER)/*.cpp)

# Object files will be placed in the output folder
OBJ_FILES = $(patsubst $(SRC_FOLDER)/%.cpp, $(OUTPUT_FOLDER)/%.o, $(SRC_FILES))

# Target to build object files and link them
build: $(OUTPUT_FOLDER) $(OUTPUT_FILE)

# Compile each .cpp file into .o in the output folder
$(OUTPUT_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link the object files into the final executable
$(OUTPUT_FILE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILES) $(CLIBS)

# Create the output folder if it doesn't exist
$(OUTPUT_FOLDER):
	mkdir -p $(OUTPUT_FOLDER)

# Run the program from the output folder
run: $(OUTPUT_FILE)
	./$(OUTPUT_FILE) ./src/configs/$(CONFIG)

# Clean up the output folder
clean:
	rm -f $(OUTPUT_FOLDER)/*
ifeq ($(OS), Windows_NT)
	del /Q $(subst /,\,$(OUTPUT_FOLDER))\*
endif