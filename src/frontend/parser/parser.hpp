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
#ifndef SRC_FRONTEND_PARSER_PARSER_HPP_
#define SRC_FRONTEND_PARSER_PARSER_HPP_

#include <memory>
#include <optional>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>

#include "ast_node.hpp"
#include "frontend/lexer/token.hpp"
#include "frontend/parser/ast_node.hpp"
#include "token_type.hpp"


template <typename TFunction>
concept Parser = requires(TFunction& function)
{
    {
        std::forward<TFunction>(function)()
        } -> std::convertible_to<bool>;
};

using parse_result_t =
    std::optional<std::tuple<std::span<token>, std::unique_ptr<ast_node_t>>>;

struct parse_program;
struct parse_binary_op;
struct parse_unary_op;
struct parse_func_def;
struct parse_procedure_def;
struct parse_signature;
struct parse_return_stmt;
struct parse_parameter_def;
struct parse_var_decl;
struct parse_var_init;
struct parse_var_assignment;
struct parse_call;
struct parse_parameter_pass;
struct parse_block;
struct parse_control_block;
struct parse_control_head;
struct parse_expression;

template <typename... Parsers>
struct parse_any;

template <typename... Parsers>
struct parse_all;

template <typename SeparatorParser, typename... ItemParsers>
struct parse_separated;

template <typename SurrounderParser, typename... InnerParsers>
struct parse_surrounded;

template <token_type wanted>
struct parse_token;


template <typename... Parsers>
struct parse_any
{
    static parse_result_t parse(const std::span<token> ts)
    {
        auto           parsers = std::array{Parsers::parse...};
        parse_result_t result;

        for (auto parser : parsers)
        {
            result = parser(ts);

            if (result)
            {
                return result;
            }
        }
        return {};
        // return (Parsers::parse(ts) || ...);
    }
};

template <typename... Parsers>
struct parse_all
{
    static parse_result_t parse(const std::span<token> ts)
    {
        auto           parsers = std::array{Parsers::parse...};
        parse_result_t result;

        for (auto parser : parsers)
        {
            result = parser(ts);

            if (!result)
            {
                return result;
            }
        }
        return {};
        // return (Parsers::parse(ts) && ...);
    }
};

template <typename SeparatorParser, typename... ItemParsers>
struct parse_separated
{
    static parse_result_t parse(const std::span<token> ts)
    {
        auto item_parsers = std::array{ItemParsers::parse...};

        for (auto parser : item_parsers)
        {
            if (!parser(ts))
            {
                return {};
            }
            if (&parser != &item_parsers.back() && SeparatorParser::parse(ts))
            {
                return {};
            }
        }
        return {};
    }
};

template <typename SurrounderParser, typename... InnerParsers>
struct parse_surrounded
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<SurrounderParser,
                         parse_all<InnerParsers...>,
                         SurrounderParser>::parse(ts);
    }
};

template <token_type wanted>
struct parse_token
{
    static parse_result_t parse(const std::span<token> ts)
    {
        if (ts[0].type == wanted)
        {
            // return {ts.subspan(1), ast_node_t{empty_node{}}};
            return {{ts.subspan(1), std::make_unique<ast_node_t>(empty_node{})}};
        }
        return {};
    }
};

struct parse_program
{
    static parse_result_t parse(const std::span<token> ts)
    {
        while (!ts.empty())
        {
            if (!parse_any<parse_procedure_def,
                           parse_func_def,
                           parse_var_decl,
                           parse_var_init>::parse(ts))
            {
                return {};
            }
        }
        return {};
    };
};

struct parse_binary_op
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_expression,
                         parse_any<parse_token<token_type::PLUS>,
                                   parse_token<token_type::MINUS>,
                                   parse_token<token_type::MULTIPLICATION>,
                                   parse_token<token_type::DIVISION>,
                                   parse_token<token_type::MODULO>,
                                   parse_token<token_type::LESS>,
                                   parse_token<token_type::LESSEQ>,
                                   parse_token<token_type::GREATER>,
                                   parse_token<token_type::GREATEREQ>,
                                   parse_token<token_type::EQUAL>,
                                   parse_token<token_type::NEQUAL>,
                                   parse_token<token_type::LOGICAL_AND>,
                                   parse_token<token_type::LOGICAL_OR>,
                                   parse_token<token_type::BINARY_AND>,
                                   parse_token<token_type::BINARY_OR>,
                                   parse_token<token_type::XOR>,
                                   parse_token<token_type::LSHIFT>,
                                   parse_token<token_type::RSHIFT>>,
                         parse_expression>::parse(ts);
    }
};

struct parse_unary_op
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_expression,
                         parse_any<parse_token<token_type::NOT>,
                                   parse_token<token_type::INCREMENT>,
                                   parse_token<token_type::DECREMENT>>>::parse(ts);
    }
};

struct parse_func_def
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_token<token_type::FUNCTION>,
                         parse_signature,
                         parse_block>::parse(ts);
    }
};

struct parse_procedure_def
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_token<token_type::PROCEDURE>,
                         parse_signature,
                         parse_block>::parse(ts);
    }
};

struct parse_signature
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_token<token_type::IDENTIFIER>,
                         parse_parameter_def>::parse(ts);
    }
};

struct parse_return_stmt
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_token<token_type::RETURN>, parse_expression>::parse(ts);
    }
};

struct parse_parameter_def
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_surrounded<
            parse_token<token_type::LPAREN>,
            parse_token<token_type::RPAREN>,
            parse_separated<parse_token<token_type::COMMA>,
                            parse_token<token_type::IDENTIFIER>>>::parse(ts);
    }
};

struct parse_var_decl
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_token<token_type::LET>,
                         parse_token<token_type::IDENTIFIER>,
                         parse_token<token_type::SEMICOLON>>::parse(ts);
    }
};

struct parse_var_init
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_token<token_type::LET>,
                         parse_token<token_type::IDENTIFIER>,
                         parse_token<token_type::EQUAL>,
                         parse_expression,
                         parse_token<token_type::SEMICOLON>>::parse(ts);
    }
};

struct parse_var_assignment
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_token<token_type::IDENTIFIER>,
                         parse_token<token_type::EQUAL>,
                         parse_expression,
                         parse_token<token_type::SEMICOLON>>::parse(ts);
    }
};

struct parse_call
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_token<token_type::IDENTIFIER>,
                         parse_parameter_pass>::parse(ts);
    }
};

struct parse_parameter_pass
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_surrounded<
            parse_token<token_type::LPAREN>,
            parse_token<token_type::RPAREN>,
            parse_separated<parse_token<token_type::COMMA>,
                            parse_token<token_type::IDENTIFIER>>>::parse(ts);
    }
};

struct parse_block
{
    static parse_result_t parse(const std::span<token> ts)
    {
        // TODO: Implement parse_many
        return parse_any<parse_any<parse_var_decl,
                                   parse_var_assignment,
                                   parse_var_init,
                                   parse_expression,
                                   parse_control_block>>::parse(ts);
    }
};

struct parse_control_block
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_all<parse_control_block, parse_block>::parse(ts);
    }
};

struct parse_control_head
{
    static parse_result_t parse(const std::span<token> ts)
    {
        {
            return parse_surrounded<parse_token<token_type::LPAREN>,
                                    parse_token<token_type::RPAREN>,
                                    parse_expression>::parse(ts);
        }
    }
};

struct parse_expression
{
    static parse_result_t parse(const std::span<token> ts)
    {
        return parse_any<parse_binary_op,
                         parse_unary_op,
                         parse_call,
                         parse_token<token_type::LITERAL>,
                         parse_token<token_type::IDENTIFIER>>::parse(ts);
    }
};

std::unique_ptr<ast_node_t> parse(const std::span<token> ts)
{
    return {std::get<1>(parse_program::parse(ts).value())};
}
#endif    // SRC_FRONTEND_PARSER_PARSER_HPP_
