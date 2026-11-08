#include "mem.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <bitset>

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
        
        std::cout << std::bitset<32>(instruction) << '\n';
    }
}
