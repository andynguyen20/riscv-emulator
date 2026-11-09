#include "cpu.hpp"
#include "bit_utils.hpp"
#include "memory.hpp"
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

    execute_instruction(instr);

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
    const std::uint32_t instruction = memory.data_read(pc);
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

void Cpu::execute_instruction(const Instruction& instruction)
{
    std::uint32_t rd = 0;
    std::uint32_t rd_a = instruction.destination_register;
    std::uint32_t rs1 = rf.register_read(instruction.source_register_1);
    std::uint32_t rs2 = rf.register_read(instruction.source_register_2);
    std::uint32_t imm = instruction.immediate;
    switch (instruction.opcode) {
        case 0b0110011: {
            switch (instruction.funct_3) {
                case 0x0: {
                    if (instruction.funct_7 == 0x00) {
                        rd = rs1 + rs2;
                        rf.register_write(rd_a, rd);
                        std::cout << "Cpu::execute_instruction: rd: " << rd << " = rs1: " << rs1 << " + rs2: " << rs2 << '\n';
                    }
                    if (instruction.funct_7 == 0x20) {
                        rd = rs1 - rs2;
                        rf.register_write(rd_a, rd);
                        std::cout << "Cpu::execute_instruction: rd: " << rd << " = rs1: " << rs1 << " - rs2: " << rs2 << '\n';
                    }
                    break;
                }
                case 0x6: {
                    rd = rs1 | rs2;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: rd: " << rd << " = rs1: " << rs1 << " | rs2: " << rs2 << '\n';
                    break;
                }
                case 0x7: {
                    rd = rs1 & rs2;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: rd: " << rd << " = rs1: " << rs1 << " & rs2: " << rs2 << '\n';
                    break;
                }
                case 0x2: {
                    rd = (rs1 < rs2) ? 1 : 0;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: rd: " << rd << " = (rs1: " << rs1 << " < rs2: " << rs2 << ") ? 1 : 0\n";
                    break;
                }
            }
            break;
        }
        case 0b0010011: {
            switch (instruction.funct_3) {
                case 0x0: {
                    rd = rs1 + imm;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: rd: " << rd << " = (rs1: " << rs1 << " + imm: " << imm << '\n';
                    break;
                }
            }
            break;
        }
        case 0b0000011: {
            switch (instruction.funct_3) {
                case 0x2: {
                    rd = memory.data_read(MemoryMap::DATA_START + rs1 + imm);
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: rd: " << rd << " = M[rs1: " << rs1 << " + imm: " << imm << "][0:31]\n";
                    break;
                }
            }
            break;
        }
        case 0b0100011: {
            switch (instruction.funct_3) {
                case 0x2: {
                    memory.data_write(rs1 + imm, rs2);
                    std::cout << "Cpu::execute_instruction: M[rs1: " << rs1 << " + imm: " << imm << "[0:31] = rs2: " << rs2 << "[0:31]\n";
                    break;
                }
            }
            break;
        }
        case 0b1100011: {
            switch (instruction.funct_3) {
                case 0x0: {
                    if (rs1 == rs2) {
                        pc = pc - 4 + imm;
                        std::cout << "Cpu::execute_instruction: if (rs1: " << rs1 << " == rs2: " << rs1 << ") PC += imm: " << imm << "\n";
                        break;
                    }
                    std::cout << "Cpu::execute_instruction: rs1: " << rs1 << " != rs2: " << rs1 << " PC += 0\n";
                    break;
                }
            }
            break;
        }
        case 0b1101111: {
            rd = pc;
            rf.register_write(rd_a, rd);
            pc = pc - 4 + imm;
            std::cout << "Cpu::execute_instruction: rd: " << rd << " = pc: " << pc << " + 4; pc: " << pc << "+= imm: " << imm << '\n';
            break;
        }
        default: {
            std::cout << "Cpu::execute_instruction: Invalid opcode\n";
            break;
        }
    }
}

void Cpu::print_data_debug(std::uint32_t index)
{
    std::uint32_t value = memory.data_read(MemoryMap::DATA_START + index);
    std::cout << "Cpu::print_memory_debug: M[" << index << "] = " << value << '\n';
}