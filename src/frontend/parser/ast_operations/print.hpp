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
#ifndef SRC_FRONTEND_PARSER_AST_OPERATIONS_PRINT_HPP
#define SRC_FRONTEND_PARSER_AST_OPERATIONS_PRINT_HPP

#include <iostream>

#include "ast_node.hpp"
#include "util.hpp"

struct print_visitor
{
    size_t nesting;

    print_visitor();
    explicit print_visitor(size_t nesting);

    void print_base(const ast_node& node);

    void operator()(const program_node& node);
    void operator()(const binary_op_node& node);
    void operator()(const unary_op_node& node);
    void operator()(const func_def_node& node);
    void operator()(const procedure_def_node& node);
    void operator()(const signature_node& node);
    void operator()(const return_stmt_node& node);
    void operator()(const parameter_def_node& node);
    void operator()(const var_decl_node& node);
    void operator()(const var_init_node& node);
    void operator()(const var_assignment_node& node);
    void operator()(const call_node& node);
    void operator()(const parameter_pass_node& node);
    void operator()(const block_node& node);
    void operator()(const control_block_node& node);
    void operator()(const control_head_node& node);
};

#endif    // SRC_FRONTEND_PARSER_AST_OPERATIONS_PRINT_HPP
