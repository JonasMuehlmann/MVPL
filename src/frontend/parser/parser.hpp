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
#include "nlohmann/json.hpp"
#include "token_type.hpp"


//****************************************************************************//
//                                 Private API                                //
//****************************************************************************//

using namespace std::literals::string_view_literals;
using json = nlohmann::ordered_json;

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

struct parse_error
{
    std::string parsed_structure;
    token       token_;

    explicit parse_error(std::string& parsed_structure) :
        parsed_structure(parsed_structure),
        token_(token_type::END_TOKEN, ""sv, source_location())
    {}

    parse_error(std::string& parsed_structure, token token_) :
        parsed_structure(parsed_structure), token_(token_)
    {}

    void throw_()
    {
        json j;
        to_json(j, token_);

        throw std::runtime_error("Could not parse " + parsed_structure + " at token "
                                 + j.dump(4));
    }
};

using parse_content = std::tuple<std::span<token>, std::unique_ptr<ast_node_t>>;
using parse_result  = std::variant<parse_content, parse_error>;

template <typename Function>
using ReturnTypeOfFunction = typename decltype(std::function{
    std::declval<Function>()})::result_type;

template <typename ReturnType, typename... Pack>
concept PackContainsFunctionWithReturnType =
    ((std::is_same<ReturnTypeOfFunction<Pack>, ReturnType>::value) || ...);

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
struct control_head_parser;
struct expression_parser;

namespace combinators
{
template <typename Parser>
struct expect;

template <typename Expector, typename Parser>
struct if_expect;

template <typename Parser>
struct optional;

template <typename... Parsers>
struct any;

template <typename Parser>
struct many;

template <typename... Parsers>
struct all;

template <typename SeparatorParser, typename ItemParser>
struct separated;

template <typename OpeningParser, typename ClosingParser, typename... InnerParsers>
struct surrounded;
}    // namespace combinators

template <token_type wanted>
struct token_parser;

std::span<token>& get_token_stream(parse_result& result);

bool try_add_parse_result(parse_result&&             cur_result,
                          std::vector<parse_result>& results,
                          std::span<token>&          ts,
                          bool                       overwrite_errors = false);

bool try_add_parse_result(parse_result&&    cur_result,
                          parse_result&     result,
                          std::span<token>& ts,
                          bool              overwrite_errors = false);

bool try_add_parse_result(std::vector<parse_result>&& cur_result,
                          std::vector<parse_result>&  results,
                          std::span<token>&           ts,
                          bool                        overwrite_errors = false);

bool is_any_parse_result_valid(std::vector<parse_result>& results)
{
    return std::ranges::any_of(results, [](auto& parse_result_) {
        return std::holds_alternative<parse_content>(parse_result_);
    });
}

bool are_all_parse_results_valid(std::vector<parse_result>& results)
{
    return std::ranges::all_of(results, [](auto& parse_result_) {
        return std::holds_alternative<parse_content>(parse_result_);
    });
}
//****************************************************************************//
//                             Parser combinators                             //
//****************************************************************************//

namespace combinators
{
template <typename Parser>
struct optional
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results{};


        // FIX: We are being passed a parse_error from combinators::any here,
        //  even though one alternative was matched!
        if (!ts.empty() && try_add_parse_result(Parser::parse(ts), results, ts))
        {
            return results;
        }

        parse_result new_node = parse_content{
            ts,
            std::make_unique<ast_node_t>(std::in_place_type<missing_optional_node>)};

        results.clear();
        results.push_back(std::move(new_node));

        return results;
    }
};


template <typename... Parsers>
struct any
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results;

        (try_add_parse_result(Parsers::parse(ts), results, ts, true) || ...);

        return results;
    }
};

template <typename Parser>
struct many
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results;

        results.reserve(10);
        if (!try_add_parse_result(Parser::parse(ts), results, ts))
        {
            return results;
        }


        while (!ts.empty() && try_add_parse_result(Parser::parse(ts), results, ts))
        {}

        // When the loop stops, it always adds a parse error!
        results.erase(results.end()); 
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


        (try_add_parse_result(Parsers::parse(ts), results, ts) && ...);


        return results;
    }
};

template <typename SeparatorParser, typename ItemParser>
struct separated
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results;
        results.reserve(10);


        if (!try_add_parse_result(
                combinators::many<combinators::all<ItemParser, SeparatorParser>>::parse(
                    ts),
                results,
                ts))
        {
            return results;
        }

        // NOTE: The previous block always fails to parse the last item,
        // because it expects trailing commas, hence we parse the last item manually
        try_add_parse_result(ItemParser::parse(ts), results, ts);

        return results;
    }
};

template <typename OpeningParser, typename ClosingParser, typename... InnerParsers>
struct surrounded
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results;
        // 2 stands for the two surrounder parsers
        results.reserve(2 + sizeof...(InnerParsers));


        if (!try_add_parse_result(OpeningParser::parse(ts), results, ts))
        {
            return results;
        }


        if (!(try_add_parse_result(InnerParsers::parse(ts), results, ts) && ...))
        {
            return results;
        }

        try_add_parse_result(ClosingParser::parse(ts), results, ts);

        return results;
    }
};
};    // namespace combinators


//****************************************************************************//
//                              Helper functions                              //
//****************************************************************************//
std::span<token>& get_token_stream(parse_result& result)
{
    return std::get<0>(std::get<parse_content>(result));
}

std::unique_ptr<ast_node_t>& get_node(parse_result& result)
{
    return std::get<1>(std::get<parse_content>(result));
}

source_location get_source_location_from_compound(std::vector<parse_result>& nodes)
{
    auto first_node = get_node(nodes.front()).get();
    auto last_node  = get_node(nodes.back()).get();

    auto start_location = std::visit(source_location_retriever_visitor{}, *first_node);
    auto end_location   = std::visit(source_location_retriever_visitor{}, *last_node);

    return {start_location.line_start,
            start_location.col_start,
            end_location.line_end,
            end_location.col_end};
}

bool try_add_parse_result(parse_result&&             cur_result,
                          std::vector<parse_result>& results,
                          std::span<token>&          ts,
                          bool                       overwrite_errors)
{
    if (std::holds_alternative<parse_error>(cur_result))
    {
        if (std::ranges::none_of(results, [](auto& result) {
                return std::holds_alternative<parse_error>(result);
            }))
        {
            results.push_back(std::move(cur_result));
        }

        return false;
    }


    if (overwrite_errors)
    {
        std::erase_if(results, [](auto& cur_result) {
          return std::holds_alternative<parse_error>(cur_result);
        });
    }
    
    results.push_back(std::move(cur_result));

    ts = get_token_stream(results.back());

    return true;
}

bool try_add_parse_result(parse_result&&    cur_result,
                          parse_result&     result,
                          std::span<token>& ts,
                          bool              overwrite_errors)
{
    result = std::move(cur_result);

    if (std::holds_alternative<parse_content>(result))
    {
        ts = get_token_stream(result);

        return true;
    }
    return false;
}

bool try_add_parse_result(std::vector<parse_result>&& cur_results,
                          std::vector<parse_result>&  results,
                          std::span<token>&           ts,
                          bool                        overwrite_errors)
{
    if (std::ranges::all_of(cur_results, [](auto& cur_result) {
            return std::holds_alternative<parse_content>(cur_result);
        }))
    {
        std::ranges::move(cur_results, std::back_inserter(results));

        if (overwrite_errors)
        { 
            std::erase_if(results, [](auto& cur_result) {
                return std::holds_alternative<parse_error>(cur_result);
            });
        }
        ts = get_token_stream(results.back());

        return true;
    }

    if (std::ranges::none_of(results, [](auto& cur_result) {
            return std::holds_alternative<parse_error>(cur_result);
        }))
    {
        results.push_back(
            std::move(*std::ranges::find_if(cur_results, [](auto& cur_result) {
                return std::holds_alternative<parse_error>(cur_result);
            })));
    }

    return false;
}


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


        if (!is_any_parse_result_valid(program))
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
                                         get_node(unary_op[0]),
                                         get_node(unary_op[1]),
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
            get_node(procedure_def[0]),
            get_node(procedure_def[1]),
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

        auto return_stmt = combinators::all<token_parser<token_type::RETURN>,
                                            expression_parser>::parse(ts);

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

        if (!is_any_parse_result_valid(parameter_def))
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

        if (!is_any_parse_result_valid(parameter_pass))
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
        auto block = combinators::all<token_parser<token_type::LBRACAE>,
                                      combinators::optional<combinators::many<
                                          combinators::any<var_assignment_parser,
                                                           var_init_parser,
                                                           var_decl_parser,
                                                           expression_parser,
                                                           control_block_parser,
                                                           call_parser,
                                                           return_stmt_parser>>>,
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
            std::ranges::for_each(block, [&statements](auto&& element) {
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

        auto control_block =
            combinators::all<control_head_parser, block_parser>::parse(ts);

        if (!are_all_parse_results_valid(control_block))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<control_block_node>,
            get_node(control_block[0]),
            get_node(control_block[1]),
            get_source_location_from_compound(control_block));

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(control_block.back()),
                            std::move(new_node));
    }
};

struct control_head_parser
{
    static inline std::string parsed_structure = "";

    static parse_result parse(std::span<token> ts)
    {
        if (ts.empty())
        {
            return parse_error(parsed_structure);
        }

        auto control_head = combinators::surrounded<token_parser<token_type::LPAREN>,
                                                    token_parser<token_type::RPAREN>,
                                                    expression_parser>::parse(ts);

        if (!is_any_parse_result_valid(control_head))
        {
            return parse_error(parsed_structure, ts[0]);
        }

        auto new_node = std::make_unique<ast_node_t>(
            std::in_place_type<control_head_node>,
            get_node(control_head[1]),
            get_source_location_from_compound(control_head));

        return parse_result(std::in_place_type<parse_content>,
                            get_token_stream(control_head.back()),
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
#endif    // SRC_FRONTEND_PARSER_PARSER_HPP_
