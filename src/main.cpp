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
        sign_extend_11_bit_immediate(0xFFF);
        sign_extend_12_bit_immediate(0x1FFF);
        sign_extend_20_bit_immediate(0x1FFFFF);
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}