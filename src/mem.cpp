#include "mem.hpp"
#include <fstream>
#include <stdexcept>

Memory::Memory(std::string bin_path)
{
    std::ifstream ifs(bin_path, std::ios_base::binary);

    if (!ifs) {
        throw std::runtime_error("Memory::Memory(): Failed to open binary file: " + bin_path);
    }

    
}
