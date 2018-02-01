#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>

#include "asm/token.hpp"

namespace Asm {
    class Lexer {
        public:
            Lexer();
            Lexer(std::string file);

            void tokenizeFile(std::string path);
            void tokenizeString(std::string data);
            
            void appendToken(Token t);
            void appendToken(std::string tokstr); // Determines the token type and adds it to the tokenlist

            Token getNext(); // Returns next token from stream or TokenType::Eof if we reached the end
            void rewind(); // Rewind the token pointer to the start of the stream
        private:
            std::vector<std::string> lines; // Used for error messages
            std::vector<Token> tokens;
            std::vector<Token>::iterator it;

    };
}

#endif
