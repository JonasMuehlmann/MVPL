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
#include "token_type.hpp"

// Based on parser combinators
parser::parser(std::span<token> token_stream_) : token_stream_{token_stream_} {}

nullary_predicate auto parse_token(const std::span<token> ts, const token_type wanted)
{
    return [=]() -> bool { return ts[0].type == wanted; };
}

nullary_predicate auto parse_any(nullary_predicate auto&&... parser)
{
    return [=]() -> bool { return (parser() || ...); };
}

nullary_predicate auto parse_all(nullary_predicate auto&&... parser)
{
    return [=]() -> bool { return (parser() && ...); };
}

nullary_predicate auto parse_surrounded(nullary_predicate auto&& surrounder_parser,
                                        nullary_predicate auto&&... inner_parser)
{
    return parse_all(surrounder_parser, parse_all(inner_parser...), surrounder_parser);
}

nullary_predicate auto parse_separated(nullary_predicate auto&& separator_parser,
                                       nullary_predicate auto&&... item_parser)
{
    return [=]() -> bool {
        auto item_parsers = std::array{item_parser...};

        for (auto parser : item_parsers)
        {
            if (!parser())
            {
                return false;
            }
            if (&parser != &item_parsers.back() && separator_parser())
            {
                return false;
            }
        }
        return true;
    };
}

nullary_predicate auto parse_parameter_def(const std::span<token> ts)
{
    return parse_surrounded(parse_token(ts, token_type::LPAREN),
                            parse_token(ts, token_type::RPAREN),
                            parse_separated(parse_token(ts, token_type::COMMA),
                                            parse_token(ts, token_type::IDENTIFIER)));
}

nullary_predicate auto parse_signature(const std::span<token> ts)
{
    return parse_all(parse_token(ts, token_type::IDENTIFIER), parse_parameter_def(ts));
}

nullary_predicate auto parse_block(const std::span<token> ts)
{
    return [=]() { return true; };
}

nullary_predicate auto parse_procedure_def(const std::span<token> ts)
{
    return parse_all(
        parse_token(ts, token_type::PROCEDURE), parse_signature(ts), parse_block(ts));
}

nullary_predicate auto parse_func_def(const std::span<token> ts)
{
    return parse_all(
        parse_token(ts, token_type::FUNCTION), parse_signature(ts), parse_block(ts));
}

nullary_predicate auto parse_var_decl(const std::span<token> ts)
{
    return parse_all(parse_token(ts, token_type::LET),
                     parse_token(ts, token_type::IDENTIFIER),
                     parse_token(ts, token_type::SEMICOLON));
}

nullary_predicate auto parse_parameter_pass(const std::span<token> ts)
{
    return parse_surrounded(parse_token(ts, token_type::LPAREN),
                            parse_token(ts, token_type::RPAREN),
                            parse_separated(parse_token(ts, token_type::COMMA),
                                            parse_token(ts, token_type::IDENTIFIER)));
}

nullary_predicate auto parse_call(const std::span<token> ts)
{
    return parse_all(parse_token(ts, token_type::IDENTIFIER), parse_parameter_pass(ts));
}

nullary_predicate auto parse_expression(const std::span<token> ts)
{
    return parse_any(parse_binary_op(ts),
                     parse_unary_op(ts),
                     parse_call(ts),
                     parse_token(ts, token_type::LITERAL),
                     parse_token(ts, token_type::IDENTIFIER));
}

nullary_predicate auto parse_binary_op(const std::span<token> ts)
{
    return parse_all(parse_expression(ts),
                     parse_any(parse_token(ts, token_type::PLUS),
                               parse_token(ts, token_type::MINUS),
                               parse_token(ts, token_type::MULTIPLICATION),
                               parse_token(ts, token_type::DIVISION),
                               parse_token(ts, token_type::MODULO),
                               parse_token(ts, token_type::LESS),
                               parse_token(ts, token_type::LESSEQ),
                               parse_token(ts, token_type::GREATER),
                               parse_token(ts, token_type::GREATEREQ),
                               parse_token(ts, token_type::EQUAL),
                               parse_token(ts, token_type::NEQUAL),
                               parse_token(ts, token_type::LOGICAL_AND),
                               parse_token(ts, token_type::LOGICAL_OR),
                               parse_token(ts, token_type::BINARY_AND),
                               parse_token(ts, token_type::BINARY_OR),
                               parse_token(ts, token_type::XOR),
                               parse_token(ts, token_type::LSHIFT),
                               parse_token(ts, token_type::RSHIFT)),
                     parse_expression(ts));
}

nullary_predicate auto parse_unary_op(const std::span<token> ts)
{
    return parse_all(parse_expression(ts),
                     parse_any(parse_token(ts, token_type::NOT),
                               parse_token(ts, token_type::INCREMENT),
                               parse_token(ts, token_type::DECREMENT)));
}

nullary_predicate auto parse_var_assignment(const std::span<token> ts)
{
    return parse_all(parse_token(ts, token_type::IDENTIFIER),
                     parse_token(ts, token_type::EQUAL),
                     parse_expression(ts),
                     parse_token(ts, token_type::SEMICOLON));
}

nullary_predicate auto parse_var_init(const std::span<token> ts)
{
    return parse_all(parse_token(ts, token_type::LET),
                     parse_token(ts, token_type::IDENTIFIER),
                     parse_token(ts, token_type::EQUAL),
                     parse_expression(ts),
                     parse_token(ts, token_type::SEMICOLON));
}

nullary_predicate auto parse_program(const std::span<token> ts)
{
    return [=]() {
        while (!ts.empty())
        {
            if (!(parse_any(parse_procedure_def(ts),
                            parse_func_def(ts),
                            parse_var_decl(ts),
                            parse_var_init(ts))()))
            {
                return false;
            }
        }
        return true;
    };
}

nullary_predicate auto parse_return_stmt(const std::span<token> ts)
{
    return parse_all(parse_token(ts, token_type::RETURN), parse_expression(ts));
}


nullary_predicate auto parse_control_head(const std::span<token> ts)
{
    return parse_surrounded(parse_token(ts, token_type::LPAREN),
                            parse_token(ts, token_type::RPAREN),
                            parse_expression(ts));
}

nullary_predicate auto parse_control_block(const std::span<token> ts)
{
    return parse_all(parse_control_head(ts), parse_block(ts));
}

std::unique_ptr<ast_node_t> parser::parse()
{
    if (parse_program(token_stream_)())
    {
        return std::move(ast);
    }
    return std::unique_ptr<ast_node_t>{};
}
