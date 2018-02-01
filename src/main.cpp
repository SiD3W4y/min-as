#include <iostream>
#include <exception>
#include <string>

#include "asm/lexer.hpp"

int main(int argc, char **argv)
{
    try{
        Asm::Lexer l;
        l.tokenizeFile("./samples/hello_world.min");
    }catch(std::exception &e){
        std::cerr << "Err : " << e.what() << std::endl;
    }

    return 0;
}
