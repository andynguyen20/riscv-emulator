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
        cpu.print_data_debug(100);
        cpu.print_data_debug(104);
        cpu.print_data_debug(108);
        cpu.print_data_debug(112);
        cpu.print_data_debug(116);
        cpu.print_data_debug(120);
        cpu.print_data_debug(124);
        cpu.print_data_debug(128);
        cpu.print_data_debug(132);
        cpu.print_data_debug(136);
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}