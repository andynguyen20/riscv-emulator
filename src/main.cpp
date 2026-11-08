#include "cpu.hpp"
#include "mem.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    try {
        if (argc != 2) {
            throw std::runtime_error("main(): Invalid number of arguments. \nUsage: <executable> <binary_file>");
        }
        Memory RAM(argv[1]);
        RAM.print_instruction_memory();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}