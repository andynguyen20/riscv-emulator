#include "cpu.hpp"
#include "bit_utils.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    try {
        if (argc != 2) {
            throw std::runtime_error("main(): Invalid number of arguments. \nUsage: <executable> <binary_file>");
        }
        Cpu cpu(argv[1]);
        cpu.run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}