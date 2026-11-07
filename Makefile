CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Iinclude

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))
TARGET = build/riscv-emulator.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir build

clean:
	if exist build rmdir /S /Q build
