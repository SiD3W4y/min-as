#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>

#include "asm/token.hpp"

namespace Asm {
    class Lexer {
        public:

        private:
            std::vector<std::string> lines; // Used for error messages
            std::vector<Token> tokens;

    };
}

#endif
