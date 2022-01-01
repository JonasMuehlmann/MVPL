// Copyright © 2021 Jonas Muehlmann
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <streambuf>
#include <string>

#include "ast_node_type.hpp"
#include "common/util.hpp"
#include "frontend/lexer/lexer.hpp"
#include "frontend/lexer/token.hpp"
#include "frontend/lexer/token_type.hpp"
#include "frontend/parser/ast_node.hpp"
#include "frontend/parser/parser.hpp"
#include "state.hpp"

// TODO: Add CLI flags for building and printing up to: tokenstream, AST, bytecode.
int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        return 1;
    }
    std::ifstream source_stream(argv[1]);
    std::string   source_code((std::istreambuf_iterator<char>(source_stream)),
                            std::istreambuf_iterator<char>());

    lexer              lexer(source_code);
    std::vector<token> token_stream = lexer.lex();
    print_token_stream(token_stream);

    parser parser(token_stream);
    // std::unique_ptr<ast_node> ast = parser.parse();
    std::vector<std::unique_ptr<ast_node_t>> v1{};

    std::vector<std::unique_ptr<ast_node_t>> v2{};

    std::vector<std::unique_ptr<ast_node_t>> foo{};
    foo.push_back(std::make_unique<ast_node_t>(program_node(v1, source_location())));
    foo.push_back(std::make_unique<ast_node_t>(program_node(v2, source_location())));

    auto ast = std::make_unique<program_node>(program_node(foo, source_location()));


    print_ast(*ast);

    return 0;
}
