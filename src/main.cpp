#include <iostream>
#include <string.h>

#include "asm/token.hpp"

int main(int argc, char **argv)
{
    Asm::Token t(Asm::TokenType::Symbol, std::string("Hello World !"));
    std::cout << "min-as" << std::endl;
    return 0;
}
