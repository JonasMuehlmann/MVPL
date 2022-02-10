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
        mvpl [-S STAGE] [-t OUT_FILE] [-a OUT_FILE] [-s OUT_FILE] [-g OUT_FILE] [-p OUT_FILE] FILE
        mvpl -r FILE

    Arguments:
        FILE        source file to process
        STAGE:      token_stream
                    ast
                    symbol_table
                    semantic_validator
                    code_generation

    Options:
        -h --help                                show this help message and exit
        -r --run                                 run an already compiled program
        -S STAGE --stage=STAGE                   stop after completinng the stage
        -t OUT_FILE --token-steam=OUT_FILE       write token stream to file [default: stdout]
        -a OUT_FILE --ast=OUT_FILE               write abstract syntax tree to file [default: stdout]
        -s OUT_FILE --symbol-table=OUT_FILE      write program's symbol tabke to file [default: stdout]
        -g OUT_FILE --generated-code=OUT_FILE    write generated code to file [default: stdout]
        -p OUT_FILE --program-output=OUT_FILE    write run program's output to file [default: stdout]

    )";

int main(int argc, char* argv[])
{
    json output_artifacts{};

    std::map<std::string, docopt::value> args =
        docopt::docopt(options, {argv + 1, argv + argc}, true);

    std::ifstream source_stream(args["FILE"].asString());
    std::string   source_code((std::istreambuf_iterator<char>(source_stream)),
                            std::istreambuf_iterator<char>());

    lexer              lexer(source_code);
    std::vector<token> token_stream = lexer.lex();

    auto token_stream_output_artifact = token_stream_to_json(token_stream);

    std::unique_ptr<ast_node_t> ast = parse(token_stream);

    auto ast_output_artifact = ast_to_json(*ast);
    // if (ast != nullptr)
    // {
    //     auto ast_output_artifact = ast_to_json(*ast);
    // }

    return 0;
}
