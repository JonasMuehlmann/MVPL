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
#ifndef SRC_COMMON_ENUM_RANGE_HPP_
#define SRC_COMMON_ENUM_RANGE_HPP_

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
// From https://stackoverflow.com/a/63365764/14977798
template <class T, T last>
struct EnumRange
{
    using size_type = int;

    struct Iterator
    {
        using iterator_category = std::input_iterator_tag;
        using value_type        = T;
        using reference         = T&;
        using pointer           = T*;
        using difference_type   = int;

        constexpr explicit Iterator(int v) : value(v) {}

        constexpr void operator++()
        {
            ++value;
        }

        constexpr bool operator!=(Iterator rhs)
        {
            return value != rhs.value;
        }
        constexpr bool operator==(Iterator rhs)
        {
            return value == rhs.value;
        }

        constexpr T operator*() const
        {
            return static_cast<T>(value);
        }

     private:
        int value = 0;
    };

    [[nodiscard]] constexpr Iterator begin() const
    {
        return Iterator(0);
    }

    [[nodiscard]] constexpr T front() const
    {
        return *begin();
    }

    [[nodiscard]] constexpr Iterator end() const
    {
        return Iterator(static_cast<int>(last) + 1);
    }

    [[nodiscard]] constexpr T back() const
    {
        return end() - 1;
    }

    [[nodiscard]] constexpr size_t size() const
    {
        return static_cast<int>(last) + 1;
    }
};

template <typename EnumType, EnumType last>
constexpr auto enum_to_array()
{
    EnumRange<EnumType, last>          range;
    std::array<EnumType, range.size()> arr{};

    std::copy(range.begin(), range.end(), begin(arr));

    return arr;
}
#endif    // SRC_COMMON_ENUM_RANGE_HPP_
