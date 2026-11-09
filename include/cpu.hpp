#ifndef CPU_H
#define CPU_H

#include "memory.hpp"
#include "register_file.hpp"
#include "instruction.hpp"
#include <string>
#include <cstdint>

class Cpu {
private:
    Memory memory;
    RegisterFile rf;
    std::uint32_t pc = 0;
    std::uint32_t pc_next = pc + 4;
    std::uint64_t cycles = 0;
    bool halted = false;
public:
    explicit Cpu(const std::string& bin_path);
    void step();
    void run();
    std::uint32_t fetch_instruction();
    Instruction decode_instruction(const std::uint32_t instruction);
    void execute_instruction(const Instruction& instruction);
    void print_data_debug(std::uint32_t index);
};

#endif