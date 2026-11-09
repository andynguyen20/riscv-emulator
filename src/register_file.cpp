#include "register_file.hpp"

#include <cstdint>
#include <iostream>

std::uint32_t RegisterFile::register_read(std::uint8_t source_register) 
{
    return reg_file[source_register];
}


void RegisterFile::register_write(std::uint8_t source_register, std::uint32_t write_data)
{
    if (!source_register) {
        std::cout << "RegisterFile::register_write: Cannot write to register 0, skipping write\n";
        return;
    }
    reg_file[source_register] = write_data;
}