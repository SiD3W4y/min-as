#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace Asm {
    enum TokenType {
        Symbol,
        Op,
        Reg,
        Value,
        Ref,
        String,
        Eof
    };

    class Token {
        public:
            Token(TokenType t, std::string value);
            
            void setType(TokenType t);
            TokenType getType();

            void setValue(std::string value);
            std::string getValue();

            void setRow(int r);
            void setCol(int c);
            int getRow();
            int getCol();

        private:
            TokenType type;
            std::string value;
            int row = 0;
            int col = 0;

    };
}


#endif
