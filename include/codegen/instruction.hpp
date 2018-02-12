#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <tuple>
#include <string>

namespace codegen {
    enum ArgType {
        Ref,
        Reg,
        Value
    };

    class Argument {
        public:
            Argument(std::string ref_name);
            Argument(int imm);
            
            void setValue(int imm);
            void setValue(std::string ref);

            std::string getReference();
            int getValue();
            
            ArgType getType();
            int getSize();

        private:
            ArgType type;
            int imm_value;
            std::string ref_name = nullptr;
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
        private:
            std::tuple<Argument, Argument> args;
            int opcode = 0;
    };
}

#endif
