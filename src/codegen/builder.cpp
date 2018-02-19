#include <vector>
#include <tuple>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "codegen/builder.hpp"
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

    }

    void Builder::parseSlotDecl()
    {

    }

    void Builder::parseFnDecl()
    {
        // TODO : Handle error handling for multiple definition
        Asm::Token fn_name = lexer.getNext();
        symbols[fn_name.getValue()] = fd.tellg();
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
                default:
                    // do nothing
                    break;
            }

            current = lexer.getNext();
        }

        fd.close();
    }

}
