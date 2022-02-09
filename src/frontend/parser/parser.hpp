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
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <retrieve_source_location.hpp>
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


        arr[static_cast<size_t>(token_type::INCREMENT)]      = 90;
        arr[static_cast<size_t>(token_type::DECREMENT)]      = 90;
        arr[static_cast<size_t>(token_type::NOT)]            = 90;
        arr[static_cast<size_t>(token_type::MULTIPLICATION)] = 80;
        arr[static_cast<size_t>(token_type::DIVISION)]       = 80;
        arr[static_cast<size_t>(token_type::MODULO)]         = 80;
        arr[static_cast<size_t>(token_type::PLUS)]           = 70;
        arr[static_cast<size_t>(token_type::MINUS)]          = 70;
        arr[static_cast<size_t>(token_type::BINARY_AND)]     = 60;
        arr[static_cast<size_t>(token_type::BINARY_OR)]      = 60;
        arr[static_cast<size_t>(token_type::LSHIFT)]         = 60;
        arr[static_cast<size_t>(token_type::RSHIFT)]         = 60;
        arr[static_cast<size_t>(token_type::XOR)]            = 60;
        arr[static_cast<size_t>(token_type::LESS)]           = 50;
        arr[static_cast<size_t>(token_type::LESSEQ)]         = 50;
        arr[static_cast<size_t>(token_type::GREATER)]        = 50;
        arr[static_cast<size_t>(token_type::GREATEREQ)]      = 50;
        arr[static_cast<size_t>(token_type::EQUAL)]          = 50;
        arr[static_cast<size_t>(token_type::NEQUAL)]         = 50;
        arr[static_cast<size_t>(token_type::LOGICAL_AND)]    = 50;
        arr[static_cast<size_t>(token_type::LOGICAL_OR)]     = 50;

        return arr;
    }();

    return arr;
}();

//****************************************************************************//
//                            Forward declarations                            //
//****************************************************************************//

struct program_parser;
struct binary_op_parser;
struct expression_parser;
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

template <token_type wanted>
struct token_parser;

//****************************************************************************//
//                                   Parsers                                  //
//****************************************************************************//

template <token_type wanted>
struct token_parser
{
    static constexpr std::string_view parsed_structure = "token";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure, ts[0].value);

        if (ts[0].type == wanted)
        {
            log_parse_success(parsed_structure, ts[0].value);
            return parse_result(std::in_place_type<parse_content>,
                                ts.subspan(1),
                                std::make_unique<ast_node_t>(leaf_node{ts[0]}));
        }
        log_parse_error(parsed_structure,
                        ts[0].value,
                        LUT_TOKEN_TO_LEXEME.at(static_cast<size_t>(wanted)));
        return parse_error(parsed_structure, ts[0]);
    };
};

struct program_parser
{
    static constexpr std::string_view parsed_structure = "progam";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto program = combinators::many<combinators::any<var_init_parser,
                                                          var_decl_parser,
                                                          procedure_def_parser,
                                                          func_def_parser>>::parse(ts);


        if (!are_all_parse_results_valid(program))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(program.back());
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

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(program.back()),
                            std::move(new_node));
    }
};

struct binary_op_parser
{
    static constexpr std::string_view parsed_structure = "binary operation";

    static parse_result parse(std::span<token> ts,
                              parse_result&    lhs,
                              int              previous_operator_precedence = 0);
};

struct expression_parser
{
    static constexpr std::string_view parsed_structure = "expression";

    static parse_result parse(std::span<token> ts,
                              int              previous_operator_precedence = 0);
};

parse_result binary_op_parser::parse(std::span<token> ts,
                                     parse_result&    lhs,
                                     int              previous_operator_precedence)
{
    if (ts.empty())
    {
        return parse_error(parsed_structure);
    }

    log_parse_attempt(parsed_structure);

    std::vector<parse_result> bin_op =
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
                                          token_parser<token_type::RSHIFT>>>::parse(ts);

    if (!are_all_parse_results_valid(bin_op))
    {
        log_parse_error(parsed_structure);
        return std::get<parse_error>(bin_op.back());
    }

    bin_op.push_back(expression_parser::parse(get_token_stream(bin_op.back()),
                                              previous_operator_precedence));


    auto new_node =
        std::make_unique<ast_node_t>(std::in_place_type<binary_op_node>,
                                     get_node(lhs),
                                     get_node(bin_op[1]),
                                     get_node(bin_op[0]),
                                     get_source_location_from_compound(bin_op));


    log_parse_success(parsed_structure);
    return parse_result(std::in_place_type<parse_content>,
                        get_token_stream(bin_op.back()),
                        std::move(new_node));
}

parse_result expression_parser::parse(std::span<token> ts,
                                      int              previous_operator_precedence)
{
    if (ts.empty())
    {
        return parse_error(parsed_structure);
    }

    log_parse_attempt(parsed_structure);

    // TODO: Handle parenthesesed expressions
    // TODO: Grammer: expression := [terminal] [lparen expression rparen]

    // FIX: With the current implementation an expression fails to parse with a ts like
    // (5 * 5) + 1, because of the parens, should we lhs =
    // parenthesized_expression>>parse(ts)?
    // auto lhs = combinators::all<
    //     combinators::optional<token_parser<token_type::LPAREN>>,
    //     combinators::any<unary_op_parser,
    //                      call_parser,
    //                      token_parser<token_type::LITERAL>,
    //                      token_parser<token_type::IDENTIFIER>>>::parse(ts);


    // auto lhs = combinators::any<
    //     combinators::all<token_parser<token_type::LPAREN>,
    //                      combinators::any<unary_op_parser,
    //                                       call_parser,
    //                                       token_parser<token_type::LITERAL>,
    //                                       token_parser<token_type::IDENTIFIER>>,
    //                      token_parser<token_type::RPAREN>>,
    //     combinators::any<unary_op_parser,
    //                      call_parser,
    //                      token_parser<token_type::LITERAL>,
    //                      token_parser<token_type::IDENTIFIER>>>::parse(ts);


    auto lhs = combinators::any<
        combinators::all<token_parser<token_type::LPAREN>,
                         expression_parser,
                         token_parser<token_type::RPAREN>>,
        combinators::any<unary_op_parser,
                         call_parser,
                         token_parser<token_type::LITERAL>,
                         token_parser<token_type::IDENTIFIER>>>::parse(ts);

    if (!are_all_parse_results_valid(lhs))
    {
        log_parse_error(parsed_structure);
        return std::get<parse_error>(lhs.back());
    }

    source_location  location_start;
    source_location  location_end;
    std::span<token> ts_end;
    // bool             was_parenthesized = false;
    if (lhs.size() == 3)
    {
        location_start =
            std::visit(source_location_retriever_visitor(), *get_node(lhs.front()));
        location_end =
            std::visit(source_location_retriever_visitor(), *get_node(lhs.back()));
        auto expr = std::move(lhs[1]);
        ts_end    = get_token_stream(lhs.back());

        lhs.clear();
        lhs.push_back(std::move(expr));

        // was_parenthesized = true;
        get_token_stream(lhs.front()) = ts_end;
        auto& lhs_expr =
            std::visit(source_location_retriever_visitor(), *get_node(lhs.front()));

        lhs_expr.col_start  = location_start.col_start;
        lhs_expr.line_start = location_start.line_start;

        lhs_expr.col_end  = location_end.col_end;
        lhs_expr.line_end = location_end.line_end;
    }


    // NOTE: Passing the lhs avoids indirect left-recursion
    while (!get_token_stream(lhs.back()).empty()
           && LUT_TOKEN_TO_PRECEDENCE.at(
                  static_cast<size_t>(get_token_stream(lhs.back())[0].type))
                  > previous_operator_precedence)
    {
        auto bin_op = binary_op_parser::parse(
            get_token_stream(lhs.back()),
            lhs.back(),
            LUT_TOKEN_TO_PRECEDENCE.at(
                static_cast<size_t>(get_token_stream(lhs.back())[0].type)));

        if (std::holds_alternative<parse_error>(bin_op))
        {
            // TODO: To simplify error handling, could we build a global error
            // list and and throw the one, which parsed the most tokens?

            // FIX: This gives more correct error messages, but gives false errors
            // when parsing a non-binary-operation expression
            //
            // log_parse_error(parsed_structure);
            //
            // return std::get<parse_error>(bin_op);
        }
        else
        {
            lhs.back() = std::move(bin_op);
        }
    }

    // TODO: If we find an RPAREN here, there is a syntax error!

    log_parse_success(parsed_structure);
    return parse_result(std::in_place_type<parse_content>,
                        get_token_stream(lhs.back()),
                        std::move(get_node(lhs.back())));
}

struct unary_op_parser
{
    static constexpr std::string_view parsed_structure = "unary operation";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto unary_op = combinators::all<

            combinators::any<token_parser<token_type::NOT>,
                             token_parser<token_type::INCREMENT>,
                             token_parser<token_type::DECREMENT>>,
            expression_parser>::parse(ts);

        if (!are_all_parse_results_valid(unary_op))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(unary_op.back());
        }


        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<unary_op_node>,
                                         get_node(unary_op[1]),
                                         get_node(unary_op[0]),
                                         get_source_location_from_compound(unary_op));


        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(unary_op.back()),
                            std::move(new_node));
    }
};

struct func_def_parser
{
    static constexpr std::string_view parsed_structure = "fuction definition";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto func_def = combinators::all<token_parser<token_type::FUNCTION>,
                                         signature_parser,
                                         block_parser>::parse(ts);

        if (!are_all_parse_results_valid(func_def))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(func_def.back());
        }


        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<func_def_node>,
                                         get_node(func_def[1]),
                                         get_node(func_def[2]),
                                         get_source_location_from_compound(func_def));


        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(func_def.back()),
                            std::move(new_node));
    }
};

struct procedure_def_parser
{
    static constexpr std::string_view parsed_structure = "procedure definition";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto procedure_def = combinators::all<token_parser<token_type::PROCEDURE>,
                                              signature_parser,
                                              block_parser>::parse(ts);

        if (!are_all_parse_results_valid(procedure_def))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(procedure_def.back());
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<procedure_def_node>,
            get_node(procedure_def[1]),
            get_node(procedure_def[2]),
            get_source_location_from_compound(procedure_def));


        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(procedure_def.back()),
                            std::move(new_node));
    }
};

struct signature_parser
{
    static constexpr std::string_view parsed_structure = "function signature";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto signature = combinators::all<token_parser<token_type::IDENTIFIER>,
                                          parameter_def_parser>::parse(ts);

        if (!are_all_parse_results_valid(signature))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(signature.back());
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<signature_node>,
            std::get<leaf_node>(*get_node(signature[0])).value,
            get_node(signature[1]),
            get_source_location_from_compound(signature));

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(signature.back()),
                            std::move(new_node));
    }
};

struct return_stmt_parser
{
    static constexpr std::string_view parsed_structure = "return statement";

    // TODO: The expression should be optional, to allow returning in procedures
    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto return_stmt =
            combinators::all<token_parser<token_type::RETURN>,
                             combinators::optional<expression_parser>,
                             token_parser<token_type::SEMICOLON>>::parse(ts);

        if (!are_all_parse_results_valid(return_stmt))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(return_stmt.back());
        }


        if (std::holds_alternative<missing_optional_node>(*get_node(return_stmt[1])))
        {
            get_node(return_stmt[1]) = nullptr;
        }
        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<return_stmt_node>,
            get_node(return_stmt[1]),
            get_source_location_from_compound(return_stmt));

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(return_stmt.back()),
                            std::move(new_node));
    }
};

struct parameter_def_parser
{
    static constexpr std::string_view parsed_structure = "callable parameter defintion";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto parameter_def = combinators::surrounded<
            token_parser<token_type::LPAREN>,
            token_parser<token_type::RPAREN>,
            combinators::optional<combinators::any<
                combinators::separated<token_parser<token_type::COMMA>,
                                       token_parser<token_type::IDENTIFIER>>,
                token_parser<token_type::IDENTIFIER>>>>::parse(ts);

        if (!are_all_parse_results_valid(parameter_def))
        {
            log_parse_error(parsed_structure);

            parse_error err;

            if (parameter_def.size() > 1
                && std::holds_alternative<missing_optional_node>(
                    *get_node(*(parameter_def.end() - 2))))
            {
                err = get_parse_error(*(parameter_def.end() - 2));
            }
            else
            {
                err = get_parse_error(parameter_def.back());
            }

            return err;
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

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(parameter_def.back()),
                            std::move(new_node));
    }
};

struct var_decl_parser
{
    static constexpr std::string_view parsed_structure = "variable declaration";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto var_decl =
            combinators::all<token_parser<token_type::LET>,
                             token_parser<token_type::IDENTIFIER>,
                             token_parser<token_type::SEMICOLON>>::parse(ts);

        if (!are_all_parse_results_valid(var_decl))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(var_decl.back());
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<var_decl_node>,
            std::get<leaf_node>(*get_node(var_decl[1])).value,
            get_source_location_from_compound(var_decl));

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(var_decl.back()),
                            std::move(new_node));
    }
};

struct var_init_parser
{
    static constexpr std::string_view parsed_structure = "variable initialization";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto var_init =
            combinators::all<token_parser<token_type::LET>,
                             token_parser<token_type::IDENTIFIER>,
                             token_parser<token_type::ASSIGN>,
                             expression_parser,
                             token_parser<token_type::SEMICOLON>>::parse(ts);

        if (!are_all_parse_results_valid(var_init))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(var_init.back());
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<var_init_node>,
            std::get<leaf_node>(*get_node(var_init[1])).value,
            get_node(var_init[3]),
            get_source_location_from_compound(var_init));

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(var_init.back()),
                            std::move(new_node));
    }
};

struct var_assignment_parser
{
    static constexpr std::string_view parsed_structure = "variable assignment";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto var_assignment =
            combinators::all<token_parser<token_type::IDENTIFIER>,
                             token_parser<token_type::ASSIGN>,
                             expression_parser,
                             token_parser<token_type::SEMICOLON>>::parse(ts);

        if (!are_all_parse_results_valid(var_assignment))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(var_assignment.back());
        }


        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<var_assignment_node>,
            std::get<leaf_node>(*get_node(var_assignment[0])).value,
            get_node(var_assignment[2]),
            get_source_location_from_compound(var_assignment));

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(var_assignment.back()),
                            std::move(new_node));
    }
};

struct call_parser
{
    static constexpr std::string_view parsed_structure = "call";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto call = combinators::all<token_parser<token_type::IDENTIFIER>,
                                     parameter_pass_parser>::parse(ts);

        if (!are_all_parse_results_valid(call))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(call.back());
        }


        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<call_node>,
                                         std::get<leaf_node>(*get_node(call[0])).value,
                                         get_node(call[1]),
                                         get_source_location_from_compound(call));

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(call.back()),
                            std::move(new_node));
    }
};

struct parameter_pass_parser
{
    static constexpr std::string_view parsed_structure = "callable parameter pass";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto parameter_pass = combinators::surrounded<
            token_parser<token_type::LPAREN>,
            token_parser<token_type::RPAREN>,
            combinators::optional<
                combinators::any<combinators::separated<token_parser<token_type::COMMA>,
                                                        expression_parser>,
                                 expression_parser>>>::parse(ts);

        if (!are_all_parse_results_valid(parameter_pass))
        {
            log_parse_error(parsed_structure);

            parse_error err;

            if (parameter_pass.size() > 1
                && std::holds_alternative<missing_optional_node>(
                    *get_node(*(parameter_pass.end() - 2))))
            {
                err = std::get<missing_optional_node>(
                          *get_node(*(parameter_pass.end() - 2)))
                          .encountered_error;
            }
            else
            {
                err = get_parse_error(parameter_pass.back());
            }

            return err;
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

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(parameter_pass.back()),
                            std::move(new_node));
    }
};

struct statement_parser
{
    static constexpr std::string_view parsed_structure = "statement";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto statement = combinators::any<
            var_assignment_parser,
            var_init_parser,
            var_decl_parser,
            combinators::all<expression_parser,
                             token_parser<token_type::SEMICOLON>>>::parse(ts);

        if (!is_any_parse_result_valid(statement))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(statement.back());
        }

        // TODO: Check if the size is always 1 or 2
        if (statement.size() == 2)
        {
            // The caller does not care about parsed semicolon,
            // but we still need to drop it from the token stream
            get_token_stream(statement[0]) = get_token_stream(statement[1]);
        }

        return std::move(statement[0]);
    }
};

struct block_parser
{
    static constexpr std::string_view parsed_structure = "code block";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto block = combinators::all<
            token_parser<token_type::LBRACE>,
            combinators::optional<combinators::many<
                combinators::
                    any<statement_parser, control_block_parser, return_stmt_parser>>>,
            token_parser<token_type::RBRACE>>::parse(ts);

        if (!are_all_parse_results_valid(block))
        {
            log_parse_error(parsed_structure);

            parse_error err;

            if (block.size() > 1
                && std::holds_alternative<missing_optional_node>(
                    *get_node(*(block.end() - 2))))
            {
                err = std::get<missing_optional_node>(*get_node(*(block.end() - 2)))
                          .encountered_error;
            }
            else
            {
                err = get_parse_error(block.back());
            }

            return err;
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
            // Make sure, that we don't copy the curly braces as statements
            std::for_each(
                block.begin() + 1, block.end() - 1, [&statements](auto&& element) {
                    statements.push_back(
                        std::move(get_node(std::forward<decltype(element)>(element))));
                });

            new_node = std::make_unique<ast_node_t>(
                std::in_place_type<block_node>, std::move(statements), new_location);
        }
        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(block.back()),
                            std::move(new_node));
    }
};

struct control_block_parser
{
    static constexpr std::string_view parsed_structure = "control block";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto control_block = combinators::any<if_stmt_parser,
                                              else_if_stmt_parser,
                                              else_stmt_parser,
                                              for_loop_parser,
                                              while_loop_parser,
                                              switch_parser>::parse(ts);

        if (!is_any_parse_result_valid(control_block))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(control_block.back());
        }

        return std::move(control_block[0]);
    }
};

struct if_stmt_parser
{
    static constexpr std::string_view parsed_structure = "if statement";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto if_stmt =
            combinators::all<token_parser<token_type::IF>,
                             combinators::surrounded<token_parser<token_type::LPAREN>,
                                                     token_parser<token_type::RPAREN>,
                                                     expression_parser>,
                             block_parser>::parse(ts);

        if (!are_all_parse_results_valid(if_stmt))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(if_stmt.back());
        }

        auto new_location = get_source_location_from_compound(if_stmt);

        auto new_node = std::make_unique<ast_node_t>(std::in_place_type<if_stmt_node>,
                                                     get_node(if_stmt[2]),
                                                     get_node(if_stmt[4]),
                                                     new_location);

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(if_stmt.back()),
                            std::move(new_node));
    }
};

struct else_if_stmt_parser
{
    static constexpr std::string_view parsed_structure = "else if statement";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto else_if_stmt =
            combinators::all<token_parser<token_type::ELSE>,
                             token_parser<token_type::IF>,
                             combinators::surrounded<token_parser<token_type::LPAREN>,
                                                     token_parser<token_type::RPAREN>,
                                                     expression_parser>,
                             block_parser>::parse(ts);


        if (!are_all_parse_results_valid(else_if_stmt))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(else_if_stmt.back());
        }

        auto new_location = get_source_location_from_compound(else_if_stmt);

        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<else_if_stmt_node>,
                                         get_node(else_if_stmt[3]),
                                         get_node(else_if_stmt[5]),
                                         new_location);

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(else_if_stmt.back()),
                            std::move(new_node));
    }
};

struct else_stmt_parser
{
    static constexpr std::string_view parsed_structure = "else statement";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto else_stmt =
            combinators::all<token_parser<token_type::ELSE>, block_parser>::parse(ts);

        if (!are_all_parse_results_valid(else_stmt))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(else_stmt.back());
        }

        auto new_location = get_source_location_from_compound(else_stmt);

        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<else_stmt_node>, get_node(else_stmt[1]), new_location);

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(else_stmt.back()),
                            std::move(new_node));
    }
};

struct for_loop_parser
{
    static constexpr std::string_view parsed_structure = "for loop";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto for_loop = combinators::all<
            token_parser<token_type::FOR>,
            combinators::surrounded<
                token_parser<token_type::LPAREN>,
                token_parser<token_type::RPAREN>,
                // Statements already contain semicolons
                combinators::any<token_parser<token_type::SEMICOLON>,
                                 combinators::optional<statement_parser>>,
                combinators::optional<expression_parser>,
                token_parser<token_type::SEMICOLON>,
                combinators::optional<expression_parser>>,
            block_parser>::parse(ts);

        if (!are_all_parse_results_valid(for_loop))
        {
            log_parse_error(parsed_structure);

            parse_error err;

            if (for_loop.size() > 1
                && std::holds_alternative<missing_optional_node>(
                    *get_node(*(for_loop.end() - 2))))
            {
                err = std::get<missing_optional_node>(*get_node(*(for_loop.end() - 2)))
                          .encountered_error;
            }
            else
            {
                err = get_parse_error(for_loop.back());
            }

            return err;
        }


        if (std::holds_alternative<leaf_node>(*get_node(for_loop[2])))
        {
            get_node(for_loop[2]) = nullptr;
        }
        if (std::holds_alternative<missing_optional_node>(*get_node(for_loop[3])))
        {
            get_node(for_loop[3]) = nullptr;
        }
        if (std::holds_alternative<missing_optional_node>(*get_node(for_loop[5])))
        {
            get_node(for_loop[5]) = nullptr;
        }

        auto new_location = get_source_location_from_compound(for_loop);

        auto new_node = std::make_unique<ast_node_t>(std::in_place_type<for_loop_node>,
                                                     get_node(for_loop[2]),
                                                     get_node(for_loop[3]),
                                                     get_node(for_loop[5]),
                                                     get_node(for_loop[7]),
                                                     new_location);

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(for_loop.back()),
                            std::move(new_node));
    }
};

struct while_loop_parser
{
    static constexpr std::string_view parsed_structure = "while loop";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto while_loop =
            combinators::all<token_parser<token_type::WHILE>,
                             combinators::surrounded<token_parser<token_type::LPAREN>,
                                                     token_parser<token_type::RPAREN>,
                                                     expression_parser>,
                             block_parser>::parse(ts);

        if (!are_all_parse_results_valid(while_loop))
        {
            log_parse_error(parsed_structure);
            return std::get<parse_error>(while_loop.back());
        }

        auto new_location = get_source_location_from_compound(while_loop);

        auto new_node =
            std::make_unique<ast_node_t>(std::in_place_type<while_loop_node>,
                                         get_node(while_loop[2]),
                                         get_node(while_loop[4]),
                                         new_location);

        log_parse_success(parsed_structure);
        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(while_loop.back()),
                            std::move(new_node));
    }
};

struct switch_parser
{
    static constexpr std::string_view parsed_structure = "switch statement";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);

        auto switch_stmt = combinators::all<
            token_parser<token_type::SWITCH>,
            combinators::surrounded<token_parser<token_type::LPAREN>,
                                    token_parser<token_type::RPAREN>,
                                    expression_parser>,
            combinators::surrounded<
                token_parser<token_type::LBRACE>,
                token_parser<token_type::RBRACE>,
                combinators::optional<combinators::many<case_parser>>>>::parse(ts);

        if (!are_all_parse_results_valid(switch_stmt))
        {
            log_parse_error(parsed_structure);

            parse_error err;

            if (switch_stmt.size() > 1
                && std::holds_alternative<missing_optional_node>(
                    *get_node(*(switch_stmt.end() - 2))))
            {
                err =
                    std::get<missing_optional_node>(*get_node(*(switch_stmt.end() - 2)))
                        .encountered_error;
            }
            else
            {
                err = get_parse_error(switch_stmt.back());
            }

            return err;
        }

        auto            new_ts = get_token_stream(switch_stmt.back());
        source_location new_location;
        source_location body_location;
        std::vector<std::unique_ptr<ast_node_t>> cases;

        if (std::holds_alternative<missing_optional_node>(*get_node(switch_stmt[5])))
        {
            new_location  = get_source_location_from_compound(switch_stmt);
            body_location = get_source_location_from_compound(switch_stmt.begin() + 4,
                                                              switch_stmt.begin() + 4);
        }
        else
        {
            new_location = get_source_location_from_compound(switch_stmt);

            body_location = get_source_location_from_compound(switch_stmt.begin() + 4,
                                                              switch_stmt.end() - 2);

            std::for_each(switch_stmt.begin() + 5,
                          switch_stmt.end() - 1,
                          [&cases](auto&& element) {
                              cases.push_back(std::move(
                                  get_node(std::forward<decltype(element)>(element))));
                          });
        }


        auto body = std::make_unique<ast_node_t>(
            std::in_place_type<block_node>, std::move(cases), body_location);

        auto new_node = std::make_unique<ast_node_t>(std::in_place_type<switch_node>,
                                                     get_node(switch_stmt[2]),
                                                     body,
                                                     new_location);

        log_parse_success(parsed_structure);
        return parse_result(
            std::in_place_type<parse_content>, new_ts, std::move(new_node));
    }
};

struct case_parser
{
    static constexpr std::string_view parsed_structure = "case statement";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        log_parse_attempt(parsed_structure);


        auto case_stmt =
            combinators::all<token_parser<token_type::CASE>,
                             token_parser<token_type::LITERAL>,
                             token_parser<token_type::COLON>,
                             combinators::optional<combinators::many<
                                 combinators::any<statement_parser,
                                                  control_block_parser,
                                                  return_stmt_parser>>>>::parse(ts);

        if (!are_all_parse_results_valid(case_stmt))
        {
            log_parse_error(parsed_structure);

            parse_error err;

            if (case_stmt.size() > 1
                && std::holds_alternative<missing_optional_node>(
                    *get_node(*(case_stmt.end() - 2))))
            {
                err = std::get<missing_optional_node>(*get_node(*(case_stmt.end() - 2)))
                          .encountered_error;
            }
            else
            {
                err = get_parse_error(case_stmt.back());
            }

            return err;
        }


        std::vector<std::unique_ptr<ast_node_t>> statements;
        source_location                          new_location;
        source_location                          body_location;
        std::span<token>                         new_ts;

        if (std::holds_alternative<missing_optional_node>(*get_node(case_stmt[3])))
        {
            case_stmt.pop_back();
            new_location  = get_source_location_from_compound(case_stmt);
            body_location = get_source_location_from_compound(case_stmt.begin() + 2,
                                                              case_stmt.begin() + 2);

            new_ts = get_token_stream(case_stmt.back());
        }
        else
        {
            new_location  = get_source_location_from_compound(case_stmt);
            body_location = get_source_location_from_compound(case_stmt.begin() + 3,
                                                              case_stmt.end() - 1);

            new_ts = get_token_stream(case_stmt.back());

            std::for_each(
                case_stmt.begin() + 3, case_stmt.end(), [&statements](auto&& element) {
                    statements.push_back(
                        std::move(get_node(std::forward<decltype(element)>(element))));
                });
        }


        auto body = std::make_unique<ast_node_t>(
            std::in_place_type<block_node>, std::move(statements), body_location);

        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<case_node>, get_node(case_stmt[1]), body, new_location);

        log_parse_success(parsed_structure);
        return parse_result(
            std::in_place_type<parse_content>, new_ts, std::move(new_node));
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

    return std::get<1>(std::get<parse_content>(std::move(result)));
}
