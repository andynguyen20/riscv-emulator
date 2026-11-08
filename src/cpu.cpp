#include "cpu.hpp"
#include <iostream>
#include <bitset>

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

    cycles++;

}

void Cpu::run() 
{
    while (!halted && cycles <= 21) {
        step();
    }
}

std::uint32_t Cpu::fetch_instruction()
{  
    std::uint32_t instruction = memory[pc];
    std::cout << "pc: " << pc << " | instruction: " << std::bitset<32>(instruction) << '\n';
    pc += 4;
    return instruction;
}