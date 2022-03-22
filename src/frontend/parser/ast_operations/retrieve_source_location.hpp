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
#include <concepts>
#include <exception>

#include "ast_node.hpp"
#include "source_location.hpp"
struct source_location_retriever_visitor
{
    template <typename T>
    requires(!std::same_as<T, missing_optional_node> && !std::same_as<T, parse_error>)
        source_location&
        operator()(T& node) const
    {
        return node.source_location_;
    }

    source_location& operator()([[maybe_unused]] missing_optional_node& node) const
    {
        throw std::invalid_argument(
            "Tried to retrieve source code location of empty AST node");
    }
    source_location& operator()([[maybe_unused]] parse_error& node) const
    {
        throw std::invalid_argument(
            "Tried to retrieve source code location of parse_error");
    }
};
