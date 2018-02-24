#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include "args.hxx"

#include "asm/lexer.hpp"
#include "asm/token.hpp"
#include "codegen/builder.hpp"

static void dumpFile(std::string path, std::string data)
{
    std::fstream fd;
    fd.open(path, std::ios::out | std::ios::binary);

    if(!fd)
        throw std::runtime_error("Could not open input file : "+path);

    fd << data;
    fd.close();
}

int main(int argc, char **argv)
{
    args::ArgumentParser parser("min-as : The min assembler","");
    args::HelpFlag help(parser, "help", "Displays this menu", {"h", "help"});
    args::Flag symbol(parser, "symbols", "Dumps a radare2 file with min object symbols", {"s", "symbols"});
    args::ValueFlag<std::string> input_str(parser, "input file", "Specifies the input .min asm file", {"i", "input"});
    args::ValueFlag<std::string> output_str(parser, "output file", "Specifies the output .mx file (default a.mx)", {"o", "output"});

    try
    {
        parser.ParseCLI(argc, argv);
        
        if (!input_str)
            throw std::runtime_error("No input file specified");

        // Processing the file
        Asm::Lexer l;
        l.tokenizeFile(args::get(input_str));
        Codegen::Builder builder(l);

        if (output_str) {
            dumpFile(args::get(output_str), builder.compile());
        } else {
            dumpFile("a.mx", builder.compile());
        }

        if (symbol)
            dumpFile("symbols.r2", builder.getSymbolMap());
    }
    catch (args::Help e)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    catch(std::exception &e)
    {
        std::cerr << "Err : " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
