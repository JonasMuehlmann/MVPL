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
#include <set>
#include <stdexcept>
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
        mvpl [-S STAGE] [-t OUT_FILE] [-a OUT_FILE] [-s OUT_FILE] [-g OUT_FILE] [-p OUT_FILE] [-o ARTIFACT]...  -i FILE
        mvpl -r -i FILE

    Arguments:
        STAGE:    token_stream
                  ast
                  symbol_table
                  semantic_validator
                  code_generation
        ARTIFACT: token_stream
                  ast
                  symbol_table
                  generated_code
                  program_output

    Options:
        -h --help                                show this help message and exit
        -r --run                                 run an already compiled program
        -i FILE --input=FILE                     input file to process
        -S STAGE --stage=STAGE                   stop after completinng the stage
        -o ARTIFACT --output_artifact=ARTIFACT   include the following artifact in output
        -t OUT_FILE --token-steam=OUT_FILE       redirect token stream to file
        -a OUT_FILE --ast=OUT_FILE               redirect abstract syntax tree to file
        -s OUT_FILE --symbol-table=OUT_FILE      redirect program's symbol tabke to file
        -g OUT_FILE --generated-code=OUT_FILE    redirect generated code to file
        -p OUT_FILE --program-output=OUT_FILE    redirect run program's output to file

    )";

std::set<std::string> STAGES{
    "token_stream",
    "ast",
    "symbol_table",
    "semantic_validator",
    "code_generation",
};

std::set<std::string> ARTIFACTS{
    "token_stream", "ast", "symbol_table", "generated_code", "program_output"};

int main(int argc, char* argv[])
{
    json artifact_output{};

    std::map<std::string, docopt::value> args =
        docopt::docopt(options, {argv + 1, argv + argc}, true);

    // for (auto const& arg : args)
    // {
    //     std::cout << arg.first << arg.second << std::endl;
    // }

    // Validate --output_artifact arguments
    if (!args["--output_artifact"].isStringList())
    {
        throw std::invalid_argument("output artifacts must be specified as strings");
    }
    auto output_artifacts = args["--output_artifact"].asStringList();
    auto output_artifacts_set =
        std::set(begin(output_artifacts), end(output_artifacts));

    if (!std::ranges::all_of(output_artifacts_set, [](auto& artifact) {
            return ARTIFACTS.contains(artifact);
        }))
    {
        throw std::invalid_argument("Invalid artifact passed");
    }

    // Validate --stage argument
    if (args["--stage"].isString() && !STAGES.contains(args["--stage"].asString()))
    {
        throw std::invalid_argument("Invalid stage passed");
    }

    std::ifstream source_stream(args["--input"].asString());
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
