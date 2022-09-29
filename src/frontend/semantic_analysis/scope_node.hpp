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

#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "ast_node.hpp"

struct scope_node
{
    std::string_view                         name;
    std::shared_ptr<scope_node>              parent_scope;
    std::vector<std::shared_ptr<scope_node>> child_scopes;

    scope_node(std::string_view name_, std::shared_ptr<scope_node> parent_scope_) :
        name{name_}, parent_scope{parent_scope_}, child_scopes{}
    {}

    scope_node() : name{}, parent_scope{}, child_scopes{} {}

    std::shared_ptr<scope_node> find_symbol_identifier_in_parents(std::string_view needle)
    {
        if (name == needle)
        {
            return std::make_shared<scope_node>(*this);
        }

        if (parent_scope == nullptr)
        {
            return {};
        }

        return parent_scope->find_symbol_identifier_in_parents(needle);
    }
};
