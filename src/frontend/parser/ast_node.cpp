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

ast_node::ast_node(const ast_node_type type, const source_location& location) :
    type{type}, source_location_{location}
{}

void ast_node::print(int nesting) const
{
    if (nesting > 1)
    {
        std::cout << std::string(4LL * nesting, ' ');
    }
    nesting++;
    std::cout << "{\n"
              << std::string(4LL * nesting, ' ') << R"("type": ")"
              << LUT_AST_NODE_TYPE_TO_STRING[static_cast<size_t>(type)] << ",\n"
              << std::string(4LL * nesting, ' ') << R"("source_location": )";
    print_source_location(source_location_, nesting);

    std::cout << ",\n" << std::string(4LL * nesting, ' ');
}

program_node::program_node(std::vector<std::unique_ptr<ast_node>>& globals,
                           const source_location&                  location) :
    ast_node(ast_node_type::PROGRAM, location), globals{globals}
{}

void program_node::print(int nesting) const
{
    ast_node::print(nesting);

    std::cout << R"("globals": [)";

    nesting++;

    for (const auto& global : globals)
    {
        if (&global == &globals.front())
        {
            std::cout << '\n';
        }

        global->print(nesting + 1);


        if (&global != &globals.back())
        {
            std::cout << ',';
        }
        std::cout << '\n';
    }

    if (!globals.empty())
    {
        std::cout << std::string(4LL * nesting, ' ');
    }
    std::cout << ']';
    nesting--;

    std::cout << "\n";
    std::cout << std::string(4LL * nesting, ' ');
    std::cout << '}';
}

binary_op_node::binary_op_node(std::unique_ptr<ast_node>& lhs_,
                               std::unique_ptr<ast_node>& rhs_,
                               operator_type              operator_,
                               const source_location&     location) :
    ast_node(ast_node_type::BINARY_OP, location),
    lhs{std::move(lhs_)},
    rhs{std::move(rhs_)},
    operator_{operator_}
{}

unary_op_node::unary_op_node(std::unique_ptr<ast_node>& operand,
                             operator_type              operator_,
                             const source_location&     location) :
    ast_node(ast_node_type::UNARY_OP, location),
    operand{std::move(operand)},
    operator_{operator_}
{}

func_def_node::func_def_node(std::unique_ptr<ast_node>& signature,
                             std::unique_ptr<ast_node>& body,
                             const source_location&     location) :
    ast_node(ast_node_type::FUNC_DEF, location),
    signature{std::move(signature)},
    body{std::move(body)}
{}

procedure_def_node::procedure_def_node(std::unique_ptr<ast_node>& signature,
                                       std::unique_ptr<ast_node>& body,
                                       const source_location&     location) :
    ast_node(ast_node_type::PROCEDURE_DEF, location),
    signature{std::move(signature)},
    body{std::move(body)}
{}

signature_node::signature_node(const std::string&         identifier,
                               std::unique_ptr<ast_node>& parameter_list,
                               const source_location&     location) :

    ast_node(ast_node_type::FUNC_SIGNATURE, location),
    identifier{identifier},
    parameter_list{std::move(parameter_list)}
{}
return_stmt_node::return_stmt_node(std::unique_ptr<ast_node>& value,
                                   const source_location&     location) :

    ast_node(ast_node_type::RETURN_STMT, location), value{std::move(value)}
{}

parameter_def_node::parameter_def_node(std::unique_ptr<ast_node>& parameter_list,
                                       const source_location&     location) :

    ast_node(ast_node_type::PARAMETER_DEF, location),
    parameter_list{std::move(parameter_list)}
{}

var_decl_node::var_decl_node(const std::string&     identifier,
                             const source_location& location) :

    ast_node(ast_node_type::VAR_DEF, location), identifier{identifier}
{}

var_init_node::var_init_node(const std::string&         identifier,
                             std::unique_ptr<ast_node>& value,
                             const source_location&     location) :

    ast_node(ast_node_type::VAR_INIT, location),
    identifier{identifier},
    value{std::move(value)}
{}

var_assignment_node::var_assignment_node(const std::string&         identifier,
                                         std::unique_ptr<ast_node>& value,
                                         const source_location&     location) :

    ast_node(ast_node_type::VAR_ASSIGNMENT, location),
    identifier{identifier},
    value{std::move(value)}
{}

call_node::call_node(const std::string&         identifier,
                     std::unique_ptr<ast_node>& parameter_pass,
                     const source_location&     location) :

    ast_node(ast_node_type::CALL, location),
    identifier{identifier},
    parameter_pass{std::move(parameter_pass)}
{}

parameter_pass_node::parameter_pass_node(std::unique_ptr<ast_node>& parameter_list,
                                         const source_location&     location) :

    ast_node(ast_node_type::PARAMETER_PASS, location),
    parameter_list{std::move(parameter_list)}
{}

block_node::block_node(std::vector<std::unique_ptr<ast_node>>& statements,
                       const source_location&                  location) :

    ast_node(ast_node_type::BLOCK, location), statements{std::move(statements)}
{}

control_block_node::control_block_node(std::unique_ptr<ast_node>& head,
                                       std::unique_ptr<ast_node>& body,
                                       const source_location&     location) :

    ast_node(ast_node_type::CONTROL_BLOCK, location),
    head{std::move(head)},
    body{std::move(body)}
{}

control_head_node::control_head_node(std::unique_ptr<ast_node>& expression,
                                     const source_location&     location) :

    ast_node(ast_node_type::CONTROL_HEAD, location), expression{std::move(expression)}
{}
