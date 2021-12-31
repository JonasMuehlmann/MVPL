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


program_node::program_node(std::vector<std::unique_ptr<ast_node_t>>& globals,
                           source_location                           location) :
    ast_node(ast_node_type::PROGRAM, location), globals{globals}
{}


binary_op_node::binary_op_node(std::unique_ptr<ast_node_t>& lhs_,
                               std::unique_ptr<ast_node_t>& rhs_,
                               operator_type                operator_,
                               source_location              location) :
    ast_node(ast_node_type::BINARY_OP, location),
    lhs{std::move(lhs_)},
    rhs{std::move(rhs_)},
    operator_{operator_}
{}

unary_op_node::unary_op_node(std::unique_ptr<ast_node_t>& operand,
                             operator_type                operator_,
                             source_location              location) :
    ast_node(ast_node_type::UNARY_OP, location),
    operand{std::move(operand)},
    operator_{operator_}
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

signature_node::signature_node(std::string&                 identifier,
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

parameter_def_node::parameter_def_node(std::unique_ptr<ast_node_t>& parameter_list,
                                       source_location              location) :

    ast_node(ast_node_type::PARAMETER_DEF, location),
    parameter_list{std::move(parameter_list)}
{}

var_decl_node::var_decl_node(std::string& identifier, source_location location) :

    ast_node(ast_node_type::VAR_DEF, location), identifier{identifier}
{}

var_init_node::var_init_node(std::string&                 identifier,
                             std::unique_ptr<ast_node_t>& value,
                             source_location              location) :

    ast_node(ast_node_type::VAR_INIT, location),
    identifier{identifier},
    value{std::move(value)}
{}

var_assignment_node::var_assignment_node(std::string&                 identifier,
                                         std::unique_ptr<ast_node_t>& value,
                                         source_location              location) :

    ast_node(ast_node_type::VAR_ASSIGNMENT, location),
    identifier{identifier},
    value{std::move(value)}
{}

call_node::call_node(std::string&                 identifier,
                     std::unique_ptr<ast_node_t>& parameter_pass,
                     source_location              location) :

    ast_node(ast_node_type::CALL, location),
    identifier{identifier},
    parameter_pass{std::move(parameter_pass)}
{}

parameter_pass_node::parameter_pass_node(std::unique_ptr<ast_node_t>& parameter_list,
                                         source_location              location) :

    ast_node(ast_node_type::PARAMETER_PASS, location),
    parameter_list{std::move(parameter_list)}
{}

block_node::block_node(std::vector<std::unique_ptr<ast_node_t>>& statements,
                       source_location                           location) :

    ast_node(ast_node_type::BLOCK, location), statements{std::move(statements)}
{}

control_block_node::control_block_node(std::unique_ptr<ast_node_t>& head,
                                       std::unique_ptr<ast_node_t>& body,
                                       source_location              location) :

    ast_node(ast_node_type::CONTROL_BLOCK, location),
    head{std::move(head)},
    body{std::move(body)}
{}

control_head_node::control_head_node(std::unique_ptr<ast_node_t>& expression,
                                     source_location              location) :

    ast_node(ast_node_type::CONTROL_HEAD, location), expression{std::move(expression)}
{}
