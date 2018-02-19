#include <string>
#include <unordered_map>

#include "asm/token.hpp"

namespace Asm {

    std::unordered_map<std::string, TokenType> keywords = {
        {"string", TokenType::StringDecl},
        {"num", TokenType::NumDecl},
        {"bytes", TokenType::BytesDecl},
        {"slot", TokenType::SlotDecl},
        {"fn", TokenType::FnDecl},
        {"add", TokenType::Op},
        {"add", TokenType::Op},
        {"sub", TokenType::Op},
        {"mul", TokenType::Op},
        {"mov", TokenType::Op},
        {"ldr", TokenType::Op},
        {"ldrb", TokenType::Op},
        {"str", TokenType::Op},
        {"strb", TokenType::Op},
        {"push", TokenType::Op},
        {"pop", TokenType::Op},
        {"cmp", TokenType::Op},
        {"jmp", TokenType::Op},
        {"jne", TokenType::Op},
        {"je", TokenType::Op},
        {"jle", TokenType::Op},
        {"jbe", TokenType::Op},
        {"sys", TokenType::Op},
        {"xor", TokenType::Op},
        {"and", TokenType::Op},
        {"or", TokenType::Op},
        {"shr", TokenType::Op},
        {"shl", TokenType::Op},
        {"call", TokenType::Op},
        {"ret", TokenType::Op}
    };

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
