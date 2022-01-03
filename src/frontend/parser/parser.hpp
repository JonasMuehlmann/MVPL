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


template <typename TFunction>
concept nullary_predicate = requires(TFunction& function)
{
    {
        std::forward<TFunction>(function)()
        } -> std::convertible_to<bool>;
};

class parser
{
 private:
    std::span<token>            token_stream_;
    std::unique_ptr<ast_node_t> ast;


 public:
    explicit parser(std::span<token> token_stream_);
    std::unique_ptr<ast_node_t> parse();
};

nullary_predicate auto parse_program(const std::span<token> ts);

nullary_predicate auto parse_binary_op(const std::span<token> ts);

nullary_predicate auto parse_unary_op(const std::span<token> ts);

nullary_predicate auto parse_func_def(const std::span<token> ts);

nullary_predicate auto parse_procedure_def(const std::span<token> ts);

nullary_predicate auto parse_signature(const std::span<token> ts);

nullary_predicate auto parse_return_stmt(const std::span<token> ts);

nullary_predicate auto parse_parameter_def(const std::span<token> ts);

nullary_predicate auto parse_var_decl(const std::span<token> ts);

nullary_predicate auto parse_var_init(const std::span<token> ts);

nullary_predicate auto parse_var_assignment(const std::span<token> ts);

nullary_predicate auto parse_call(const std::span<token> ts);

nullary_predicate auto parse_parameter_pass(const std::span<token> ts);

nullary_predicate auto parse_block(const std::span<token> ts);

nullary_predicate auto parse_control_block(const std::span<token> ts);

nullary_predicate auto parse_control_head(const std::span<token> ts);

nullary_predicate auto parse_expression(const std::span<token> ts);

nullary_predicate auto parse_any(nullary_predicate auto&&... parser);
nullary_predicate auto parse_all(nullary_predicate auto&&... parser);

nullary_predicate auto parse_separated(nullary_predicate auto&& separator_parser,
                                       nullary_predicate auto&&... item_parser);

nullary_predicate auto parse_surrounded(nullary_predicate auto&& surrounder_parser,
                                        nullary_predicate auto&&... inner_parser);

nullary_predicate auto parse_token(const std::span<token> ts, const token_type wanted);

#endif    // SRC_FRONTEND_PARSER_PARSER_HPP_
