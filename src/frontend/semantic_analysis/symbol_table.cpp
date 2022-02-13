#include "frontend/semantic_analysis/symbol_table.hpp"

#include "frontend/semantic_analysis/symbol.hpp"
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


    // TODO: Should the ast use shared_ptr instead of unique_ptr?
    std::visit(node_lister_visitor(ast_nodes), ast);

    for (auto& ast_node : ast_nodes)
    {
        if (does_node_declare_symbol(*ast_node))
        {
            auto identifier =
                std::visit(symbol_identifier_retriever_visitor(), ast_node);
        }
    }

    return symbol_table;
}
