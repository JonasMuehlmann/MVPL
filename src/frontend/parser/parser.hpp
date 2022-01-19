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

#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "ast_node.hpp"
#include "frontend/lexer/token.hpp"
#include "frontend/parser/ast_node.hpp"
#include "frontend/parser/ast_operations/retrieve_source_location.hpp"
#include "frontend/parser/parse_error.hpp"
#include "frontend/parser/parser_combinators.hpp"
#include "frontend/parser/util.hpp"
#include "nlohmann/json.hpp"
#include "token_type.hpp"


//****************************************************************************//
//                                 Private API                                //
//****************************************************************************//

using namespace std::literals::string_view_literals;
using json = nlohmann::ordered_json;

const auto LUT_TOKEN_TO_PRECEDENCE = []() {
    constexpr auto arr = []() {
        std::array<int, NUM_TOKENS> arr{};
        arr.fill(0);


        arr[static_cast<size_t>(token_type::LPAREN)]         = 100;
        arr[static_cast<size_t>(token_type::LESS)]           = 50;
        arr[static_cast<size_t>(token_type::LESSEQ)]         = 50;
        arr[static_cast<size_t>(token_type::GREATER)]        = 50;
        arr[static_cast<size_t>(token_type::GREATEREQ)]      = 50;
        arr[static_cast<size_t>(token_type::EQUAL)]          = 50;
        arr[static_cast<size_t>(token_type::NEQUAL)]         = 50;
        arr[static_cast<size_t>(token_type::NOT)]            = 90;
        arr[static_cast<size_t>(token_type::LOGICAL_AND)]    = 50;
        arr[static_cast<size_t>(token_type::LOGICAL_OR)]     = 50;
        arr[static_cast<size_t>(token_type::BINARY_AND)]     = 60;
        arr[static_cast<size_t>(token_type::BINARY_OR)]      = 60;
        arr[static_cast<size_t>(token_type::LSHIFT)]         = 60;
        arr[static_cast<size_t>(token_type::RSHIFT)]         = 60;
        arr[static_cast<size_t>(token_type::XOR)]            = 60;
        arr[static_cast<size_t>(token_type::PLUS)]           = 70;
        arr[static_cast<size_t>(token_type::MINUS)]          = 70;
        arr[static_cast<size_t>(token_type::MULTIPLICATION)] = 80;
        arr[static_cast<size_t>(token_type::DIVISION)]       = 80;
        arr[static_cast<size_t>(token_type::MODULO)]         = 80;
        arr[static_cast<size_t>(token_type::INCREMENT)]      = 90;
        arr[static_cast<size_t>(token_type::DECREMENT)]      = 90;

        return arr;
    }();

    return arr;
}();

//****************************************************************************//
//                            Forward declarations                            //
//****************************************************************************//

struct program_parser;
struct binary_op_parser;
struct unary_op_parser;
struct func_def_parser;
struct procedure_def_parser;
struct signature_parser;
struct return_stmt_parser;
struct parameter_def_parser;
struct var_decl_parser;
struct var_init_parser;
struct var_assignment_parser;
struct call_parser;
struct parameter_pass_parser;
struct block_parser;
struct control_block_parser;
struct if_stmt_parser;
struct else_if_stmt_parser;
struct else_stmt_parser;
struct for_loop_parser;
struct while_loop_parser;
struct switch_parser;
struct case_parser;
struct statement_parser;
struct expression_parser;


template <token_type wanted>
struct token_parser;

//****************************************************************************//
//                                   Parsers                                  //
//****************************************************************************//
template <token_type wanted>
struct token_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        if (ts[0].type == wanted)
        {
            // return {ts.subspan(1), ast_node_t{empty_node{}}};
            return parse_result(std::in_place_type<parse_content>,
                                ts.subspan(1),
                                std::make_unique<ast_node_t>(leaf_node{ts[0]}));
        }
        return parse_error(parsed_structure, ts[0]);
    };
};

struct program_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto program = combinators::many<combinators::any<var_init_parser,
                                                          var_decl_parser,
                                                          procedure_def_parser,
                                                          func_def_parser>>::parse(ts);


        if (!are_all_parse_results_valid(program))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto new_location = get_source_location_from_compound(program);

        std::vector<std::unique_ptr<ast_node_t>> globals;
        globals.reserve(program.size());

        std::ranges::for_each(program, [&globals](auto&& element) {
            globals.push_back(
                std::move(get_node(std::forward<decltype(element)>(element))));
        });

        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<program_node>, std::move(globals), new_location);

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(program.back()),
                            std::move(new_node));
    }
};


// TODO: Implement pratt parser
// NOTE: Passing the lhs avoids indirect left-recursion
struct binary_op_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts, parse_result& lhs)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto bin_op =
            combinators::all<combinators::any<token_parser<token_type::PLUS>,
                                              token_parser<token_type::MINUS>,
                                              token_parser<token_type::MULTIPLICATION>,
                                              token_parser<token_type::DIVISION>,
                                              token_parser<token_type::MODULO>,
                                              token_parser<token_type::LESS>,
                                              token_parser<token_type::LESSEQ>,
                                              token_parser<token_type::GREATER>,
                                              token_parser<token_type::GREATEREQ>,
                                              token_parser<token_type::EQUAL>,
                                              token_parser<token_type::NEQUAL>,
                                              token_parser<token_type::LOGICAL_AND>,
                                              token_parser<token_type::LOGICAL_OR>,
                                              token_parser<token_type::BINARY_AND>,
                                              token_parser<token_type::BINARY_OR>,
                                              token_parser<token_type::XOR>,
                                              token_parser<token_type::LSHIFT>,
                                              token_parser<token_type::RSHIFT>>,
                             expression_parser>::parse(ts);

        if (!are_all_parse_results_valid(bin_op))
        {
            return parse_error(parsed_structure, ts[0]);
        }


        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<binary_op_node>,
                                         get_node(lhs),
                                         get_node(bin_op[1]),
                                         get_node(bin_op[0]),
                                         get_source_location_from_compound(bin_op));


        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(bin_op.back()),
                            std::move(new_node));
    }
};

struct unary_op_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto unary_op = combinators::all<

            combinators::any<token_parser<token_type::NOT>,
                             token_parser<token_type::INCREMENT>,
                             token_parser<token_type::DECREMENT>>,
            expression_parser>::parse(ts);

        if (!are_all_parse_results_valid(unary_op))
        {
            return parse_error(parsed_structure, ts[0]);
        }


        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<unary_op_node>,
                                         get_node(unary_op[1]),
                                         get_node(unary_op[0]),
                                         get_source_location_from_compound(unary_op));


        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(unary_op.back()),
                            std::move(new_node));
    }
};

struct expression_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        // TODO: Handle parenthesesed expressions
        auto expression =
            combinators::any<unary_op_parser,
                             call_parser,
                             token_parser<token_type::LITERAL>,
                             token_parser<token_type::IDENTIFIER>>::parse(ts);

        if (!is_any_parse_result_valid(expression))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        // NOTE: Passing the lhs avoids indirect left-recursion
        if (!(get_token_stream(expression.back()).empty()))
        {
            auto bin_op = binary_op_parser::parse(get_token_stream(expression.back()),
                                                  expression.back());

            if (std::holds_alternative<parse_content>(bin_op))
            {
                expression.back() = std::move(bin_op);
            }
        }

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(expression.back()),
                            std::move(get_node(expression.back())));
    }
};

struct func_def_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto func_def = combinators::all<token_parser<token_type::FUNCTION>,
                                         signature_parser,
                                         block_parser>::parse(ts);

        if (!are_all_parse_results_valid(func_def))
        {
            return parse_error(parsed_structure, ts[0]);
        }


        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<func_def_node>,
                                         get_node(func_def[1]),
                                         get_node(func_def[2]),
                                         get_source_location_from_compound(func_def));


        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(func_def.back()),
                            std::move(new_node));
    }
};

struct procedure_def_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto procedure_def = combinators::all<token_parser<token_type::PROCEDURE>,
                                              signature_parser,
                                              block_parser>::parse(ts);

        if (!are_all_parse_results_valid(procedure_def))
        {
            return parse_error(parsed_structure, ts[0]);
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<procedure_def_node>,
            get_node(procedure_def[1]),
            get_node(procedure_def[2]),
            get_source_location_from_compound(procedure_def));


        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(procedure_def.back()),
                            std::move(new_node));
    }
};

struct signature_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto signature = combinators::all<token_parser<token_type::IDENTIFIER>,
                                          parameter_def_parser>::parse(ts);

        if (!are_all_parse_results_valid(signature))
        {
            return parse_error(parsed_structure, ts[0]);
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<signature_node>,
            std::get<leaf_node>(*get_node(signature[0])).value,
            get_node(signature[1]),
            get_source_location_from_compound(signature));

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(signature.back()),
                            std::move(new_node));
    }
};

struct return_stmt_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto return_stmt =
            combinators::all<token_parser<token_type::RETURN>,
                             expression_parser,
                             token_parser<token_type::SEMICOLON>>::parse(ts);

        if (!are_all_parse_results_valid(return_stmt))
        {
            return parse_error(parsed_structure, ts[0]);
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<return_stmt_node>,
            get_node(return_stmt[1]),
            get_source_location_from_compound(return_stmt));

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(return_stmt.back()),
                            std::move(new_node));
    }
};

struct parameter_def_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto parameter_def = combinators::surrounded<
            token_parser<token_type::LPAREN>,
            token_parser<token_type::RPAREN>,
            combinators::optional<combinators::any<
                combinators::separated<token_parser<token_type::COMMA>,
                                       token_parser<token_type::IDENTIFIER>>,
                token_parser<token_type::IDENTIFIER>>>>::parse(ts);

        if (!are_all_parse_results_valid(parameter_def))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto parameters = std::vector<std::string_view>();
        // Drop surrounder and separators
        parameters.reserve((parameter_def.size() - 2) / 2 + 1);

        std::unique_ptr<ast_node_t> new_node;

        if (parameter_def.size() == 3
            && std::holds_alternative<missing_optional_node>(
                (*get_node(parameter_def[1]))))
        {
            new_node = std::make_unique<ast_node_t>(
                std::in_place_type<parameter_def_node>,
                std::move(parameters),
                get_source_location_from_compound(parameter_def));
        }
        else
        {
            for (decltype(parameter_def)::size_type i = 1; i < parameter_def.size();
                 i += 2)
            {
                parameters.push_back(
                    std::move(std::get<leaf_node>(*get_node(parameter_def[i]))).value);
            }

            auto source_location = get_source_location_from_compound(parameter_def);

            new_node =
                std::make_unique<ast_node_t>(std::in_place_type<parameter_def_node>,
                                             std::move(parameters),
                                             source_location);
        }

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(parameter_def.back()),
                            std::move(new_node));
    }
};

struct var_decl_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto var_decl =
            combinators::all<token_parser<token_type::LET>,
                             token_parser<token_type::IDENTIFIER>,
                             token_parser<token_type::SEMICOLON>>::parse(ts);

        if (!are_all_parse_results_valid(var_decl))
        {
            return parse_error(parsed_structure, ts[0]);
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<var_decl_node>,
            std::get<leaf_node>(*get_node(var_decl[1])).value,
            get_source_location_from_compound(var_decl));

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(var_decl.back()),
                            std::move(new_node));
    }
};

struct var_init_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto var_init =
            combinators::all<token_parser<token_type::LET>,
                             token_parser<token_type::IDENTIFIER>,
                             token_parser<token_type::ASSIGN>,
                             expression_parser,
                             token_parser<token_type::SEMICOLON>>::parse(ts);

        if (!are_all_parse_results_valid(var_init))
        {
            return parse_error(parsed_structure, ts[0]);
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<var_init_node>,
            std::get<leaf_node>(*get_node(var_init[1])).value,
            get_node(var_init[3]),
            get_source_location_from_compound(var_init));

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(var_init.back()),
                            std::move(new_node));
    }
};

struct var_assignment_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto var_assignment =
            combinators::all<token_parser<token_type::IDENTIFIER>,
                             token_parser<token_type::ASSIGN>,
                             expression_parser,
                             token_parser<token_type::SEMICOLON>>::parse(ts);

        if (!are_all_parse_results_valid(var_assignment))
        {
            return parse_error(parsed_structure, ts[0]);
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<var_assignment_node>,
            std::get<leaf_node>(*get_node(var_assignment[0])).value,
            get_node(var_assignment[2]),
            get_source_location_from_compound(var_assignment));

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(var_assignment.back()),
                            std::move(new_node));
    }
};

struct call_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto call = combinators::all<token_parser<token_type::IDENTIFIER>,
                                     parameter_pass_parser>::parse(ts);

        if (!are_all_parse_results_valid(call))
        {
            return parse_error(parsed_structure, ts[0]);
        }


        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<call_node>,
                                         std::get<leaf_node>(*get_node(call[0])).value,
                                         get_node(call[1]),
                                         get_source_location_from_compound(call));

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(call.back()),
                            std::move(new_node));
    }
};

struct parameter_pass_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto parameter_pass = combinators::surrounded<
            token_parser<token_type::LPAREN>,
            token_parser<token_type::RPAREN>,
            combinators::optional<
                combinators::any<combinators::separated<token_parser<token_type::COMMA>,
                                                        expression_parser>,
                                 expression_parser>>>::parse(ts);

        if (!are_all_parse_results_valid(parameter_pass))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto parameters = std::vector<std::string_view>();
        // Drop surrounder and separators
        parameters.reserve((parameter_pass.size() - 2) / 2 + 1);

        std::unique_ptr<ast_node_t> new_node;

        if (parameter_pass.size() == 3
            && std::holds_alternative<missing_optional_node>(
                (*get_node(parameter_pass[1]))))
        {
            new_node = std::make_unique<ast_node_t>(
                std::in_place_type<parameter_pass_node>,
                std::move(parameters),
                get_source_location_from_compound(parameter_pass));
        }
        else
        {
            for (decltype(parameter_pass)::size_type i = 1; i < parameter_pass.size();
                 i += 2)
            {
                parameters.push_back(
                    std::move(std::get<leaf_node>(*get_node(parameter_pass[i]))).value);
            }

            auto source_location = get_source_location_from_compound(parameter_pass);

            new_node =
                std::make_unique<ast_node_t>(std::in_place_type<parameter_pass_node>,
                                             std::move(parameters),
                                             source_location);
        }

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(parameter_pass.back()),
                            std::move(new_node));
    }
};
struct statement_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto statement = combinators::any<var_assignment_parser,
                                          var_init_parser,
                                          var_decl_parser,
                                          expression_parser,
                                          call_parser>::parse(ts);

        if (!is_any_parse_result_valid(statement))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        return std::move(statement[0]);
    }
};
struct block_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        // TODO: Handle empty block
        auto block = combinators::all<
            token_parser<token_type::LBRACE>,
            combinators::optional<combinators::many<
                combinators::
                    any<statement_parser, control_block_parser, return_stmt_parser>>>,
            token_parser<token_type::RBRACE>>::parse(ts);

        if (!are_all_parse_results_valid(block))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto new_location = get_source_location_from_compound(block);

        std::unique_ptr<ast_node_t>              new_node;
        std::vector<std::unique_ptr<ast_node_t>> statements;
        statements.reserve(block.size());

        if (block.size() == 3
            && std::holds_alternative<missing_optional_node>((*get_node(block[1]))))
        {
            new_node =
                std::make_unique<ast_node_t>(std::in_place_type<block_node>,
                                             std::move(statements),
                                             get_source_location_from_compound(block));
        }
        else
        {
            // Make sure, that we don't copy the parentheses as statements
            std::for_each(
                block.begin() + 1, block.end() - 1, [&statements](auto&& element) {
                    statements.push_back(
                        std::move(get_node(std::forward<decltype(element)>(element))));
                });

            new_node = std::make_unique<ast_node_t>(
                std::in_place_type<block_node>, std::move(statements), new_location);
        }
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(block.back()),
                            std::move(new_node));
    }
};

struct control_block_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto control_block = combinators::any<if_stmt_parser,
                                              else_if_stmt_parser,
                                              else_stmt_parser,
                                              for_loop_parser,
                                              while_loop_parser,
                                              switch_parser,
                                              case_parser>::parse(ts);

        if (!is_any_parse_result_valid(control_block))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        return std::move(control_block[0]);
    }
};

struct if_stmt_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto if_stmt =
            combinators::all<token_parser<token_type::IF>,
                             combinators::surrounded<token_parser<token_type::LPAREN>,
                                                     token_parser<token_type::RPAREN>,
                                                     expression_parser>,
                             block_parser>::parse(ts);

        if (!are_all_parse_results_valid(if_stmt))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto new_location = get_source_location_from_compound(if_stmt);

        auto new_node = std::make_unique<ast_node_t>(std::in_place_type<if_stmt_node>,
                                                     get_node(if_stmt[2]),
                                                     get_node(if_stmt[4]),
                                                     new_location);

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(if_stmt.back()),
                            std::move(new_node));
    }
};

struct else_if_stmt_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto else_if_stmt =
            combinators::all<token_parser<token_type::ELSE>,
                             token_parser<token_type::IF>,
                             combinators::surrounded<token_parser<token_type::LPAREN>,
                                                     token_parser<token_type::RPAREN>,
                                                     expression_parser>,
                             block_parser>::parse(ts);


        if (!are_all_parse_results_valid(else_if_stmt))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto new_location = get_source_location_from_compound(else_if_stmt);

        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<else_if_stmt_node>,
                                         get_node(else_if_stmt[3]),
                                         get_node(else_if_stmt[5]),
                                         new_location);

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(else_if_stmt.back()),
                            std::move(new_node));
    }
};

struct else_stmt_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto else_stmt =
            combinators::all<token_parser<token_type::ELSE>, block_parser>::parse(ts);

        if (!are_all_parse_results_valid(else_stmt))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto new_location = get_source_location_from_compound(else_stmt);

        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<else_stmt_node>, get_node(else_stmt[1]), new_location);

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(else_stmt.back()),
                            std::move(new_node));
    }
};

struct for_loop_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto for_loop = combinators::all<
            token_parser<token_type::FOR>,
            combinators::surrounded<
                token_parser<token_type::LPAREN>,
                token_parser<token_type::RPAREN>,
                combinators::all<combinators::optional<statement_parser>,
                                 token_parser<token_type::SEMICOLON>,
                                 combinators::optional<expression_parser>,
                                 token_parser<token_type::SEMICOLON>,
                                 combinators::optional<expression_parser>>>,
            block_parser>::parse(ts);


        if (!are_all_parse_results_valid(for_loop))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        if (std::holds_alternative<missing_optional_node>(*get_node(for_loop[2])))
        {
            get_node(for_loop[2]) = nullptr;
        }
        if (std::holds_alternative<missing_optional_node>(*get_node(for_loop[4])))
        {
            get_node(for_loop[4]) = nullptr;
        }
        if (std::holds_alternative<missing_optional_node>(*get_node(for_loop[6])))
        {
            get_node(for_loop[6]) = nullptr;
        }

        auto new_location = get_source_location_from_compound(for_loop);

        auto new_node = std::make_unique<ast_node_t>(std::in_place_type<for_loop_node>,
                                                     get_node(for_loop[2]),
                                                     get_node(for_loop[4]),
                                                     get_node(for_loop[6]),
                                                     get_node(for_loop[8]),
                                                     new_location);

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(for_loop.back()),
                            std::move(new_node));
    }
};

struct while_loop_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto while_loop =
            combinators::all<token_parser<token_type::WHILE>,
                             combinators::surrounded<token_parser<token_type::LPAREN>,
                                                     token_parser<token_type::RPAREN>,
                                                     expression_parser>,
                             block_parser>::parse(ts);

        if (!are_all_parse_results_valid(while_loop))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto new_location = get_source_location_from_compound(while_loop);

        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<while_loop_node>,
                                         get_node(while_loop[2]),
                                         get_node(while_loop[4]),
                                         new_location);

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(while_loop.back()),
                            std::move(new_node));
    }
};

struct switch_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto switch_stmt =
            combinators::all<token_parser<token_type::SWITCH>,
                             combinators::surrounded<token_parser<token_type::LPAREN>,
                                                     token_parser<token_type::RPAREN>,
                                                     expression_parser>,
                             block_parser>::parse(ts);

        if (!are_all_parse_results_valid(switch_stmt))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto new_location = get_source_location_from_compound(switch_stmt);

        auto new_node = std::make_unique<ast_node_t>(std::in_place_type<switch_node>,
                                                     get_node(switch_stmt[2]),
                                                     get_node(switch_stmt[4]),
                                                     new_location);

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(switch_stmt.back()),
                            std::move(new_node));
    }
};

struct case_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }


        auto case_stmt =
            combinators::all<token_parser<token_type::CASE>,
                             expression_parser,
                             token_parser<token_type::COLON>,
                             combinators::optional<combinators::many<
                                 combinators::any<statement_parser,
                                                  control_block_parser,
                                                  return_stmt_parser>>>>::parse(ts);

        if (!are_all_parse_results_valid(case_stmt))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        std::vector<std::unique_ptr<ast_node_t>> statements;
        source_location                          new_location;

        if (std::holds_alternative<missing_optional_node>(*get_node(case_stmt[3])))
        {
            case_stmt.pop_back();
            new_location = get_source_location_from_compound(case_stmt);
        }
        else
        {
            new_location = get_source_location_from_compound(case_stmt);

            std::for_each(
                case_stmt.begin(), case_stmt.end() - 1, [&statements](auto&& element) {
                    statements.push_back(
                        std::move(get_node(std::forward<decltype(element)>(element))));
                });
        }
        source_location body_location;

        auto body = std::make_unique<ast_node_t>(
            std::in_place_type<block_node>, std::move(statements), body_location);

        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<case_node>, get_node(case_stmt[1]), body, new_location);

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(case_stmt.back()),
                            std::move(new_node));
    }
};
//****************************************************************************//
//                                 Public API                                 //
//****************************************************************************//
inline std::unique_ptr<ast_node_t> parse(std::span<token> ts)
{
    auto result = program_parser::parse(ts);

    if (std::holds_alternative<parse_error>(result))
    {
        std::get<parse_error>(result).throw_();
    }
    // return get_node(std::get<parse_content>(result));
    return std::get<1>(std::get<parse_content>(program_parser::parse(ts)));
}
