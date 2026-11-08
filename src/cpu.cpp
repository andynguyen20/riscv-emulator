#include "cpu.hpp"
#include "bit_utils.hpp"
#include <iostream>
#include <format>
#include <stdexcept>

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
    switch (opcode) {
        case 0b0110011: { // R-type
            instr.funct_7 = static_cast<std::uint8_t>((instruction & 0xFE000000) >> 25);
            instr.source_register_2 = static_cast<std::uint8_t>((instruction & 0x01F00000) >> 20);
            instr.source_register_1 = static_cast<std::uint8_t>((instruction & 0x000F8000) >> 15);
            instr.funct_3 = static_cast<std::uint8_t>((instruction & 0x00007000) >> 12);
            instr.destination_register = static_cast<std::uint8_t>((instruction & 0x00000F80) >> 7);
            instr.opcode = opcode;
            instr.print_instruction_members();
            std::cout << "Cpu::decode_instruction: Instruction decoded (R-type instruction)\n";
            break;
        }
        case 0b0010011: case 0b0000011: case 0b1100111:{ // I-type
            std::int32_t imm = static_cast<std::int32_t>((instruction & 0xFFF00000) >> 20); 
            instr.immediate = sign_extend_11_bit_immediate(imm);
            instr.source_register_1 = static_cast<std::uint8_t>((instruction & 0x000F8000) >> 15);
            instr.funct_3 = static_cast<std::uint8_t>((instruction & 0x00007000) >> 12);
            instr.destination_register = static_cast<std::uint8_t>((instruction & 0x00000F80) >> 7);
            instr.opcode = opcode;
            instr.print_instruction_members();
            std::cout << "Cpu::decode_instruction: Instruction decoded (I-type instruction)\n";
            break;
        }
        case 0b0100011: { // S-type
            std::int32_t imm = static_cast<std::int32_t>(((instruction & 0xFE000000) >> 20) | ((instruction & 0xF80) >> 7)); 
            instr.immediate = sign_extend_11_bit_immediate(imm);
            instr.source_register_2 = static_cast<std::uint8_t>((instruction & 0x1F00000) >> 20);
            instr.source_register_1 = static_cast<std::uint8_t>((instruction & 0xF8000) >> 15);
            instr.funct_3 = static_cast<std::uint8_t>((instruction & 0x7000) >> 12);
            instr.opcode = opcode;
            instr.print_instruction_members();
            std::cout << "Cpu::decode_instruction: Instruction decoded (S-type instruction)\n";
            break;
        }
        case 0b1100011: { // B-type
            std::int32_t imm = static_cast<std::int32_t>(((instruction & 0x80000000) >> 19) | ((instruction & 0x7E000000) >> 20) | ((instruction & 0xF00) >> 7) | ((instruction & 0x80) << 4));
            instr.immediate = sign_extend_12_bit_immediate(imm);
            instr.source_register_2 = static_cast<std::uint8_t>((instruction & 0x1F00000) >> 20);
            instr.source_register_1 = static_cast<std::uint8_t>((instruction & 0xF8000) >> 15);
            instr.funct_3 = static_cast<std::uint8_t>((instruction & 0x7000) >> 12);
            instr.opcode = opcode;
            instr.print_instruction_members();
            std::cout << "Cpu::decode_instruction: Instruction decoded (B-type instruction)\n";
            break;
        }
        case 0b1101111: { // J-type
            std::int32_t imm = static_cast<std::int32_t>(((instruction & 0x80000000) >> 11) | ((instruction & 0x7FE00000) >> 20) | ((instruction & 0x100000) >> 9) | (instruction & 0xFF000));
            instr.immediate = imm;
            instr.destination_register = static_cast<std::uint8_t>((instruction & 0xF80) >> 7);
            instr.opcode = opcode;
            instr.print_instruction_members();
            std::cout << "Cpu::decode_instruction: Instruction decoded (J-type instruction)\n";
            break;
        }
        case 0b0110111: case 0b0010111: {// U-type
            std::int32_t imm = static_cast<std::int32_t>((instruction & 0xFFFFF000));
            instr.immediate = sign_extend_20_bit_immediate(imm);
            instr.destination_register = static_cast<std::uint8_t>((instruction & 0xF80) >> 7);
            instr.opcode = opcode;
            instr.print_instruction_members();
            std::cout << "Cpu::decode_instruction: Instruction decoded (U-type instruction)\n";
            break;
        }
        default:
            throw std::runtime_error{"Cpu::decode_instruction: Invalid opcode\n"};
            break;
    }
    return instr;
}