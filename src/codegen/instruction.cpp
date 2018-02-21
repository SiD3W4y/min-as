#include <tuple>
#include <string>
#include <ostream>
#include <cstdint>
#include <stdexcept>

#include "codegen/instruction.hpp"

namespace Codegen {

    // Instruction
    // Initializing the strings to nullptr causes a segfault so we use an empty string instead
    Instruction::Instruction() : args(std::tuple<Argument, Argument>({ArgType::Reg, "", 0}, {ArgType::Reg, "", 0})), opcode(-1)
    {
    }

    Instruction::Instruction(int op) : args(std::tuple<Argument, Argument>({ArgType::Reg, "", 0}, {ArgType::Reg, "", 0})), opcode(op)
    {
    }
    
    Argument Instruction::getFirst()
    {
        return std::get<0>(args);
    }

    Argument Instruction::getSecond()
    {
        return std::get<1>(args);
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

        if (first_arg.type == ArgType::Reg)
            size += 2;
        else
            size += 4;

        if (second_arg.type == ArgType::Reg)
            size += 2;
        else
            size += 4;

        return size;
    }

    bool Instruction::isPure()
    {
        if (std::get<0>(args).type == ArgType::Ref || std::get<1>(args).type == ArgType::Ref) {
            return false;
        }

        return true;
    }

    std::ostream &operator<<(std::ostream &stream, const Instruction &ins)
    {
        if (ins.opcode < 0)
            throw std::runtime_error("Trying to compile uninitialized instruction");

        uint8_t op = ins.opcode;
        
        Argument first_arg = std::get<0>(ins.args);
        Argument second_arg = std::get<1>(ins.args);

        bool first_arg_reg = first_arg.type == ArgType::Reg;
        bool second_arg_reg = second_arg.type == ArgType::Reg;

        uint8_t control_op = second_arg_reg << 1 | first_arg_reg;

        if (first_arg.type == ArgType::Ref || second_arg.type == ArgType::Ref) {
            throw std::runtime_error("Trying to compile unresolved reference");
        }

        stream.write(reinterpret_cast<char *>(&op), 1);
        stream.write(reinterpret_cast<char *>(&control_op), 1);

        if (first_arg_reg) {
            uint16_t imm = (uint16_t)first_arg.imm_value;
            stream.write(reinterpret_cast<char *>(&imm), 2);
        } else {
            uint32_t imm = (uint32_t)first_arg.imm_value;
            stream.write(reinterpret_cast<char *>(&imm), 4);
        }

        if (second_arg_reg) {
            uint16_t imm = (uint16_t)second_arg.imm_value;
            stream.write(reinterpret_cast<char *>(&imm), 2);
        } else {
            uint32_t imm = (uint32_t)second_arg.imm_value;
            stream.write(reinterpret_cast<char *>(&imm), 4);
        }
        


        return stream;
    }
}
