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
#include <span>
#include <type_traits>
#include <utility>

#include "ast_node.hpp"
#include "frontend/lexer/token.hpp"
#include "token_type.hpp"


template <typename R, typename C, typename... Args>
C function_pointer_class(R (C::*)(Args...));

template <typename TFunction>
concept nullary_predicate_function = requires(TFunction& function)
{
    {
        std::forward<TFunction>(function)()
        } -> std::convertible_to<bool>;
};

template <typename TFunction>
concept nullary_predicate_member = requires(TFunction& function)
{
    {
        std::invoke(function,
                    std::declval<decltype(function_pointer_class(function))>())
        } -> std::convertible_to<bool>;
};

template <typename TFunction>
concept nullary_predicate =
    nullary_predicate_member<TFunction> || nullary_predicate_function<TFunction>;

// Recursive descent parser
class parser
{
 private:
    // Members
    std::span<token>            token_stream_;
    std::unique_ptr<ast_node_t> ast;
    // Methods
    bool parse_program();
    bool parse_binary_op();
    bool parse_unary_op();
    bool parse_func_def();
    bool parse_procedure_def();
    bool parse_signature();
    bool parse_return_stmt();
    bool parse_parameter_def();
    bool parse_var_decl();
    bool parse_var_init();
    bool parse_var_assignment();
    bool parse_call();
    bool parse_parameter_pass();
    bool parse_block();
    bool parse_control_block();
    bool parse_control_head();

    bool parse_any(nullary_predicate auto&&... parser);
    bool parse_all(nullary_predicate auto&&... parser);

    bool parse_separated(nullary_predicate auto&& separator_parser,
                         nullary_predicate auto&&... item_parser);

    bool parse_surrounded(nullary_predicate auto&& surrounder_parser,
                          nullary_predicate auto&&... inner_parser);

    struct parse_token
    {
        token_type wanted_type;

        explicit parse_token(token_type wanted_type);

        bool operator()();
    };

    parse_token make_parse_token(token_type wanted_type);

    bool parser_invoker(nullary_predicate_function auto&& parser_)
    {
        return std::invoke(parser_);
    }
    bool parser_invoker(nullary_predicate_member auto&& parser_)
    {
        return std::invoke(parser_, this);
    };

 public:
    explicit parser(std::span<token> token_stream_);
    std::unique_ptr<ast_node_t> parse();
};
#endif    // SRC_FRONTEND_PARSER_PARSER_HPP_
