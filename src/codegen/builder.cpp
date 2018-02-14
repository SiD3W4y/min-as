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
        if(tk.getType() != target){
            // TODO : Add line printing
            std::stringstream error_message;
            error_message << "Wrong token type at position (";
            error_message << tk.getRow() << ", " << tk.getCol();
            error_message << ")" << std::endl;

            throw std::runtime_error(error_message.str()); 
        }
    }

    void Builder::compile(std::string output_path)
    {
        // TODO : Implement parsing and generation (custom stream)
        
        Asm::Token current = lexer.getNext();
        std::fstream fd;
        fd.open(output_path, std::ios::out | std::ios::binary);

        if(!fd.is_open())
            throw std::runtime_error("Could not open target file");

        // TODO : Write header to start of file

        while(current.getType() != Asm::TokenType::Eof){
            // TODO : Add token types for keywords (fn, string, bytes, slot, num, ...)
            if(current.getValue() == "string"){
                Asm::Token name = lexer.getNext();
                Asm::Token string_data = lexer.getNext();

                check_type(name, Asm::TokenType::Symbol);
                check_type(string_data, Asm::TokenType::String);

                symbols[name.getValue()] = fd.tellg();
                
                fd << string_data.getValue();
            }

            current = lexer.getNext();
        }

        fd.close();
    }

}
