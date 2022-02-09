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

#include "common/util.hpp"
#include "docopt.h"
#include "frontend/lexer/lexer.hpp"
#include "frontend/lexer/token.hpp"
#include "frontend/lexer/token_type.hpp"
#include "frontend/parser/ast_node.hpp"
#include "frontend/parser/ast_node_type.hpp"
#include "frontend/parser/parser.hpp"

static const std::string options =
    R"(MVPL - The minimum viable programming language.

    Usage:
        mvpl -h
        mvpl [-Stasgpv] FILE
        mvpl -r FILE

    Arguments:
        FILE    source file to process
        STAGE:  token_stream
                ast
                symbol_table
                semantic_validator
                code_generation

    Options:
        -h --help                   show this help message and exit
        -v --verbose                print extra output on stderr
        -r --run                    run an already compiled program
        -S STAGE --stage=STAGE      stop after completinng the stage
        -t --token-steam=FILE       write token stream to file [default: stdout]
        -a --ast=FILE               write abstract syntax tree to file [default: stdout]
        -s --symbol-table=FILE      write program's symbol tabke to file [default: stdout]
        -g --generated-code=FILE    write generated code to file [default: stdout]
        -p --program-output=FILE    write run program's output to file [default: stdout]

    )";

int main(int argc, char* argv[])
{
    std::map<std::string, docopt::value> args =
        docopt::docopt(options, {argv + 1, argv + argc}, true);

    std::ifstream source_stream(args["FILE"].asString());
    std::string   source_code((std::istreambuf_iterator<char>(source_stream)),
                            std::istreambuf_iterator<char>());

    lexer              lexer(source_code);
    std::vector<token> token_stream = lexer.lex();

    print_token_stream(token_stream);

    std::unique_ptr<ast_node_t> ast = parse(token_stream);

    if (ast != nullptr)
    {
        print_ast(*ast);
    }

    return 0;
}
