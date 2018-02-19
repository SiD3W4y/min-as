#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <unordered_map>

namespace Asm {
    enum TokenType {
        Symbol,
        Op,
        Reg,
        Value,
        Ref,
        String,
        Lbrack, // [
        Rbrack, // ]
        Sep, // ,
        StringDecl,
        BytesDecl,
        NumDecl,
        SlotDecl,
        FnDecl,
        Eof
    };
    

    extern std::unordered_map<std::string, TokenType> keywords;

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
