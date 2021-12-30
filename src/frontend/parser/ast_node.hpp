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
#ifndef SRC_FRONTEND_PARSER_AST_NODE_HPP
#define SRC_FRONTEND_PARSER_AST_NODE_HPP

#include <memory>
#include <vector>

#include "ast_node_type.hpp"
#include "common/source_location.hpp"
#include "frontend/lexer/token.hpp"

struct ast_node
{
    const ast_node_type   type;
    const source_location source_location_;

    ast_node(const ast_node_type type, const source_location location);
    virtual void print();
};

struct program_node : public ast_node
{
    const std::vector<std::unique_ptr<ast_node>>& globals;

    program_node(std::vector<std::unique_ptr<ast_node>>& globals,
                 const source_location                   location);
};

struct binary_op_node : public ast_node
{
    const std::unique_ptr<ast_node> lhs;
    const std::unique_ptr<ast_node> rhs;

    binary_op_node(const ast_node&       lhs_,
                   const ast_node&       rhs_,
                   const source_location location);
};
struct unary_op_node : public ast_node
{
    const std::unique_ptr<ast_node> operand;

    unary_op_node(const ast_node& operand, const source_location location);
};
struct func_def_node : public ast_node
{
    const std::unique_ptr<ast_node> signature;
    const std::unique_ptr<ast_node> body;

    func_def_node(const ast_node&       signature,
                  const ast_node&       body,
                  const source_location location);
};
struct procedure_def_node : public ast_node
{
    const std::unique_ptr<ast_node> signature;
    const std::unique_ptr<ast_node> body;

    procedure_def_node(const ast_node&       signature,
                       const ast_node&       body,
                       const source_location location);
};
struct signature_node : public ast_node
{
    const std::unique_ptr<ast_node> name;
    const std::unique_ptr<ast_node> parameter_list;

    signature_node(const ast_node&       name,
                   const ast_node&       parameter_list,
                   const source_location location);
};
struct return_stmt_node : public ast_node
{
    const std::unique_ptr<ast_node> value;

    return_stmt_node(const ast_node& value, const source_location location);
};
struct parameter_def_node : public ast_node
{
    const std::unique_ptr<ast_node> parameter_list;

    parameter_def_node(const ast_node& parameter_list, const source_location location);
};
struct var_decl_node : public ast_node
{
    const std::unique_ptr<ast_node> identifier;

    var_decl_node(const ast_node& identifier, const source_location location);
};
struct var_init_node : public ast_node
{
    const std::unique_ptr<ast_node> identifier;
    const std::unique_ptr<ast_node> value;

    var_init_node(const ast_node&       identifier,
                  const ast_node&       value,
                  const source_location location);
};
struct var_assignment_node : public ast_node
{
    const std::unique_ptr<ast_node> identifier;
    const std::unique_ptr<ast_node> value;

    var_assignment_node(const ast_node&       identifier,
                        const ast_node&       value,
                        const source_location location);
};
struct call_node : public ast_node
{
    const std::unique_ptr<ast_node> identifier;
    const std::unique_ptr<ast_node> parameter_pass;

    call_node(const ast_node&       identifier,
              const ast_node&       parameter_pass,
              const source_location location);
};
struct parameter_pass_node : public ast_node
{
    const std::unique_ptr<ast_node> parameter_list;

    parameter_pass_node(const ast_node& parameter_list, const source_location location);
};
struct block_node : public ast_node
{
    const std::vector<std::unique_ptr<ast_node>> statements;

    block_node(const ast_node& statements, const source_location location);
};
struct control_block_node : public ast_node
{
    const std::unique_ptr<ast_node> head;
    const std::unique_ptr<ast_node> body;

    control_block_node(const ast_node&       head,
                       const ast_node&       body,
                       const source_location location);
};
struct control_head_node : public ast_node
{
    const std::unique_ptr<ast_node> expression;

    control_head_node(const ast_node& expression, const source_location location);
};

#endif    // SRC_FRONTEND_PARSER_AST_NODE_HPP
