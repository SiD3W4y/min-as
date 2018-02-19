#include <unordered_map>
#include <string>

#include "codegen/ops.hpp"

namespace Codegen { 
    // TODO : Add call and ret to min-vm
    std::unordered_map<std::string, int> opcodes = {
        {"add", 0x00},
        {"sub", 0x01},
        {"mul", 0x02},
        {"mov", 0x03},
        {"ldr", 0x04},
        {"ldrb", 0x05},
        {"str", 0x06},
        {"strb", 0x07},
        {"push", 0x08},
        {"pop", 0x09},
        {"cmp", 0x0a},
        {"jmp", 0x0b},
        {"jne", 0x0c},
        {"je", 0x0d},
        {"jle", 0x0e},
        {"jbe", 0x0f},
        {"sys", 0x10},
        {"xor", 0x11},
        {"and", 0x12},
        {"or", 0x13},
        {"shr", 0x14},
        {"shl", 0x15},
        {"call", 0x16},
        {"ret", 0x17}
    };

    std::unordered_map<std::string, int> registers = {
        {"A", 0x00},
        {"B", 0x01},
        {"C", 0x02},
        {"D", 0x03},
        {"E", 0x04},
        {"F", 0x05},
        {"BP", 0x06},
        {"SP", 0x07}
    };

}
