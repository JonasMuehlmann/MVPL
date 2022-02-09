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
#include "util.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "enum_range.hpp"
#include "frontend/lexer/token_type.hpp"
#include "frontend/parser/ast_node.hpp"
#include "frontend/parser/ast_node_type.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;


void print_token_stream(const std::vector<token>& token_stream)
{
    // Double brackets to define as key-value pair instead of list
    json j = {{"token_stream", token_stream}};
    std::cout << j.dump(4) << std::endl;
}

void print_ast(const ast_node_t& ast)
{
    // Double brackets to define as key-value pair instead of list
    json j = {{"ast", ast}};

    std::cout << j.dump(4) << std::endl;
}
