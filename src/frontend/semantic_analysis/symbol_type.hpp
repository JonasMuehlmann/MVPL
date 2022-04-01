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

#include <variant>

#include "ast_node.hpp"

enum class symbol_type
{
    PROCEDURE,
    FUNCTION,
    MAIN_FUNCTION,
    VARIABLE
};

symbol_type get_symbol_type_from_ast_node(ast_node_t& node)
{
    if (std::holds_alternative<procedure_def_node>(node))
    {}

    else if (std::holds_alternative<func_def_node>(node))
    {}

    else if (std::holds_alternative<func_def_node>(node)
             && std::get<signature_node>(std::get<func_def_node>(node).signature)
                        .identifier
                    == "main")
    {}

    else if (std::holds_alternative<var_decl_node>(node)
             || std::holds_alternative<var_init_node>(node))
    {}
}
