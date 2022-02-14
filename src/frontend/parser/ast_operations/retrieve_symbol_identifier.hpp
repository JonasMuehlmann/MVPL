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
#include <concepts>
#include <exception>
#include <string_view>

#include "ast_node.hpp"

struct symbol_identifier_retriever_visitor
{
    std::string_view operator()(var_decl_node node)
    {
        return node.identifier;
    }

    std::string_view operator()(var_init_node node)
    {
        return node.identifier;
    }

    std::string_view operator()(func_def_node node)
    {
        return std::get<signature_node>(*(node.signature)).identifier;
    }

    std::string_view operator()(procedure_def_node node)
    {
        return std::get<signature_node>(*(node.signature)).identifier;
    }

    std::string_view operator()([[maybe_unused]] auto node)
    {
        throw std::invalid_argument(
            "Tried to retrieve symbol identifier of ast node, which "
            "does not declare a symbol");
    }
};
