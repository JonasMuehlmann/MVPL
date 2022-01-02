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
#include "parser.hpp"

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "ast_node.hpp"
#include "ast_node_type.hpp"

std::unique_ptr<ast_node_t> parser::parse()
{
    return std::move(ast);
}

// Based on parser combinators
parser::parser(std::span<token> token_stream_) : token_stream_{token_stream_} {}

bool parser::parse_program()
{
    while (!token_stream_.empty())
    {
        if (!parse_any(&parser::parse_procedure_def,
                       &parser::parse_func_def,
                       &parser::parse_var_decl,
                       &parser::parse_var_init))
        {
            return false;
        }
    }
    return true;
}
bool parser::parse_binary_op()
{
    return true;
}
bool parser::parse_unary_op()
{
    return true;
}
bool parser::parse_func_def()
{
    return true;
}
bool parser::parse_procedure_def()
{
    return true;
}
bool parser::parse_signature()
{
    return true;
}
bool parser::parse_return_stmt()
{
    return true;
}
bool parser::parse_parameter_def()
{
    return true;
}
bool parser::parse_var_decl()
{
    return true;
}
bool parser::parse_var_init()
{
    return true;
}
bool parser::parse_var_assignment()
{
    return true;
}
bool parser::parse_call()
{
    return true;
}
bool parser::parse_parameter_pass()
{
    return true;
}
bool parser::parse_block()
{
    return true;
}
bool parser::parse_control_block()
{
    return true;
}
bool parser::parse_control_head()
{
    return true;
}
bool parser::parse_any(nullary_predicate auto&&... parser)
{
    return (std::invoke(parser, this) || ...);
}
bool parser::parse_all(nullary_predicate auto&&... parser)
{
    return (std::invoke(parser, this) && ...);
}
bool parser::parse_separated(nullary_predicate auto&& separator_parser,
                             nullary_predicate auto&&... item_parser)
{
    auto item_parsers = {item_parser...};

    for (auto parser : item_parsers)
    {
        if (!std::invoke(parser, this))
        {
            return false;
        }
        if (&parser != &item_parsers.back() && separator_parser())
        {
            return false;
        }
    }
    return true;
}
bool parser::parse_surrounded(nullary_predicate auto&& surrounder_parser,
                              nullary_predicate auto&&... inner_parser)
{
    return surrounder_parser() & parse_all(inner_parser...) & surrounder_parser();
}
