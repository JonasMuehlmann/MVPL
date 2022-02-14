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

#include "frontend/parser/ast_operations/retrieve_symbol_identifier.hpp"
#include "frontend/semantic_analysis/symbol.hpp"
#include "frontend/semantic_analysis/symbol_identifier.hpp"
#include "list_nodes.hpp"

bool does_node_declare_symbol(ast_node_t node)
{
    return std::holds_alternative<var_decl_node>(node)
           || std::holds_alternative<var_init_node>(node)
           || std::holds_alternative<func_def_node>(node)
           || std::holds_alternative<procedure_def_node>(node);
}

symbol_table build_symbol_table(ast_node_t ast)
{
    scope_node                                     scope_tree("global", nullptr);
    symbol_table                                   symbol_table;
    std::forward_list<std::shared_ptr<ast_node_t>> ast_nodes;


    std::visit(node_lister_visitor(ast_nodes), ast);

    for (auto& ast_node : ast_nodes)
    {
        if (does_node_declare_symbol(*ast_node))
        {
            auto symbol_name =
                std::visit(symbol_identifier_retriever_visitor(), *ast_node);

            // TODO: How can we efficiently find the scope of a node?
            auto scope      = scope_node{};
            auto identifier = symbol_identifier(symbol_name, scope);
        }
    }

    return symbol_table;
}
