#include "instruction.hpp"
#include <iostream>


void Instruction::print_instruction_members()
{
    std::cout << "Instruction::print_instruction_members: immediate = " << static_cast<unsigned int>(immediate) << '\n';
    std::cout << "Instruction::print_instruction_members: funct_7 = " << static_cast<unsigned int>(funct_7) << '\n';
    std::cout << "Instruction::print_instruction_members: source_register_2 = " << static_cast<unsigned int>(source_register_2) << '\n';
    std::cout << "Instruction::print_instruction_members: source_register_1 = " << static_cast<unsigned int>(source_register_1) << '\n';
    std::cout << "Instruction::print_instruction_members: funct_3 = " << static_cast<unsigned int>(funct_3) << '\n';
    std::cout << "Instruction::print_instruction_members: destination_register = " << static_cast<unsigned int>(destination_register) << '\n';
    std::cout << "Instruction::print_instruction_members: opcode = " << static_cast<unsigned int>(opcode) << '\n';
}