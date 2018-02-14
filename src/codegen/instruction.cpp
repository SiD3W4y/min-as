#include <tuple>
#include <string>
#include <ostream>
#include <cstdint>
#include <stdexcept>

#include "codegen/instruction.hpp"

namespace Codegen {

    // Instruction
    Instruction::Instruction() : args(std::tuple<Argument, Argument>({ArgType::Reg, nullptr, 0}, {ArgType::Reg, nullptr, 0})), opcode(-1)
    {
    }

    Instruction::Instruction(int op) : args(std::tuple<Argument, Argument>({ArgType::Reg, nullptr, 0}, {ArgType::Reg, nullptr, 0})), opcode(op)
    {
    }

    void Instruction::setFirst(Argument first)
    {
        std::get<0>(args) = first;
    }

    void Instruction::setSecond(Argument second)
    {
        std::get<1>(args) = second;
    }

    int Instruction::getSize()
    {
        int size = 2; // Starting size (opcode + control byte)

        Argument first_arg = std::get<0>(args);
        Argument second_arg = std::get<1>(args);

        if(first_arg.type == ArgType::Reg)
            size += 2;
        else
            size += 4;

        if(second_arg.type == ArgType::Reg)
            size += 2;
        else
            size += 4;

        return size;
    }

    std::ostream &operator<<(std::ostream &stream, const Instruction &ins)
    {
        if(ins.opcode < 0)
            throw std::runtime_error("Trying to compile uninitialized instruction");

        uint8_t op = ins.opcode;
        
        Argument first_arg = std::get<0>(ins.args);
        Argument second_arg = std::get<1>(ins.args);

        bool first_arg_reg = first_arg.type == ArgType::Reg;
        bool second_arg_reg = second_arg.type == ArgType::Reg;

        uint8_t control_op = second_arg_reg << 1 | first_arg_reg;

        if(first_arg.type == ArgType::Ref || second_arg.type == ArgType::Ref){
            throw std::runtime_error("Trying to compile unresolved reference");
        }

        stream.write(reinterpret_cast<char *>(&op), 1);
        stream.write(reinterpret_cast<char *>(&control_op), 1);

        if(first_arg_reg)
            stream.write(reinterpret_cast<char *>(&first_arg.imm_value), 2);
        else
            stream.write(reinterpret_cast<char *>(&first_arg.imm_value), 4);

        if(second_arg_reg)
            stream.write(reinterpret_cast<char *>(&second_arg.imm_value), 2);
        else
            stream.write(reinterpret_cast<char *>(&second_arg.imm_value), 4);

        return stream;
    }
}
