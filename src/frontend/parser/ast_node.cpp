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

#include <memory>

#include "ast_node_type.hpp"
#include "source_location.hpp"

ast_node::ast_node(const ast_node_type type, const source_location location) :
    type{type}, source_location_{location}
{}

program_node::program_node(std::vector<std::unique_ptr<ast_node>>& globals,
                           const source_location                   location) :
    ast_node(ast_node_type::PROGRAM, location), globals{globals}
{}

binary_op_node::binary_op_node(const ast_node&       lhs_,
                               const ast_node&       rhs_,
                               const source_location location) :
    ast_node(ast_node_type::BINARY_OP, location),
    lhs{std::make_unique<ast_node>(lhs_)},
    rhs{std::make_unique<ast_node>(rhs_)}
{}

unary_op_node::unary_op_node(const ast_node& operand, const source_location location) :
    ast_node(ast_node_type::UNARY_OP, location),
    operand{std::make_unique<ast_node>(operand)}
{}

func_def_node::func_def_node(const ast_node&       signature,
                             const ast_node&       body,
                             const source_location location) :
    ast_node(ast_node_type::FUNC_DEF, location),
    signature{std::make_unique<ast_node>(signature)},
    body{std::make_unique<ast_node>(body)}
{}

procedure_def_node::procedure_def_node(const ast_node&       signature,
                                       const ast_node&       body,
                                       const source_location location) :
    ast_node(ast_node_type::PROCEDURE_DEF, location),
    signature{std::make_unique<ast_node>(signature)},
    body{std::make_unique<ast_node>(body)}
{}

signature_node::signature_node(const std::string&    identifier,
                               const ast_node&       parameter_list,
                               const source_location location) :

    ast_node(ast_node_type::FUNC_SIGNATURE, location),
    identifier{std::make_unique<ast_node>(identifier)},
    parameter_list{std::make_unique<ast_node>(parameter_list)}
{}
return_stmt_node::return_stmt_node(const ast_node&       value,
                                   const source_location location) :

    ast_node(ast_node_type::RETURN_STMT, location),
    value{std::make_unique<ast_node>(value)}
{}

parameter_def_node::parameter_def_node(const ast_node&       parameter_list,
                                       const source_location location) :

    ast_node(ast_node_type::PARAMETER_DEF, location),
    parameter_list{std::make_unique<ast_node>(parameter_list)}
{}

var_decl_node::var_decl_node(const std::string&    identifier,
                             const source_location location) :

    ast_node(ast_node_type::VAR_DEF, location),
    identifier{std::make_unique<ast_node>(identifier)}
{}

var_init_node::var_init_node(const std::string&    identifier,
                             const ast_node&       value,
                             const source_location location) :

    ast_node(ast_node_type::VAR_INIT, location),
    identifier{std::make_unique<ast_node>(identifier)},
    value{std::make_unique<ast_node>(value)}
{}

var_assignment_node::var_assignment_node(const std::string&    identifier,
                                         const ast_node&       value,
                                         const source_location location) :

    ast_node(ast_node_type::VAR_ASSIGNMENT, location),
    identifier{identifier},
    value{std::make_unique<ast_node>(value)}
{}

call_node::call_node(const std::string&    identifier,
                     const ast_node&       parameter_pass,
                     const source_location location) :

    ast_node(ast_node_type::CALL, location),
    identifier{identifier},
    parameter_pass{std::make_unique<ast_node>(parameter_pass)}
{}

parameter_pass_node::parameter_pass_node(const ast_node&       parameter_list,
                                         const source_location location) :

    ast_node(ast_node_type::PARAMETER_PASS, location),
    parameter_list{std::make_unique<ast_node>(parameter_list)}
{}

block_node::block_node(const ast_node& statements, const source_location location) :

    ast_node(ast_node_type::BLOCK, location),
    statements{std::make_unique<ast_node>(statements)}
{}

control_block_node::control_block_node(const ast_node&       head,
                                       const ast_node&       body,
                                       const source_location location) :

    ast_node(ast_node_type::CONTROL_BLOCK, location),
    head{std::make_unique<ast_node>(head)},
    body{std::make_unique<ast_node>(body)}
{}

control_head_node::control_head_node(const ast_node&       expression,
                                     const source_location location) :

    ast_node(ast_node_type::CONTROL_HEAD, location),
    expression{std::make_unique<ast_node>(expression)}
{}
