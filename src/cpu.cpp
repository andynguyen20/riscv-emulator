#include "cpu.hpp"
#include <iostream>
#include <format>

Cpu::Cpu(const std::string& bin_path) 
    : memory(bin_path)
{

}

void Cpu::step()
{
    if (halted) {
        std::cout << "Cpu::step(): Cpu Halted\n";
    }

    std::uint32_t instruction = fetch_instruction();

    Instruction instr = decode_instruction(instruction);

    cycles++;

}

void Cpu::run() 
{
    while (!halted && cycles < 21) {
        step();
    }
}

std::uint32_t Cpu::fetch_instruction()
{  
    const std::uint32_t instruction = memory[pc];
    std::cout << std::format("CPU::fetch_instruction(): pc: {:x} | instruction: {:b}\n", pc, instruction);
    pc += 4;
    return instruction;
}


Instruction Cpu::decode_instruction(const std::uint32_t instruction)
{
    std::uint8_t opcode = (instruction & 0x0000007F);
    Instruction instr;
    instr.opcode = opcode;
    switch (opcode) {
        case 0b0110011: {
            instr.funct_7 = static_cast<std::uint8_t>((instruction & 0xFE000000) >> 25);
            instr.source_register_2 = static_cast<std::uint8_t>((instruction & 0x01F00000) >> 20);
            instr.source_register_1 = static_cast<std::uint8_t>((instruction & 0x000F8000) >> 15);
            instr.funct_3 = static_cast<std::uint8_t>((instruction & 0x00007000) >> 12);
            instr.destination_register = static_cast<std::uint8_t>((instruction & 0x00000F80) >> 7);
            instr.print_instruction_members();
            std::cout << "Cpu::decode_instruction: Instruction decoded (R-type instruction)\n";
            break;
        }
        //case 0b0010011:
        //case 0b0000011:
        //case 0b0100011:
        //case 0b1100011:
        //case 0b1101111:
        //case 0b0110111:
        //case 0b0101111:
        default:
            std::cout << "Cpu::decode_instruction: Invalid opcode\n";
            break;
    }
    return instr;
}