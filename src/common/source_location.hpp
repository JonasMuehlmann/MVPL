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
#ifndef SRC_COMMON_SOURCE_LOCATION_HPP
#define SRC_COMMON_SOURCE_LOCATION_HPP

#include <cstddef>

struct source_location
{
    const size_t line_start{};
    const size_t col_start{};

    const size_t line_end{};
    const size_t col_end{};

    source_location() = default;

    source_location(size_t line_start,
                    size_t col_start,
                    size_t line_end,
                    size_t col_end) :
        line_start{line_start},
        col_start{col_start},
        line_end{line_end},
        col_end{col_end}
    {}
};

#endif    // SRC_COMMON_SOURCE_LOCATION_HPP
