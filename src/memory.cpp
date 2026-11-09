#include "memory.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <format>

Memory::Memory(std::string bin_path)
{
    std::ifstream ifs(bin_path, std::ios_base::binary);

    if (!ifs) {
        throw std::runtime_error("Memory::Memory(): Failed to open binary file: " + bin_path);
    }

    std::size_t address = 0;
    std::array<uint8_t, 4> bytes{};

    while (ifs.read(reinterpret_cast<char*>(bytes.data()), bytes.size())) {
        if (address + bytes.size() > MemoryMap::INSTR_END) {
            throw std::runtime_error("Memory::Memory(): Instruction memory overflow");
        }
        RAM[address++] = bytes[3];
        RAM[address++] = bytes[2];
        RAM[address++] = bytes[1];
        RAM[address++] = bytes[0];
    } 
}

void Memory::print_instruction_memory()
{
    std::uint32_t instruction = 0;
    for (std::size_t i = MemoryMap::INSTR_START; i < MemoryMap::INSTR_START + 21 * 4; i += 4) {
        instruction = static_cast<std::uint32_t>(RAM[i]) | 
            static_cast<std::uint32_t>(RAM[i + 1] << 8) |
            static_cast<std::uint32_t>(RAM[i + 2] << 16) |
            static_cast<std::uint32_t>(RAM[i + 3] << 24); 
        
        std::cout << std::format("{:b}\n", instruction);
    }
}

std::uint32_t Memory::data_read(std::uint32_t index)
{
    return static_cast<std::uint32_t>(RAM[index])
        | (static_cast<std::uint32_t>(RAM[index + 1]) << 8)
        | (static_cast<std::uint32_t>(RAM[index + 2]) << 16)
        | (static_cast<std::uint32_t>(RAM[index + 3]) << 24);
}


void Memory::data_write(std::uint32_t index, std::uint32_t data)
{
    std::uint8_t byte_0 = static_cast<uint8_t>(data & 0xFF);
    std::uint8_t byte_1 = static_cast<uint8_t>((data & 0xFF00) >> 8);
    std::uint8_t byte_2 = static_cast<uint8_t>((data & 0xFF0000) >> 16);
    std::uint8_t byte_3 = static_cast<uint8_t>((data & 0xFF000000) >> 24);
    RAM[MemoryMap::DATA_START + index] = byte_0;
    RAM[MemoryMap::DATA_START + index + 1] = byte_1;
    RAM[MemoryMap::DATA_START + index + 2] = byte_2;
    RAM[MemoryMap::DATA_START + index + 3] = byte_3;
}