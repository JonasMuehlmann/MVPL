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

#include <memory>
#include <string>

#include "frontend/semantic_analysis/scope_node.hpp"
#include "frontend/semantic_analysis/symbol_identifier.hpp"
#include "frontend/semantic_analysis/symbol_type.hpp"
#include "source_location.hpp"

struct symbol
{
    symbol_type       type;
    std::size_t       scope_index_of_defintion;
    symbol_identifier identifier;
    source_location   source_location;

    symbol(symbol_type            type,
           std::size_t            scope_index_of_defintion,
           symbol_identifier      identifier,
           struct source_location source_location) :
        type{type},
        scope_index_of_defintion{scope_index_of_defintion},
        identifier{identifier},
        source_location(source_location)
    {}
};
