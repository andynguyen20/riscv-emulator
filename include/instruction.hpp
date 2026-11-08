#include <cstdint>

struct Instruction {
private:
public:
    std::uint32_t immediate = 0;
    std::uint8_t source_register_1 = 0;
    std::uint8_t source_register_2 = 0;
    std::uint8_t funct_7 = 0;
    std::uint8_t funct_3 = 0;
    std::uint8_t destination_register = 0;
    std::uint8_t opcode = 0;
    void print_instruction_members();
};

