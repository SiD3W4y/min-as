#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>
#include <sstream>

#include "codegen/instruction.hpp"
#include "asm/lexer.hpp"
#include "asm/token.hpp"

namespace Codegen {

    // Class Parsing+Generating code to a stream
    class Builder {
        public:
            Builder(Asm::Lexer lex);
            
            std::string compile();
            std::string getSymbolMap(); // returns the symbols with corresponding offsets as a radare2 project file
        private:
            // parse functions will actually parse+compile the code
            void parseStringDecl();
            void parseNumDecl();
            void parseBytesDecl();
            void parseSlotDecl();
            void parseFnDecl();
            void parseOpStmt(Asm::Token op);
            void writeStub(int len);
            void check_type(Asm::Token op, Asm::TokenType target);
            Argument processArg();

            std::vector<std::tuple<int, Instruction>> to_patch; // Tuples of instruction + offset to patch when resolving refs
            std::unordered_map<std::string, int> symbols;
            Asm::Lexer lexer;

            std::stringstream fd;
            std::stringstream symbol_map;
    };

}


#endif
