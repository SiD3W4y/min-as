#include <unordered_map>
#include <string>

#include "codegen/ops.hpp"

namespace Codegen { 
    // TODO : Add call and ret to min-vm
    std::unordered_map<std::string, OpInfo> opcodes = {
        {"add", {0x00, 2}},
        {"sub", {0x01, 2}},
        {"mul", {0x02, 2}},
        {"mov", {0x03, 2}},
        {"ldr", {0x04, 2}},
        {"ldrb", {0x05, 2}},
        {"str", {0x06, 2}},
        {"strb", {0x07, 2}},
        {"push", {0x08, 1}},
        {"pop", {0x09, 1}},
        {"cmp", {0x0a, 2}},
        {"jmp", {0x0b, 1}},
        {"jne", {0x0c, 1}},
        {"je", {0x0d, 1}},
        {"jle", {0x0e, 1}},
        {"jbe", {0x0f, 1}},
        {"sys", {0x10, 0}},
        {"xor", {0x11, 2}},
        {"and", {0x12, 2}},
        {"or", {0x13, 2}},
        {"shr", {0x14, 2}},
        {"shl", {0x15, 2}},
        {"call", {0x16, 1}},
        {"ret", {0x17, 0}}
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
