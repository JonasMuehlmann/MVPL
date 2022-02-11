#include "frontend/semantic_analysis/symbol_table.hpp"

#include "frontend/semantic_analysis/symbol.hpp"

symbol_table build_symbol_table(ast_node_t ast)
{
    scope_node   scope_tree("global", nullptr);
    symbol_table symbol_table;

    // TODO: Add visitor functions for listing ast_node_t
    // TODO: Should the ast use shared_ptr instead of unique_ptr?

    return symbol_table;
}
