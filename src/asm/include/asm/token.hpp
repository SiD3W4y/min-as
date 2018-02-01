#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace Asm {
    enum TokenType {
        Symbol,
        Reg,
        Value,
        Ref,
        String
    };

    class Token {
        public:
            Token(std::string value, TokenType t);
            
            void setType(TokenType t);
            TokenType getType();

            void setValue(std::string value);
            std::string getValue();

        private:
            TokenType type;
            std::string value;

    };
}


#endif
