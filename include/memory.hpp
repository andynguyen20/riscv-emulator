#ifndef MEM_H
#define MEM_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace MemoryMap {
    constexpr std::size_t RAM_SIZE = 0xFFFF;
    constexpr std::uint32_t INSTR_START = 0x0000;
    constexpr std::uint32_t INSTR_END = 0x8000;
    constexpr std::uint32_t DATA_START = 0x8000;
    constexpr std::uint32_t DATA_END = 0xF000;
    constexpr std::uint32_t STACK_START = 0xF000;
    constexpr std::uint32_t STACK_END = 0x10000;
};

class Memory {
private:
    std::array<std::uint8_t, MemoryMap::RAM_SIZE> RAM{};
    std::uint64_t instruction_count = 0;
public:
    explicit Memory(std::string bin_path);
    void print_instruction_memory();
    std::uint32_t data_read(std::uint32_t index);
    void data_write(std::uint32_t index, std::uint32_t data);
    void data_write_byte(std::uint32_t index, std::uint8_t data);
    void data_write_half(std::uint32_t index, std::uint16_t data);
    std::uint64_t return_instruction_count();
};


#endif