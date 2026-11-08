#ifndef REG_FILE_H
#define REG_FILE_H

#include <array>
#include <cstdint>

class RegisterFile {
private:
    std::array<std::uint32_t, 32> reg_file{};
public:
    std::uint32_t register_read(std::uint32_t source_register);
    void register_write(std::uint32_t source_register, std::uint32_t write_data);
};

#endif