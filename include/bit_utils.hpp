#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <cstdint>

std::int32_t sign_extend_11_bit_immediate(std::int32_t immediate);
std::int32_t sign_extend_12_bit_immediate(std::int32_t immediate);
std::int32_t sign_extend_20_bit_immediate(std::int32_t immediate);

#endif