CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Iinclude -MMD -MP

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))
DEP = $(OBJ:.o=.d)
TARGET = build/riscv-emulator.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build

-include %(DEP)
