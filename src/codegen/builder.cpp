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

    static void check_type(Asm::Token tk, Asm::TokenType target)
    {
        // check_type needs to support custom error messages for better error reporting
        if(tk.getType() != target){
            // TODO : Add line printing
            std::stringstream error_message;
            error_message << "Wrong token type at position (";
            error_message << tk.getRow() << ", " << tk.getCol();
            error_message << ")" << std::endl;

            throw std::runtime_error(error_message.str()); 
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
        std::vector<std::string> bytes;
        
        Asm::Token bt_name = lexer.getNext();
        Asm::Token brack = lexer.getNext();

        check_type(brack, Asm::TokenType::Lbrack);

        Asm::Token current = lexer.getNext();

        while(current.getType() != Asm::TokenType::Rbrack){
            check_type(current, Asm::TokenType::Value);

            bytes.push_back(current.getValue());
            current = lexer.getNext();

            if(current.getType() == Asm::TokenType::Sep)
                current = lexer.getNext(); // Skipping commas
        } 
    }

    void Builder::parseNumDecl()
    {
        Asm::Token n_name = lexer.getNext();
        Asm::Token n_value = lexer.getNext();

        check_type(n_name, Asm::TokenType::Symbol);
        check_type(n_value, Asm::TokenType::Value);

        symbols[n_name.getValue()] = fd.tellg();
    }

    void Builder::parseSlotDecl()
    {
        Asm::Token sl_name = lexer.getNext();
        Asm::Token sl_value = lexer.getNext();
        
        check_type(sl_name, Asm::TokenType::Symbol);
        check_type(sl_value, Asm::TokenType::Value);

        symbols[sl_name.getValue()] = fd.tellg();
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
        
        if(arg_tk.getType() == Asm::TokenType::Value){
            arg.type = ArgType::Value;
            arg.imm_value = Utils::str_to_int(arg_tk.getValue()); 
        }else if(arg_tk.getType() == Asm::TokenType::Ref){
            arg.type = ArgType::Ref;
            arg.value = arg_tk.getValue();
        }else if(arg_tk.getType() == Asm::TokenType::Reg){
            arg.type = ArgType::Reg;

            if(registers.find(arg_tk.getValue()) == registers.end())
                throw std::runtime_error("Could not find register : "+arg_tk.getValue());

            arg.imm_value = registers[arg_tk.getValue()];
        }else{
            throw new std::runtime_error("Not a valid argument -> "+arg_tk.getValue());
        }

        return arg;
    }

    void Builder::parseOpStmt(Asm::Token op)
    {
        // If target is recognized as an op, it is in the map
        OpInfo op_info = opcodes[op.getValue()];
        Instruction ins(op_info.opcode);
        
        if(op_info.arg_count > 2){
            throw new std::runtime_error("Too many arguments for opcode");
        }

        if(op_info.arg_count == 1){
            ins.setFirst(processArg());
        }

        if(op_info.arg_count == 2){
            ins.setFirst(processArg());
            ins.setSecond(processArg());
        }

    }

    void Builder::compile(std::string output_path)
    {
        // TODO : Implement parsing and generation (custom stream)
        
        Asm::Token current = lexer.getNext();
        fd.open(output_path, std::ios::out | std::ios::binary);

        if(!fd.is_open())
            throw std::runtime_error("Could not open target file");

        // TODO : Write header to start of file

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
                default:
                    // do nothing
                    break;
            }

            current = lexer.getNext();
        }

        fd.close();
    }

}
