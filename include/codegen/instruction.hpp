#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <tuple>
#include <string>
#include <cstdint>
#include <ostream>

namespace Codegen {
    enum ArgType {
        Ref,
        Reg,
        Value
    };

    struct Argument {
        ArgType type;
        std::string value;
        uint32_t imm_value;
    };

    class Instruction {
        public: 
            Instruction();
            Instruction(int opcode);
            
            int getOpcode();
            int getSize();
            bool isPure(); // Are all references fixed = ready for compilation

            void setFirst(Argument first);
            void setSecond(Argument second);
        
            friend std::ostream& operator<<(std::ostream& stream, const Instruction &ins);
        private:
            std::tuple<Argument, Argument> args;
            int opcode = 0;
    };
}

#endif
