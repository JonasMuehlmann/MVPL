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

#include "scope_node.hpp"

struct symbol_identifier
{
    std::string_view            name;
    std::shared_ptr<scope_node> enclosing_scope;

    symbol_identifier(std::string_view name, std::shared_ptr<scope_node> enclosing_scope) :
        name{name}, enclosing_scope{enclosing_scope}
    {}
    symbol_identifier() = default;
    std::string_view make_fully_qualified_name() const;
    bool             operator==(const symbol_identifier& other) const;

 private:
    void add_parent_scope_name(std::stringstream& name, scope_node* parent_scope) const;
};

struct hash_fn
{
    std::size_t operator()(const symbol_identifier& s) const;
};
