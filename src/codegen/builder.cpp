#include <vector>
#include <tuple>
#include <string>

#include "codegen/builder.hpp"
#include "asm/token.hpp"

namespace Codegen {

    Builder::Builder(std::vector<Asm::Token> toks) : tokens(toks)
    {
    }

    void Builder::compile(std::string output_path)
    {
        // TODO : Implement parsing and generation (custom stream)
    }

}
