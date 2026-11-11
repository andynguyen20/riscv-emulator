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

    pc = pc_next;
    pc_next = pc + 4;

    cycles++;

}

void Cpu::run() 
{
    while (!halted && cycles < memory.return_instruction_count()) {
        step();
    }
}

std::uint32_t Cpu::fetch_instruction()
{  
    const std::uint32_t instruction = memory.data_read(pc);
    std::cout << std::format("CPU::fetch_instruction(): pc: {:x} | instruction: {:b}\n", pc, instruction);
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
            //instr.print_instruction_members();
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
            //instr.print_instruction_members();
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
            //instr.print_instruction_members();
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
            //instr.print_instruction_members();
            std::cout << "Cpu::decode_instruction: Instruction decoded (B-type instruction)\n";
            break;
        }
        case 0b1101111: { // J-type
            std::int32_t imm = static_cast<std::int32_t>(((instruction & 0x80000000) >> 11) | ((instruction & 0x7FE00000) >> 20) | ((instruction & 0x100000) >> 9) | (instruction & 0xFF000));
            instr.immediate = imm;
            instr.destination_register = static_cast<std::uint8_t>((instruction & 0xF80) >> 7);
            instr.opcode = opcode;
            //instr.print_instruction_members();
            std::cout << "Cpu::decode_instruction: Instruction decoded (J-type instruction)\n";
            break;
        }
        case 0b0110111: case 0b0010111: {// U-type
            std::int32_t imm = static_cast<std::int32_t>((instruction & 0xFFFFF000));
            instr.immediate = sign_extend_20_bit_immediate(imm);
            instr.destination_register = static_cast<std::uint8_t>((instruction & 0xF80) >> 7);
            instr.opcode = opcode;
            //instr.print_instruction_members();
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
                    if (instruction.funct_7 == 0x00) { // ADD
                        rd = rs1 + rs2;
                        rf.register_write(rd_a, rd);
                        std::cout << "Cpu::execute_instruction: ADD | rd: " << rd << " = rs1: " << rs1 << " + rs2: " << rs2 << '\n';
                        break;
                    }
                    if (instruction.funct_7 == 0x20) { // SUB
                        rd = rs1 - rs2;
                        rf.register_write(rd_a, rd);
                        std::cout << "Cpu::execute_instruction: SUB | rd: " << rd << " = rs1: " << rs1 << " - rs2: " << rs2 << '\n';
                        break;
                    }
                }
                case 0x4: { // XOR
                    rd = rs1 ^ rs2;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: XOR | rd: " << rd << " = rs1: " << rs1 << " ^ rs2: " << rs2 << '\n';
                    break;
                }
                case 0x6: { // OR
                    rd = rs1 | rs2;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: OR | rd: " << rd << " = rs1: " << rs1 << " | rs2: " << rs2 << '\n';
                    break;
                }
                case 0x7: { // AND
                    rd = rs1 & rs2;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: AND | rd: " << rd << " = rs1: " << rs1 << " & rs2: " << rs2 << '\n';
                    break;
                }
                case 0x1: { // Shift Left Logical
                    rd = rs1 << (rs2 & 0x1F);
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Shift Left Logical | rd: " << rd << " = rs1: " << rs1 << " << rs2: " << rs2 << '\n';
                    break;
                }
                case 0x5: {
                    if (instruction.funct_7 == 0x00) { // Shift Right Logical
                        rd = rs1 >> (rs2 & 0x1F);
                        rf.register_write(rd_a, rd);
                        std::cout << "Cpu::execute_instruction: Shift Right Logical | rd: " << rd << " = rs1: " << rs1 << " >> rs2: " << rs2 << '\n';
                        break;
                    }
                    if (instruction.funct_7 == 0x20) { // Shift Left Arith*
                        rd = static_cast<std::uint32_t>((static_cast<std::int32_t>(rs1) >> (rs2 & 0x1F)));
                        rf.register_write(rd_a, rd);
                        std::cout << "Cpu::execute_instruction: Shift Right Arith* | rd: " << static_cast<std::int32_t>(rd) << " = rs1: " << static_cast<std::int32_t>(rs1) << " >> rs2: " << rs2 << '\n';
                        break;
                    }
                }
                case 0x2: { // Set Less Than
                    rd = (static_cast<std::int32_t>(rs1) < static_cast<std::int32_t>(rs2)) ? 1 : 0;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Set Less Than | rd: " << rd << " = (rs1: " << static_cast<std::int32_t>(rs1) << " < rs2: " << static_cast<std::int32_t>(rs2) << ") ? 1 : 0\n";
                    break;
                }
                case 0x3: { //Set Less Than (U)
                    rd = rs1 < rs2 ? 1 : 0;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Set Less Than (U) | rd: " << rd << " = (rs1: " << rs1 << " < rs2: " << rs2 << ") ? 1 : 0\n";
                    break;
                }
            }
            break;
        }
        case 0b0010011: {
            switch (instruction.funct_3) {
                case 0x0: { // ADD Immediate
                    rd = rs1 + imm;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: ADD Immediate | rd: " << rd << " = rs1: " << rs1 << " + imm: " << imm << '\n';
                    break;
                }
                case 0x4: { // XOR Immediate
                    rd = rs1 ^ imm;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: XOR Immediate | rd: " << rd << " = rs1: " << rs1 << " ^ imm: " << imm << '\n';
                    break;
                }
                case 0x6: { // OR Immediate
                    rd = rs1 | imm;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: OR Immediate | rd: " << rd << " = rs1: " << rs1 << " | imm: " << imm << '\n';
                    break;
                }
                case 0x7: { // AND Immediate
                    rd = rs1 & imm;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: AND Immediate | rd: " << rd << " = rs1: " << rs1 << " & imm: " << imm << '\n';
                    break;
                }
                case 0x1: { // Shift Left Logical Immediate
                    rd = rs1 << (imm & 0x1F);
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Shift Right Logical Immediate | rd: " << rd << " = rs1: " << rs1 << " << imm: " << imm << '\n';
                    break;
                }
                case 0x5: { 
                    if (instruction.funct_7 == 0x00) {// Shift Right Logical Immediate
                    rd = rs1 >> (imm & 0x1F);
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Shift Right Logical Immediate | rd: " << rd << " = rs1: " << rs1 << " >> imm: " << imm << '\n';
                    break;
                    }
                    if (instruction.funct_7 == 0x20) {// Shift Right Arithmetic Immediate
                    rd = static_cast<std::uint32_t>(static_cast<std::int32_t>(rs1) >> (imm & 0x1F));
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Shift Right Arithmetic Immediate | rd: " << static_cast<std::int32_t>(rd) << " = rs1: " << static_cast<std::int32_t>(rs1) << " >> imm: " << imm << '\n';
                    break;
                    }
                }
                case 0x2: { // Set Less Than Imm
                    rd = (static_cast<std::int32_t>(rs1) < static_cast<std::int32_t>(imm)) ? 1 : 0;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Set Less Than Imm | rd: " << rd << " = (rs1: " << static_cast<std::int32_t>(rs1) << " < imm: " << static_cast<std::int32_t>(imm) << ") ? 1 : 0\n";
                    break;
                }
                case 0x3: { //Set Less Than Imm (U)
                    rd = rs1 < imm ? 1 : 0;
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Set Less Than Imm (U) | rd: " << rd << " = (rs1: " << rs1 << " < imm: " << imm << ") ? 1 : 0\n";
                    break;
                }
            }
            break;
        }
        case 0b0000011: {
            switch (instruction.funct_3) {
                case 0x0: { // Load Byte
                    rd = static_cast<std::uint32_t>(static_cast<std::int8_t>(memory.data_read(MemoryMap::DATA_START + rs1 + imm) & 0xFF));
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Load Byte | rd: " << rd << " = M[rs1: " << static_cast<std::int32_t>(rs1) << " + imm: " << imm << "][0:7]\n";
                    break;
                }
                case 0x1: { // Load Half
                    rd = static_cast<std::uint32_t>(static_cast<std::int16_t>(memory.data_read(MemoryMap::DATA_START + rs1 + imm) & 0xFFFF));
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Load Half | rd: " << rd << " = M[rs1: " << static_cast<std::int32_t>(rs1) << " + imm: " << imm << "][0:15]\n";
                    break;
                }
                case 0x2: { // Load Word
                    rd = memory.data_read(MemoryMap::DATA_START + rs1 + imm);
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Load Word | rd: " << rd << " = M[rs1: " << rs1 << " + imm: " << imm << "][0:31]\n";
                    break;
                }
                case 0x4: { // Load Byte (U)
                    rd = (memory.data_read(MemoryMap::DATA_START + rs1 + imm) & 0xFF);
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Load Byte (U) | rd: " << rd << " = M[rs1: " << rs1 << " + imm: " << imm << "][0:7]\n";
                    break;
                }
                case 0x5: { // Load Half (U)
                    rd = (memory.data_read(MemoryMap::DATA_START + rs1 + imm) & 0xFFFF);
                    rf.register_write(rd_a, rd);
                    std::cout << "Cpu::execute_instruction: Load Half | rd: " << rd << " = M[rs1: " << rs1 << " + imm: " << imm << "][0:15]\n";
                    break;
                }
            }
            break;
        }
        case 0b0100011: {
            switch (instruction.funct_3) {
                case 0x0: { // Store Byte
                    memory.data_write_byte(rs1 + imm, static_cast<std::uint8_t>(rs2 &0xFF));
                    std::cout << "Cpu::execute_instruction: Store Byte | M[rs1: " << rs1 << " + imm: " << imm << "[0:7] = rs2: " << rs2 << "[0:7]\n";
                    break;
                }
                case 0x1: { // Store Half
                    memory.data_write_half(rs1 + imm, static_cast<std::uint16_t>(rs2 &0xFFFF));
                    std::cout << "Cpu::execute_instruction: Store Half | M[rs1: " << rs1 << " + imm: " << imm << "[0:15] = rs2: " << rs2 << "[0:15]\n";
                    break;
                }
                case 0x2: { // Store Word
                    memory.data_write(rs1 + imm, rs2);
                    std::cout << "Cpu::execute_instruction: Store Word | M[rs1: " << rs1 << " + imm: " << imm << "[0:31] = rs2: " << rs2 << "[0:31]\n";
                    break;
                }
            }
            break;
        }
        case 0b1100011: {
            switch (instruction.funct_3) {
                case 0x0: { // Branch ==
                    if (rs1 == rs2) { 
                        pc_next = pc + imm;
                        std::cout << "Cpu::execute_instruction: Branch == | if (rs1: " << rs1 << " == rs2: " << rs2 << ") PC += imm: " << imm << "\n";
                        break;
                    }
                    std::cout << "Cpu::execute_instruction: Branch == | rs1: " << rs1 << " != rs2: " << rs2 << " PC += 0\n";
                    break;
                }
                case 0x1: { // Branch !=
                    if (rs1 != rs2) {
                        pc_next = pc + imm;
                        std::cout << "Cpu::execute_instruction: Branch != | if (rs1: " << rs1 << " != rs2: " << rs2 << ") PC += imm: " << imm << "\n";
                        break;
                    }
                    std::cout << "Cpu::execute_instruction: Branch != | rs1: " << rs1 << " == rs2: " << rs2 << " PC += 0\n";
                    break;
                }
                case 0x4: { // Branch <
                    if (static_cast<std::int32_t>(rs1) < static_cast<std::int32_t>(rs2)) {
                        pc_next = pc + imm;
                        std::cout << "Cpu::execute_instruction: Branch < | if (rs1: " << static_cast<std::int32_t>(rs1) << " < rs2: " << static_cast<std::int32_t>(rs2) << ") PC += imm: " << imm << "\n";
                        break;
                    }
                    std::cout << "Cpu::execute_instruction: Branch < | rs1: " << static_cast<std::int32_t>(rs1) << " >= rs2: " << static_cast<std::int32_t>(rs2) << " PC += 0\n";
                    break;
                }
                case 0x5: { // Branch >=
                    if (static_cast<std::int32_t>(rs1) >= static_cast<std::int32_t>(rs2)) {
                        pc_next = pc + imm;
                        std::cout << "Cpu::execute_instruction: Branch >= | if (rs1: " << static_cast<std::int32_t>(rs1) << " >= rs2: " << static_cast<std::int32_t>(rs2) << ") PC += imm: " << imm << "\n";
                        break;
                    }
                    std::cout << "Cpu::execute_instruction: Branch >= | rs1: " << static_cast<std::int32_t>(rs1) << " < rs2: " << static_cast<std::int32_t>(rs2) << " PC += 0\n";
                    break;
                }
                case 0x6: { // Branch < (U)
                    if (rs1 < rs2) {
                        pc_next = pc + imm;
                        std::cout << "Cpu::execute_instruction: Branch < (U) | if (rs1: " << rs1 << " < rs2: " << rs2 << ") PC += imm: " << imm << "\n";
                        break;
                    }
                    std::cout << "Cpu::execute_instruction: Branch >= (U) | rs1: " << rs1 << " >= rs2: " << rs2 << " PC += 0\n";
                    break;
                }
                case 0x7: { // Branch >= (U)
                    if (rs1 >= rs2) {
                        pc_next = pc + imm;
                        std::cout << "Cpu::execute_instruction: Branch >= (U) | if (rs1: " << rs1 << " >= rs2: " << rs2 << ") PC += imm: " << imm << "\n";
                        break;
                    }
                    std::cout << "Cpu::execute_instruction: Branch >= (U) | rs1: " << rs1 << " < rs2: " << rs2 << " PC += 0\n";
                    break;
                }
            }
            break;
        }
        case 0b1101111: {
            rd = pc + 4;
            rf.register_write(rd_a, rd);
            pc_next = pc + imm;
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