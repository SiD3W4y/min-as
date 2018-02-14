#include <iostream>
#include <exception>
#include <string>

#include "asm/lexer.hpp"
#include "asm/token.hpp"
#include "codegen/builder.hpp"

int main(int argc, char **argv)
{
    try{
        Asm::Lexer l;
        l.tokenizeFile("./samples/hello_world.min");
        Codegen::Builder builder(l);
        builder.compile("a.mx");
    }catch(std::exception &e){
        std::cerr << "Err : " << e.what() << std::endl;
    }

    return 0;
}
