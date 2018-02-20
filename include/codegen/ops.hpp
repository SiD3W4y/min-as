#ifndef OPS_HPP
#define OPS_HPP

#include <unordered_map>
#include <string>

namespace Codegen {

    struct OpInfo {
        int opcode;
        int arg_count;
    };

    extern std::unordered_map<std::string, OpInfo> opcodes;
    extern std::unordered_map<std::string, int> registers;
}
#endif
