#include <vector>
#include <tuple>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "utils/convert.hpp"
#include "codegen/builder.hpp"
#include "codegen/instruction.hpp"
#include "codegen/ops.hpp"
#include "asm/lexer.hpp"
#include "asm/token.hpp"


namespace Codegen {

    Builder::Builder(Asm::Lexer lex) : lexer(lex)
    {
    }

    void Builder::check_type(Asm::Token tk, Asm::TokenType target)
    {
        // check_type needs to support custom error messages for better error reporting
        if(tk.getType() != target){
            throw Asm::CodeException("Wrong token type", lexer.getLine(tk.getRow()), tk);
        }
    }

    void Builder::parseStringDecl()
    {
        Asm::Token name = lexer.getNext();
        Asm::Token user_string = lexer.getNext();

        check_type(user_string, Asm::TokenType::String);

        symbols[name.getValue()] = fd.tellg();

        fd << user_string.getValue();
        fd << '\x00';
    }

    void Builder::parseBytesDecl()
    {
        // TODO : Parse bytes to uint -> write bytes to fd
        std::vector<char> bytes;
        
        Asm::Token bt_name = lexer.getNext();
        Asm::Token brack = lexer.getNext();
        
        symbols[bt_name.getValue()] = fd.tellp();

        check_type(brack, Asm::TokenType::Lbrack);

        Asm::Token current = lexer.getNext();

        while (current.getType() != Asm::TokenType::Rbrack) {
            check_type(current, Asm::TokenType::Value);

            bytes.push_back(Utils::str_to_byte(current.getValue()));
            current = lexer.getNext();

            if(current.getType() == Asm::TokenType::Sep)
                current = lexer.getNext(); // Skipping commas
        }

        std::vector<char>::iterator it;
        
        // Maybe I should do it in a single loop ?
        for (it = bytes.begin(); it != bytes.end(); ++it) {
            fd << *it;
        }
    }

    void Builder::parseNumDecl()
    {
        Asm::Token n_name = lexer.getNext();
        Asm::Token n_value = lexer.getNext();

        check_type(n_name, Asm::TokenType::Symbol);
        check_type(n_value, Asm::TokenType::Value);

        symbols[n_name.getValue()] = fd.tellg();

        int value = Utils::str_to_int(n_value.getValue());

        fd.write(reinterpret_cast<char *>(&value), 4);
    }

    void Builder::parseSlotDecl()
    {
        Asm::Token sl_name = lexer.getNext();
        Asm::Token sl_value = lexer.getNext();
        
        check_type(sl_name, Asm::TokenType::Symbol);
        check_type(sl_value, Asm::TokenType::Value);

        symbols[sl_name.getValue()] = fd.tellg();

        writeStub(Utils::str_to_int(sl_value.getValue()));
    }

    void Builder::parseFnDecl()
    {
        // TODO : Handle error handling for multiple definition
        Asm::Token fn_name = lexer.getNext();
        symbols[fn_name.getValue()] = fd.tellg();
    }

    Argument Builder::processArg()
    {
        Asm::Token arg_tk = lexer.getNext();
        Argument arg;
        
        switch(arg_tk.getType())
        {
            case Asm::TokenType::Value:
                arg.type = ArgType::Value;
                arg.imm_value = Utils::str_to_int(arg_tk.getValue());
                break;
            case Asm::TokenType::Ref:
                arg.type = ArgType::Ref;
                arg.value = arg_tk.getValue();
                break;
            case Asm::TokenType::Reg:
                arg.type = ArgType::Reg;

                if (registers.find(arg_tk.getValue()) == registers.end())
                    throw std::runtime_error("Could not find register : "+arg_tk.getValue());

                arg.imm_value = registers[arg_tk.getValue()];
                break;
            default:
                throw std::runtime_error("Not a valid argument -> "+arg_tk.getValue());
                break;
        }

        return arg;
    }

    void Builder::parseOpStmt(Asm::Token op)
    {
        // If target is recognized as an op, it is in the map
        OpInfo op_info = opcodes[op.getValue()];
        Instruction ins(op_info.opcode);
        
        if (op_info.arg_count > 2) {
            throw new std::runtime_error("Too many arguments for opcode");
        }

        if (op_info.arg_count == 1) {
            ins.setFirst(processArg());
        }

        if (op_info.arg_count == 2) {
            ins.setFirst(processArg());
            ins.setSecond(processArg());
        }

        if (ins.isPure()){
            fd << ins;
        }else{
           to_patch.push_back({fd.tellg(), ins});
           writeStub(ins.getSize());
        }

    }
    
    // Helper function to write empty instructions that will be patched later (updating fd pointer in memory)
    void Builder::writeStub(int len)
    {
        char *buff = new char[len];
        std::memset(buff, 0, len);

        fd.write(buff, len);

        delete[] buff;
    }

    void Builder::compile(std::string output_path)
    {
        // TODO : Implement parsing and generation (custom stream)
        
        Asm::Token current = lexer.getNext();
        fd.open(output_path, std::ios::out | std::ios::binary);

        if(!fd.is_open())
            throw std::runtime_error("Could not open target file");

        // Writing file header
        // Magic
        fd << 'M';
        fd << 'X';
        
        // Entry point
        writeStub(4);

        // File size
        writeStub(4);

        while(current.getType() != Asm::TokenType::Eof){
            switch(current.getType()){
                case Asm::TokenType::FnDecl:
                    parseFnDecl();
                    break;
                case Asm::TokenType::BytesDecl:
                    parseBytesDecl();
                    break;
                case Asm::TokenType::StringDecl:
                    parseStringDecl();
                    break;
                case Asm::TokenType::SlotDecl:
                    parseSlotDecl();
                    break;
                case Asm::TokenType::Op:
                    parseOpStmt(current);
                    break;
                case Asm::TokenType::NumDecl:
                    parseNumDecl();
                    break;
                default:
                    // do nothing
                    break;
            }

            current = lexer.getNext();
        }
        
        // Now fixing references
        std::vector<std::tuple<int, Instruction>>::iterator it;
        long eof_offset = fd.tellp();

        for (it = to_patch.begin(); it != to_patch.end(); ++it) {
            long offset = std::get<0>(*it);
            Instruction ins = std::get<1>(*it);

            Argument first = ins.getFirst();
            Argument second = ins.getSecond();

            if (first.type == ArgType::Ref) {
                if (symbols.find(first.value) == symbols.end())
                    throw std::runtime_error("Could not resolve symbol -> "+first.value);
                
                first.type = ArgType::Value;
                first.imm_value = symbols[first.value];
            }

            if (second.type == ArgType::Ref) {
                if (symbols.find(second.value) == symbols.end())
                    throw std::runtime_error("Could not resolve symbol -> "+second.value);
                
                second.type = ArgType::Value;
                second.imm_value = symbols[second.value];
            }
            
            ins.setFirst(first);
            ins.setSecond(second);

            fd.seekp(offset);
            fd << ins;
        }

        fd.seekp(2);

        if (symbols.find("main") == symbols.end()) {
            throw std::runtime_error("could not find main symbol");
        }

        int entrypoint = symbols["main"];
        int filesize = eof_offset;
        
        fd.write(reinterpret_cast<char *>(&entrypoint), 4);
        fd.write(reinterpret_cast<char *>(&filesize), 4);

        // Dunno if it is useful
        fd.seekp(eof_offset);

        fd.close();
    }

}
