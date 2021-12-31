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
#include "print.hpp"

print_visitor::print_visitor() : nesting{0} {}

print_visitor::print_visitor(size_t nesting) : nesting{nesting} {}

void print_visitor::print_base(const ast_node& node)
{
    std::cout << std::string(4LL * nesting, ' ') << R"("type": ")"
              << LUT_AST_NODE_TYPE_TO_STRING[static_cast<size_t>(node.type)] << ",\n"
              << std::string(4LL * nesting, ' ') << R"("source_location": )";
    print_source_location(node.source_location_);

    std::cout << ",\n" << std::string(4LL * nesting, ' ');
}
void print_visitor::operator()(const program_node& node)
{
    if (nesting > 1)
    {
        std::cout << std::string(4LL * nesting, ' ');
    }
    std::cout << "{\n";

    nesting++;

    print_base(node);

    std::cout << R"("globals": [)";

    for (const auto& global : node.globals)
    {
        if (&global == &node.globals.front())
        {
            std::cout << '\n';
        }

        std::visit(print_visitor(nesting + 1), *global);


        if (&global != &node.globals.back())
        {
            std::cout << ',';
        }
        std::cout << '\n';
    }

    if (!node.globals.empty())
    {
        std::cout << std::string(4LL * nesting, ' ');
    }
    std::cout << ']';

    std::cout << "\n";
    nesting--;
    if (nesting > 1)
    {
        std::cout << std::string(4LL * nesting, ' ');
    }
    std::cout << '}';
}
void print_visitor::operator()(const binary_op_node& node) {}
void print_visitor::operator()(const unary_op_node& node) {}
void print_visitor::operator()(const func_def_node& node) {}
void print_visitor::operator()(const procedure_def_node& node) {}
void print_visitor::operator()(const signature_node& node) {}
void print_visitor::operator()(const return_stmt_node& node) {}
void print_visitor::operator()(const parameter_def_node& node) {}
void print_visitor::operator()(const var_decl_node& node) {}
void print_visitor::operator()(const var_init_node& node) {}
void print_visitor::operator()(const var_assignment_node& node) {}
void print_visitor::operator()(const call_node& node) {}
void print_visitor::operator()(const parameter_pass_node& node) {}
void print_visitor::operator()(const block_node& node) {}
void print_visitor::operator()(const control_block_node& node) {}
void print_visitor::operator()(const control_head_node& node) {}
