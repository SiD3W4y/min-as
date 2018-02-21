#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>

#include "asm/token.hpp"

namespace Asm {

    extern std::unordered_map<std::string, TokenType> keywords;
    
    class CodeException : public std::runtime_error {
        public:
            CodeException(std::string message, std::string line, Token t);
            const char *what() const noexcept override;

        private:
            std::string msg;
            std::string line;
            Token token;
    };

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

            std::string getLine(int lineno); // Indexed from 1
        private:
            // Row and col for the lexer state
            int st_row = 1;
            int st_col = 0;
            
            std::vector<std::string> lines; // Used for error messages
            std::vector<Token> tokens;
            std::vector<Token>::iterator it;

    };
}

#endif
