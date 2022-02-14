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

#pragma once

#include <memory>
#include <string>
#include <variant>

#include "ast_node.hpp"

struct call_to_variable;

struct assignment_to_callable;

struct evaluatio_of_procedure;

struct redifintion_of_symbol;

struct missing_main_function;

struct use_before_definition;


using semantic_error_t = std::variant<call_to_variable,
                                      assignment_to_callable,
                                      evaluatio_of_procedure,
                                      redifintion_of_symbol,
                                      missing_main_function,
                                      use_before_definition>;
struct call_to_variable
{
    std::shared_ptr<ast_node_t> violating_call;

    explicit call_to_variable(std::shared_ptr<ast_node_t> violating_call);
};

struct assignment_to_callable
{
    std::shared_ptr<ast_node_t> violating_assignment;

    explicit assignment_to_callable(std::shared_ptr<ast_node_t> violating_assignment);
};

struct evaluatio_of_procedure
{
    std::shared_ptr<ast_node_t> violating_evaluation;

    explicit evaluatio_of_procedure(std::shared_ptr<ast_node_t> violating_evaluation);
};

struct redifintion_of_symbol
{
    std::shared_ptr<ast_node_t> violating_definition;
    std::shared_ptr<ast_node_t> previous_definition;

    redifintion_of_symbol(std::shared_ptr<ast_node_t> violating_definition,
                          std::shared_ptr<ast_node_t> previous_definition);
};

struct missing_main_function
{
    missing_main_function();
};

struct use_before_definition
{
    std::shared_ptr<ast_node_t> violating_use;
    std::shared_ptr<ast_node_t> definition;

    use_before_definition(std::shared_ptr<ast_node_t> violating_use,
                          std::shared_ptr<ast_node_t> definition);
};
