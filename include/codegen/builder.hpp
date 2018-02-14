#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>

#include "codegen/instruction.hpp"
#include "asm/lexer.hpp"
#include "asm/token.hpp"

namespace Codegen {

    // Class Parsing+Generating code to a stream
    class Builder {
        public:
            Builder(Asm::Lexer lex);

            void compile(std::string output_path);

        private:
            std::vector<std::tuple<int, Instruction>> to_patch; // Tuples of instruction + offset to patch when resolving refs
            std::unordered_map<std::string, int> symbols;
            Asm::Lexer lexer;
    };

}


#endif
