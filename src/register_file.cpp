#include "register_file.hpp"

#include <cstdint>

std::uint32_t RegisterFile::register_read(std::uint32_t source_register) 
{
    return reg_file[source_register];
}


void RegisterFile::register_write(std::uint32_t source_register, std::uint32_t write_data)
{
    reg_file[source_register] = write_data;
}