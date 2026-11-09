#include "bit_utils.hpp"
#include <iostream>
#include <format>

std::int32_t sign_extend_11_bit_immediate(std::int32_t immediate)
{
    //std::cout << "sign_extend_11_bit_immediate\n";
    //std::cout << std::format("Before Sign extension: {:032b}\n", immediate);
    //std::cout << std::format("Before Sign extension: {:x}\n", immediate);
    if (!(immediate & 0x800)) {
        //std::cout << std::format("Result of mask {:032b}\n", immediate & 0x800);
        std::cout << "bit 11 is 0, no extension was performed\n";
        return immediate;
    }
    immediate |= 0xFFFFF000;
    //std::cout << std::format("After Sign extension: {:032b}\n", static_cast<uint32_t>(immediate)); //unsigned to display sign extension
    //std::cout << std::format("After Sign extension: {:x}\n", immediate);
    return immediate;
}

std::int32_t sign_extend_12_bit_immediate(std::int32_t immediate)
{
    //std::cout << "sign_extend_12_bit_immediate\n";
    //std::cout << std::format("Before Sign extension: {:032b}\n", immediate);
    //std::cout << std::format("Before Sign extension: {:x}\n", immediate);
    if (!(immediate & 0x1000)) {
        //std::cout << std::format("Result of mask {:032b}\n", immediate & 0x1000);
        std::cout << "bit 12 is 0, no extension was performed\n";
        return immediate;
    }
    immediate |= 0xFFFFE000;
    //std::cout << std::format("After Sign extension: {:032b}\n", static_cast<uint32_t>(immediate));
    //std::cout << std::format("After Sign extension: {:x}\n", immediate);
    return immediate;
}

std::int32_t sign_extend_20_bit_immediate(std::int32_t immediate)
{
    //std::cout << "sign_extend_20_bit_immediate\n";
    //std::cout << std::format("Before Sign extension: {:032b}\n", immediate);
    //std::cout << std::format("Before Sign extension: {:x}\n", immediate);
    if (!(immediate & 0x100000)) {
        //std::cout << std::format("Result of mask {:032b}\n", immediate & 0x100000);
        std::cout << "bit 20 is 0, no extension was performed\n";
        return immediate;
    }
    immediate |= 0xFFE00000;
    //std::cout << std::format("After Sign extension: {:032b}\n", static_cast<uint32_t>(immediate));
    //std::cout << std::format("After Sign extension: {:x}\n", immediate);
    return immediate;
}