// Copyright © 2021-2022 Jonas Muehlmann
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

#include "semantic_error.hpp"

call_to_variable::call_to_variable(std::shared_ptr<ast_node_t> violating_call) :
    violating_call(violating_call)
{}

assignment_to_callable::assignment_to_callable(
    std::shared_ptr<ast_node_t> violating_assignment) :
    violating_assignment(violating_assignment)
{}

evaluatio_of_procedure::evaluatio_of_procedure(
    std::shared_ptr<ast_node_t> violating_evaluation) :
    violating_evaluation(violating_evaluation)
{}

redifintion_of_symbol::redifintion_of_symbol(
    std::shared_ptr<ast_node_t> violating_definition,
    std::shared_ptr<ast_node_t> previous_definition) :
    violating_definition(violating_definition), previous_definition(previous_definition)
{}

missing_main_function::missing_main_function() {}

use_before_definition::use_before_definition(std::shared_ptr<ast_node_t> violating_use,
                                             std::shared_ptr<ast_node_t> definition) :
    violating_use(violating_use), definition(definition)
{}
