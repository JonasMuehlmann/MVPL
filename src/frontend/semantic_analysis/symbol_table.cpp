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

#include "frontend/semantic_analysis/symbol_table.hpp"

#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>

#include "frontend/parser/ast_operations/retrieve_symbol_identifier.hpp"
#include "frontend/semantic_analysis/symbol.hpp"
#include "frontend/semantic_analysis/symbol_identifier.hpp"
#include "list_nodes.hpp"
#include "retrieve_source_location.hpp"
#include "semantic_error.hpp"
#include "stringify_semantic_error.hpp"

bool does_node_declare_symbol(ast_node_t node)
{
    return std::holds_alternative<var_decl_node>(node)
           || std::holds_alternative<var_init_node>(node)
           || std::holds_alternative<func_def_node>(node)
           || std::holds_alternative<procedure_def_node>(node)
           || (std::holds_alternative<for_loop_node>(node)
               && (std::holds_alternative<var_init_node>(
                       *std::get<for_loop_node>(node).init_stmt)
                   || std::holds_alternative<var_init_node>(
                       *std::get<for_loop_node>(node).init_stmt)));
}

symbol_table build_symbol_table(ast_node_t ast)
{
    // TODO: The use of shared_ptr might not be optimal/needed
    std::shared_ptr<scope_node> scope_tree =
        std::make_shared<scope_node>("global", nullptr);
    std::shared_ptr<scope_node>                    cur_scope_node = scope_tree;
    symbol_table                                   symbol_table;
    std::forward_list<std::shared_ptr<ast_node_t>> ast_nodes;
    std::vector<semantic_error_t>                  semantic_errors;


    std::visit(node_lister_visitor(ast_nodes), ast);

    for (auto& ast_node : ast_nodes)
    {
        if (does_node_declare_symbol(*ast_node))
        {
            auto symbol_name =
                std::visit(symbol_identifier_retriever_visitor(), *ast_node);

            // TODO: How can we efficiently find the scope of a node?
            //  We could keep track of the current scope and when processing a leaf
            //  node, we could add the current scope path to the scope tree

            auto scope      = std::make_shared<scope_node>(symbol_name, cur_scope_node);
            auto identifier = symbol_identifier(symbol_name, scope);

            if (!symbol_table.contains(identifier))
            {
                auto type = get_symbol_type_from_ast_node(ast_node);

                symbol_table[identifier] =
                    symbol(type,
                           identifier,
                           std::visit(source_location_retriever_visitor(), ast_node));
            }
        }
    }

    return symbol_table;
}

void throw_semantic_analysis_error(const std::vector<semantic_error_t>& errors)
{
    std::string error_message;

    for (auto& error : errors)
    {
        error_message +=
            std::visit(semantic_error_stringifier_visitor(), error) + "\n\n";
    }

    throw std::runtime_error(error_message);
}
