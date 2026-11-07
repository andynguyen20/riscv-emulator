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
public:
    Memory(std::string bin_path);
};


#endif