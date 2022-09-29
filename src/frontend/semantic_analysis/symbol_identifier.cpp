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

#include "symbol_identifier.hpp"

#include <functional>
#include <memory>
#include <ranges>
#include <string_view>

#include "scope_node.hpp"


std::string_view symbol_identifier::make_fully_qualified_name() const
{
    std::stringstream ss;

    this->add_parent_scope_name(ss, this->enclosing_scope.get());
    ss << this->name;

    return ss.view();
}

// TODO: This should probably be moved into the scope_node type

// TODO: We should do more elaborate mangling in order to differentiate between e.g.
// equal variable declarations in two for loops in the same scope
void symbol_identifier::add_parent_scope_name(std::stringstream& name_,
                                              scope_node*        parent_scope) const
{
    if (parent_scope == nullptr)
    {
        return;
    }

    this->add_parent_scope_name(name_, parent_scope->parent_scope.get());
    name_ << parent_scope->name << "::";
}

bool symbol_identifier::operator==(const symbol_identifier& other) const
{
    return this->make_fully_qualified_name() == other.make_fully_qualified_name();
}

std::size_t hash_fn::operator()(const symbol_identifier& s) const
{
    return std::hash<std::string_view>()(s.make_fully_qualified_name());
}
