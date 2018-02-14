#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <vector>
#include <tuple>
#include <string>

#include "codegen/instruction.hpp"
#include "asm/token.hpp"

namespace Codegen {

    // Class Parsing+Generating code to a stream
    class Builder {
        public:
            Builder(std::vector<Asm::Token> toks);

            void compile(std::string output_path);

        private:
            std::vector<std::tuple<int, Instruction>> to_patch; // Tuples of instruction + offset to patch when resolving refs
            std::vector<Asm::Token> tokens;
    };

}


#endif
