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
#include "ast_node.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "ast_node_type.hpp"
#include "source_location.hpp"
#include "util.hpp"

ast_node::ast_node(const ast_node_type type, source_location location) :
    type{type}, source_location_{location}
{}


leaf_node::leaf_node(token_type       token,
                     std::string_view value,
                     source_location  location) :
    ast_node(ast_node_type::LEAF, location), token(token), value(value)
{}

leaf_node::leaf_node(struct token token_) :
    ast_node(ast_node_type::LEAF, token_.source_location_),
    token(token_.type),
    value(token_.value)
{}

program_node::program_node(std::vector<std::unique_ptr<ast_node_t>>&& globals,
                           source_location                            location) :
    ast_node(ast_node_type::PROGRAM, location), globals{std::move(globals)}
{}


binary_op_node::binary_op_node(std::unique_ptr<ast_node_t>& lhs_,
                               std::unique_ptr<ast_node_t>& rhs_,
                               std::unique_ptr<ast_node_t>& operator_,
                               source_location              location) :
    ast_node(ast_node_type::BINARY_OP, location),
    lhs{std::move(lhs_)},
    rhs{std::move(rhs_)},
    operator_{std::move(operator_)}
{}

unary_op_node::unary_op_node(std::unique_ptr<ast_node_t>& operand,
                             std::unique_ptr<ast_node_t>& operator_,
                             source_location              location) :
    ast_node(ast_node_type::UNARY_OP, location),
    operand{std::move(operand)},
    operator_{std::move(operator_)}
{}

func_def_node::func_def_node(std::unique_ptr<ast_node_t>& signature,
                             std::unique_ptr<ast_node_t>& body,
                             source_location              location) :
    ast_node(ast_node_type::FUNC_DEF, location),
    signature{std::move(signature)},
    body{std::move(body)}
{}

procedure_def_node::procedure_def_node(std::unique_ptr<ast_node_t>& signature,
                                       std::unique_ptr<ast_node_t>& body,
                                       source_location              location) :
    ast_node(ast_node_type::PROCEDURE_DEF, location),
    signature{std::move(signature)},
    body{std::move(body)}
{}

signature_node::signature_node(std::string_view             identifier,
                               std::unique_ptr<ast_node_t>& parameter_list,
                               source_location              location) :

    ast_node(ast_node_type::FUNC_SIGNATURE, location),
    identifier{identifier},
    parameter_list{std::move(parameter_list)}
{}
return_stmt_node::return_stmt_node(std::unique_ptr<ast_node_t>& value,
                                   source_location              location) :

    ast_node(ast_node_type::RETURN_STMT, location), value{std::move(value)}
{}

parameter_def_node::parameter_def_node(std::vector<std::string_view>&& parameter_list,
                                       source_location                 location) :

    ast_node(ast_node_type::PARAMETER_DEF, location),
    parameter_list{std::move(parameter_list)}
{}

var_decl_node::var_decl_node(std::string_view identifier, source_location location) :

    ast_node(ast_node_type::VAR_DECL, location), identifier{identifier}
{}

var_init_node::var_init_node(std::string_view             identifier,
                             std::unique_ptr<ast_node_t>& value,
                             source_location              location) :

    ast_node(ast_node_type::VAR_INIT, location),
    identifier{identifier},
    value{std::move(value)}
{}

var_assignment_node::var_assignment_node(std::string_view             identifier,
                                         std::unique_ptr<ast_node_t>& value,
                                         source_location              location) :

    ast_node(ast_node_type::VAR_ASSIGNMENT, location),
    identifier{identifier},
    value{std::move(value)}
{}

call_node::call_node(std::string_view             identifier,
                     std::unique_ptr<ast_node_t>& parameter_pass,
                     source_location              location) :

    ast_node(ast_node_type::CALL, location),
    identifier{identifier},
    parameter_pass{std::move(parameter_pass)}
{}

parameter_pass_node::parameter_pass_node(std::vector<std::string_view>&& parameter_list,
                                         source_location                 location) :

    ast_node(ast_node_type::PARAMETER_PASS, location),
    parameter_list{std::move(parameter_list)}
{}

block_node::block_node(std::vector<std::unique_ptr<ast_node_t>>&& statements,
                       source_location                            location) :

    ast_node(ast_node_type::BLOCK, location), statements{std::move(statements)}
{}


if_stmt_node::if_stmt_node(std::unique_ptr<ast_node_t>& condition,
                           std::unique_ptr<ast_node_t>& body,
                           source_location              location) :
    ast_node(ast_node_type::IF, location),
    condition{std::move(condition)},
    body{std::move(body)}
{}


else_if_stmt_node::else_if_stmt_node(std::unique_ptr<ast_node_t>& condition,
                                     std::unique_ptr<ast_node_t>& body,
                                     source_location              location) :
    ast_node(ast_node_type::ELSE_IF, location),
    condition{std::move(condition)},
    body{std::move(body)}
{}


else_stmt_node::else_stmt_node(std::unique_ptr<ast_node_t>& body,
                               source_location              location) :
    ast_node(ast_node_type::ELSE, location), body{std::move(body)}
{}

for_loop_node::for_loop_node(std::unique_ptr<ast_node_t>& init_stmt,
                             std::unique_ptr<ast_node_t>& test_expression,
                             std::unique_ptr<ast_node_t>& update_expression,
                             std::unique_ptr<ast_node_t>& body,
                             source_location              location) :
    ast_node(ast_node_type::FOR, location),
    init_stmt{std::move(init_stmt)},
    test_expression{std::move(test_expression)},
    update_expression{std::move(update_expression)},
    body{std::move(body)}
{}

while_loop_node::while_loop_node(std::unique_ptr<ast_node_t>& condition,
                                 std::unique_ptr<ast_node_t>& body,
                                 source_location              location) :
    ast_node(ast_node_type::WHILE, location),
    condition{std::move(condition)},
    body{std::move(body)}
{}

switch_node::switch_node(std::unique_ptr<ast_node_t>& expression,
                         std::unique_ptr<ast_node_t>& body,
                         source_location              location) :
    ast_node(ast_node_type::SWITCH, location),
    expression{std::move(expression)},
    body{std::move(body)}
{}

case_node::case_node(std::unique_ptr<ast_node_t>& value,
                     std::unique_ptr<ast_node_t>& body,
                     source_location              location) :
    ast_node(ast_node_type::CASE, location),
    value{std::move(value)},
    body{std::move(body)}
{}
