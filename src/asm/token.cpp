#include <string>

#include "asm/token.hpp"

namespace Asm {
    Token::Token(TokenType t, std::string val): type(t), value(val) 
    {
    
    }

    void Token::setType(TokenType t)
    {
        type = t;
    }

    TokenType Token::getType()
    {
        return type;
    }

    void Token::setValue(std::string data)
    {
        value = data;
    }

    std::string Token::getValue()
    {
        return value;
    }

    void Token::setRow(int r)
    {
        row = r;
    }

    void Token::setCol(int c)
    {
        col = c;
    }

    int Token::getRow()
    {
        return row;
    }

    int Token::getCol()
    {
        return col;
    }
}
