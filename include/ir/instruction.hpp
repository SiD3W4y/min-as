#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <tuple>
#include <string>

#include "ir/serializable.hpp"

namespace ir {
    enum ArgType {
        Ref,
        Value
    };

    class Argument : public IrObject {
        public:
            Argument(std::string ref_name);
            Argument(int imm);
            
            void setValue(int imm);
            void setValue(std::string ref);

            std::string getReference();
            int getValue();
            
            ArgType getType();

        private:
            ArgType type;
            int imm_value;
            std::string ref_name;
    };

    class Instruction : public IrObject {
        public: 
            Instruction();
            Instruction(int opcode);

        private:
            std::tuple<Argument, Argument> args;
    };
}

#endif
