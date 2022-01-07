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

#include <algorithm>
#include <memory>
#include <optional>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "ast_node.hpp"
#include "frontend/lexer/token.hpp"
#include "frontend/parser/ast_node.hpp"
#include "frontend/parser/ast_operations/retrieve_source_location.hpp"
#include "token_type.hpp"


//****************************************************************************//
//                                 Private API                                //
//****************************************************************************//
namespace
{
    //****************************************************************************//
    //                                    Types                                   //
    //****************************************************************************//
    template <typename TFunction>
    concept Parser = requires(TFunction& function)
    {
        {
            std::forward<TFunction>(function)()
            } -> std::convertible_to<bool>;
    };

    using parse_result =
        std::optional<std::tuple<std::span<token>, std::unique_ptr<ast_node_t>>>;

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
    struct control_head_parser;
    struct expression_parser;

    template <typename... Parsers>
    struct any;

    template <typename Parsers>
    struct many;

    template <typename... Parsers>
    struct all;

    template <typename SeparatorParser, typename... ItemParsers>
    struct separated;

    template <typename SurrounderParser, typename... InnerParsers>
    struct surrounded;

    template <token_type wanted>
    struct token_parser;

    std::span<token>& get_token_stream(parse_result& result);

    bool try_add_parse_result(parse_result&&             cur_result,
                              std::vector<parse_result>& results,
                              std::span<token>&          ts);

    bool try_add_parse_result(std::vector<parse_result>&& cur_result,
                              std::vector<parse_result>&  results,
                              std::span<token>&           ts);

    //****************************************************************************//
    //                             Parser combinators                             //
    //****************************************************************************//
    namespace combinators
    {
        template <typename... Parsers>
        struct any
        {
            static parse_result parse(std::span<token> ts)
            {
                auto         parsers = std::array{Parsers::parse...};
                parse_result result;

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
        template <typename Parser>
        struct many
        {
            static std::vector<parse_result> parse(std::span<token> ts)
            {
                std::vector<parse_result> results;
                results.reserve(10);
                parse_result cur_result = Parser::parse(ts);

                while (cur_result.has_value())
                {
                    ts = std::get<std::span<token>>(cur_result.value());
                    results.push_back(std::move(cur_result));
                    cur_result = Parser::parse(ts);
                }

                return results;
            }
        };

        template <typename... Parsers>
        struct all
        {
            static std::vector<parse_result> parse(std::span<token> ts)
            {
                std::vector<parse_result> results;
                results.reserve(sizeof...(Parsers));


                bool parsed_all =
                    (try_add_parse_result(Parsers::parse(ts), results, ts) && ...);


                if (parsed_all)
                {
                    return results;
                }
                return {};
            }
        };

        template <typename SeparatorParser, typename... ItemParsers>
        struct separated
        {
            static std::vector<parse_result> parse(std::span<token> ts)
            {
                std::vector<parse_result> results;
                results.reserve((2 * sizeof...(ItemParsers)) - 1);


                bool parsed_all =
                    ((try_add_parse_result(ItemParsers::parse(ts), results, ts)
                      && try_add_parse_result(SeparatorParser::parse(ts), results, ts))
                     && ...);


                if (parsed_all)
                {
                    return results;
                }
                return {};
            }
        };

        template <typename SurrounderParser, typename... InnerParsers>
        struct surrounded
        {
            static std::vector<parse_result> parse(std::span<token> ts)
            {
                std::vector<parse_result> results;
                // 2 stands for the two surrounder parsers
                results.reserve(2 + sizeof...(InnerParsers));


                bool parsed_all =
                    try_add_parse_result(SurrounderParser::parse(ts), results, ts);

                if (!parsed_all)
                {
                    return {};
                }

                parsed_all &=
                    (try_add_parse_result(InnerParsers::parse(ts), results, ts) && ...);

                if (!parsed_all)
                {
                    return {};
                }

                parsed_all &=
                    try_add_parse_result(SurrounderParser::parse(ts), results, ts);

                if (!parsed_all)
                {
                    return {};
                }

                return results;
            }
        };
    }    // namespace combinators


    //****************************************************************************//
    //                              Helper functions                              //
    //****************************************************************************//
    std::span<token>& get_token_stream(parse_result& result)
    {
        return std::get<0>(result.value());
    }

    std::unique_ptr<ast_node_t>& get_node(parse_result& result)
    {
        return std::get<1>(result.value());
    }

    source_location get_source_location_from_compound(std::vector<parse_result>& nodes)
    {
        auto first_node = get_node(nodes.front()).get();
        auto last_node  = get_node(nodes.back()).get();

        auto start_location =
            std::visit(source_location_retriever_visitor{}, *first_node);
        auto end_location = std::visit(source_location_retriever_visitor{}, *last_node);

        return {start_location.line_start,
                start_location.col_start,
                end_location.line_end,
                end_location.col_end};
    }
    bool try_add_parse_result(parse_result&&             cur_result,
                              std::vector<parse_result>& results,
                              std::span<token>&          ts)
    {
        if (cur_result.has_value())
        {
            results.push_back(std::move(cur_result));
            ts = std::get<std::span<token>>(results.back().value());

            return true;
        }
        return false;
    }

    bool try_add_parse_result(std::vector<parse_result>&& cur_result,
                              std::vector<parse_result>&  results,
                              std::span<token>&           ts)
    {
        if (!cur_result.empty())
        {
            std::ranges::move(cur_result, std::back_inserter(results));
            ts = std::get<std::span<token>>(cur_result.back().value());

            return true;
        }
        return false;
    }


    //****************************************************************************//
    //                                   Parsers                                  //
    //****************************************************************************//
    template <token_type wanted>
    struct token_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            if (ts[0].type == wanted)
            {
                // return {ts.subspan(1), ast_node_t{empty_node{}}};
                return {
                    {ts.subspan(1),
                     std::make_unique<ast_node_t>(leaf_node{
                         ts[0].type,
                         ts[0].value,
                         source_location(
                             ts[0].line, ts[0].col, ts[0].line + 1, ts[0].col + 1)})}};
            }
            return {};
        }
    };

    struct program_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            while (!ts.empty())
            {
                if (!combinators::any<procedure_def_parser,
                                      func_def_parser,
                                      var_decl_parser,
                                      var_init_parser>::parse(ts))
                {
                    return {};
                }
            }
            return {};
        };
    };

    struct binary_op_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto bin_op = combinators::all<
                expression_parser,
                combinators::any<token_parser<token_type::PLUS>,
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

            if (bin_op.empty())
            {
                return {};
            }


            auto new_node =
                std::make_unique<ast_node_t>(std::in_place_type<binary_op_node>,
                                             get_node(bin_op[0]),
                                             get_node(bin_op[2]),
                                             get_node(bin_op[1]),
                                             get_source_location_from_compound(bin_op));

            return {{get_token_stream(bin_op.back()), std::move(new_node)}};
        }
    };

    struct unary_op_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto unary_op = combinators::all<
                expression_parser,
                combinators::any<token_parser<token_type::NOT>,
                                 token_parser<token_type::INCREMENT>,
                                 token_parser<token_type::DECREMENT>>>::parse(ts);

            if (unary_op.empty())
            {
                return {};
            }


            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<unary_op_node>,
                get_node(unary_op[0]),
                get_node(unary_op[1]),
                get_source_location_from_compound(unary_op));

            return {{get_token_stream(unary_op.back()), std::move(new_node)}};
        }
    };

    struct func_def_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto func_def = combinators::all<token_parser<token_type::FUNCTION>,
                                             signature_parser,
                                             block_parser>::parse(ts);

            if (func_def.empty())
            {
                return {};
            }


            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<func_def_node>,
                get_node(func_def[0]),
                get_node(func_def[1]),
                get_source_location_from_compound(func_def));

            return {{get_token_stream(func_def.back()), std::move(new_node)}};
        }
    };

    struct procedure_def_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto procedure_def = combinators::all<token_parser<token_type::PROCEDURE>,
                                                  signature_parser,
                                                  block_parser>::parse(ts);

            if (procedure_def.empty())
            {
                return {};
            }


            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<procedure_def_node>,
                get_node(procedure_def[0]),
                get_node(procedure_def[1]),
                get_source_location_from_compound(procedure_def));

            return {{get_token_stream(procedure_def.back()), std::move(new_node)}};
        }
    };

    struct signature_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto signature = combinators::all<token_parser<token_type::IDENTIFIER>,
                                              parameter_def_parser>::parse(ts);

            if (signature.empty())
            {
                return {};
            }


            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<signature_node>,
                std::get<leaf_node>(*get_node(signature[0])).value,
                get_node(signature[1]),
                get_source_location_from_compound(signature));

            return {{get_token_stream(signature.back()), std::move(new_node)}};
        }
    };

    struct return_stmt_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto return_stmt = combinators::all<token_parser<token_type::RETURN>,
                                                expression_parser>::parse(ts);

            if (return_stmt.empty())
            {
                return {};
            }


            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<return_stmt_node>,
                get_node(return_stmt[0]),
                get_source_location_from_compound(return_stmt));

            return {{get_token_stream(return_stmt.back()), std::move(new_node)}};
        }
    };

    struct parameter_def_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto parameter_def = combinators::surrounded<
                token_parser<token_type::LPAREN>,
                token_parser<token_type::RPAREN>,
                combinators::separated<token_parser<token_type::COMMA>,
                                       token_parser<token_type::IDENTIFIER>>>::
                parse(ts);

            if (parameter_def.empty())
            {
                return {};
            }

            auto parameters = std::vector<std::string_view>();
            // Drop surrounder and separators
            parameters.reserve((parameter_def.size() - 2) / 2 + 1);

            for (decltype(parameter_def)::size_type i = 1; i < parameter_def.size();
                 i += 2)
            {
                parameters.push_back(
                    std::move(std::get<leaf_node>(*get_node(parameter_def[i]))).value);
            }

            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<parameter_def_node>,
                std::move(parameters),
                get_source_location_from_compound(parameter_def));

            return {{get_token_stream(parameter_def.back()), std::move(new_node)}};
        }
    };

    struct var_decl_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto var_decl =
                combinators::all<token_parser<token_type::LET>,
                                 token_parser<token_type::IDENTIFIER>,
                                 token_parser<token_type::SEMICOLON>>::parse(ts);

            if (var_decl.empty())
            {
                return {};
            }


            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<var_decl_node>,
                std::get<leaf_node>(*get_node(var_decl[1])).value,
                get_source_location_from_compound(var_decl));

            return {{get_token_stream(var_decl.back()), std::move(new_node)}};
        }
    };

    struct var_init_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto var_init =
                combinators::all<token_parser<token_type::LET>,
                                 token_parser<token_type::IDENTIFIER>,
                                 token_parser<token_type::EQUAL>,
                                 expression_parser,
                                 token_parser<token_type::SEMICOLON>>::parse(ts);

            if (var_init.empty())
            {
                return {};
            }


            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<var_init_node>,
                std::get<leaf_node>(*get_node(var_init[1])).value,
                get_node(var_init[2]),
                get_source_location_from_compound(var_init));

            return {{get_token_stream(var_init.back()), std::move(new_node)}};
        }
    };

    struct var_assignment_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto var_assignment =
                combinators::all<token_parser<token_type::IDENTIFIER>,
                                 token_parser<token_type::EQUAL>,
                                 expression_parser,
                                 token_parser<token_type::SEMICOLON>>::parse(ts);

            if (var_assignment.empty())
            {
                return {};
            }


            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<var_assignment_node>,
                std::get<leaf_node>(*get_node(var_assignment[1])).value,
                get_node(var_assignment[2]),
                get_source_location_from_compound(var_assignment));

            return {{get_token_stream(var_assignment.back()), std::move(new_node)}};
        }
    };

    struct call_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto call = combinators::all<token_parser<token_type::IDENTIFIER>,
                                         parameter_pass_parser>::parse(ts);

            if (call.empty())
            {
                return {};
            }


            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<call_node>,
                std::get<leaf_node>(*get_node(call[0])).value,
                get_node(call[1]),
                get_source_location_from_compound(call));

            return {{get_token_stream(call.back()), std::move(new_node)}};
        }
    };

    struct parameter_pass_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto parameter_pass = combinators::surrounded<
                token_parser<token_type::LPAREN>,
                token_parser<token_type::RPAREN>,
                combinators::separated<token_parser<token_type::COMMA>,
                                       expression_parser>>::parse(ts);

            if (parameter_pass.empty())
            {
                return {};
            }

            auto parameters = std::vector<std::string_view>();
            // Drop surrounder and separators
            parameters.reserve((parameter_pass.size() - 2) / 2 + 1);

            for (decltype(parameter_pass)::size_type i = 1; i < parameter_pass.size();
                 i += 2)
            {
                parameters.push_back(
                    std::move(std::get<leaf_node>(*get_node(parameter_pass[i]))).value);
            }

            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<parameter_pass_node>,
                std::move(parameters),
                get_source_location_from_compound(parameter_pass));

            return {{get_token_stream(parameter_pass.back()), std::move(new_node)}};
        }
    };

    struct block_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto block =
                combinators::many<combinators::any<var_decl_parser,
                                                   var_assignment_parser,
                                                   var_init_parser,
                                                   expression_parser,
                                                   control_block_parser>>::parse(ts);

            if (block.empty())
            {
                return {};
            }


            std::vector<std::unique_ptr<ast_node_t>> statements;
            statements.reserve(block.size());

            std::ranges::for_each(block, [&statements](auto&& element) {
                statements.push_back(
                    std::move(get_node(std::forward<decltype(element)>(element))));
            });

            auto new_node =
                std::make_unique<ast_node_t>(std::in_place_type<block_node>,
                                             std::move(statements),
                                             get_source_location_from_compound(block));

            return {{get_token_stream(block.back()), std::move(new_node)}};
        }
    };

    struct control_block_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto control_block =
                combinators::all<control_block_parser, block_parser>::parse(ts);

            if (control_block.empty())
            {
                return {};
            }

            auto new_node = std::make_unique<ast_node_t>(
                std::in_place_type<control_block_node>,
                get_node(control_block[0]),
                get_node(control_block[1]),
                get_source_location_from_compound(control_block));

            return {{get_token_stream(control_block.back()), std::move(new_node)}};
        }
    };

    struct control_head_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            {
                auto control_head =
                    combinators::surrounded<token_parser<token_type::LPAREN>,
                                            token_parser<token_type::RPAREN>,
                                            expression_parser>::parse(ts);

                if (control_head.empty())
                {
                    return {};
                }

                auto new_node = std::make_unique<ast_node_t>(
                    std::in_place_type<control_head_node>,
                    get_node(control_head[1]),
                    get_source_location_from_compound(control_head));

                return {{get_token_stream(control_head.back()), std::move(new_node)}};
            }
        }
    };

    struct expression_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            auto expression =
                combinators::any<binary_op_parser,
                                 unary_op_parser,
                                 call_parser,
                                 token_parser<token_type::LITERAL>,
                                 token_parser<token_type::IDENTIFIER>>::parse(ts);

            if (!expression.has_value())
            {
                return {};
            }

            return {{get_token_stream(expression),
                     std::move(std::get<1>(expression.value()))}};
        }
    };
}    // namespace
//****************************************************************************//
//                                 Public API                                 //
//****************************************************************************//
inline std::unique_ptr<ast_node_t> parse(std::span<token> ts)
{
    return {std::get<1>(program_parser::parse(ts).value())};
}
#endif    // SRC_FRONTEND_PARSER_PARSER_HPP_
