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
                parse_result              cur_result = Parser::parse(ts);

                while (cur_result.has_value())
                {
                    ts = std::get<std::span<token>>(cur_result.value());
                    results.push_back(cur_result);
                    cur_result = Parser::parse(ts);
                }

                return results;
                // return (Parsers::parse(ts) || ...);
            }
        };

        template <typename... Parsers>
        struct all
        {
            static std::vector<parse_result> parse(std::span<token> ts)
            {
                auto                      parsers = std::array{Parsers::parse...};
                std::vector<parse_result> results;
                parse_result              cur_result;

                for (auto parser : parsers)
                {
                    cur_result = parser(ts);

                    if (!cur_result)
                    {
                        return {};
                    }

                    results.push_back(cur_result);
                    ts = std::get<std::span<token>>(cur_result.value());
                }
                return results;
                // return (Parsers::parse(ts) && ...);
            }
        };

        template <typename SeparatorParser, typename... ItemParsers>
        struct separated
        {
            static parse_result parse(std::span<token> ts)
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
        struct surrounded
        {
            static parse_result parse(std::span<token> ts)
            {
                return combinators::all<SurrounderParser,
                                        combinators::all<InnerParsers...>,
                                        SurrounderParser>::parse(ts);
            }
        };
    }    // namespace combinators

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
                return {{ts.subspan(1), std::make_unique<ast_node_t>(empty_node{})}};
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
            return combinators::all<
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
        }
    };

    struct unary_op_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::all<
                expression_parser,
                combinators::any<token_parser<token_type::NOT>,
                                 token_parser<token_type::INCREMENT>,
                                 token_parser<token_type::DECREMENT>>>::parse(ts);
        }
    };

    struct func_def_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::all<token_parser<token_type::FUNCTION>,
                                    signature_parser,
                                    block_parser>::parse(ts);
        }
    };

    struct procedure_def_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::all<token_parser<token_type::PROCEDURE>,
                                    signature_parser,
                                    block_parser>::parse(ts);
        }
    };

    struct signature_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::all<token_parser<token_type::IDENTIFIER>,
                                    parameter_def_parser>::parse(ts);
        }
    };

    struct return_stmt_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::all<token_parser<token_type::RETURN>,
                                    expression_parser>::parse(ts);
        }
    };

    struct parameter_def_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::surrounded<
                token_parser<token_type::LPAREN>,
                token_parser<token_type::RPAREN>,
                combinators::separated<token_parser<token_type::COMMA>,
                                       token_parser<token_type::IDENTIFIER>>>::
                parse(ts);
        }
    };

    struct var_decl_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::all<token_parser<token_type::LET>,
                                    token_parser<token_type::IDENTIFIER>,
                                    token_parser<token_type::SEMICOLON>>::parse(ts);
        }
    };

    struct var_init_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::all<token_parser<token_type::LET>,
                                    token_parser<token_type::IDENTIFIER>,
                                    token_parser<token_type::EQUAL>,
                                    expression_parser,
                                    token_parser<token_type::SEMICOLON>>::parse(ts);
        }
    };

    struct var_assignment_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::all<token_parser<token_type::IDENTIFIER>,
                                    token_parser<token_type::EQUAL>,
                                    expression_parser,
                                    token_parser<token_type::SEMICOLON>>::parse(ts);
        }
    };

    struct call_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::all<token_parser<token_type::IDENTIFIER>,
                                    parameter_pass_parser>::parse(ts);
        }
    };

    struct parameter_pass_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::surrounded<
                token_parser<token_type::LPAREN>,
                token_parser<token_type::RPAREN>,
                combinators::separated<token_parser<token_type::COMMA>,
                                       token_parser<token_type::IDENTIFIER>>>::
                parse(ts);
        }
    };

    struct block_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::any<combinators::any<var_decl_parser,
                                                     var_assignment_parser,
                                                     var_init_parser,
                                                     expression_parser,
                                                     control_block_parser>>::parse(ts);
        }
    };

    struct control_block_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::all<control_block_parser, block_parser>::parse(ts);
        }
    };

    struct control_head_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            {
                return combinators::surrounded<token_parser<token_type::LPAREN>,
                                               token_parser<token_type::RPAREN>,
                                               expression_parser>::parse(ts);
            }
        }
    };

    struct expression_parser
    {
        static parse_result parse(std::span<token> ts)
        {
            return combinators::any<binary_op_parser,
                                    unary_op_parser,
                                    call_parser,
                                    token_parser<token_type::LITERAL>,
                                    token_parser<token_type::IDENTIFIER>>::parse(ts);
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
