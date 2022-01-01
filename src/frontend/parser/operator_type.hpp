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
#ifndef SRC_FRONTEND_OPERATOR_TYPE_TYPE_HPP_
#define SRC_FRONTEND_OPERATOR_TYPE_TYPE_HPP_
#include "enum_range.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;


enum class operator_type
{
    LESS,
    LESSEQ,
    GREATER,
    GREATEREQ,
    EQUAL,
    NEQUAL,
    NOT,
    LOGICAL_AND,
    LOGICAL_OR,
    BINARY_AND,
    BINARY_OR,
    LSHIFT,
    RSHIFT,
    XOR,
    PLUS,
    MINUS,
    MULTIPLICATION,
    DIVISION,
    MODULO,
    INCREMENT,
    DECREMENT
};


const int NUM_OPERATORS = []() {
    EnumRange<operator_type, operator_type::DECREMENT> range;

    return range.size();
}();

const auto ALL_OPERATORS = enum_to_array<operator_type, operator_type::DECREMENT>();

const auto LUT_OPERATOR_TYPE_TO_STRING = []() {
    using namespace std::literals::string_view_literals;

    constexpr auto arr = []() {
        std::array<std::string_view, NUM_OPERATORS> arr{};
        arr.fill(""sv);
        arr[static_cast<size_t>(operator_type::LESS)]           = "LESS"sv;
        arr[static_cast<size_t>(operator_type::LESSEQ)]         = "LESSEQ"sv;
        arr[static_cast<size_t>(operator_type::GREATER)]        = "GREATER"sv;
        arr[static_cast<size_t>(operator_type::GREATEREQ)]      = "GREATEREQ"sv;
        arr[static_cast<size_t>(operator_type::EQUAL)]          = "EQUAL"sv;
        arr[static_cast<size_t>(operator_type::NEQUAL)]         = "NEQUAL"sv;
        arr[static_cast<size_t>(operator_type::NOT)]            = "NOT"sv;
        arr[static_cast<size_t>(operator_type::LOGICAL_AND)]    = "LOGICAL_AND"sv;
        arr[static_cast<size_t>(operator_type::LOGICAL_OR)]     = "LOGICAL_OR"sv;
        arr[static_cast<size_t>(operator_type::BINARY_AND)]     = "BINARY_AND"sv;
        arr[static_cast<size_t>(operator_type::BINARY_OR)]      = "BINARY_OR"sv;
        arr[static_cast<size_t>(operator_type::LSHIFT)]         = "LSHIFT"sv;
        arr[static_cast<size_t>(operator_type::RSHIFT)]         = "RSHIFT"sv;
        arr[static_cast<size_t>(operator_type::XOR)]            = "XOR"sv;
        arr[static_cast<size_t>(operator_type::PLUS)]           = "PLUS"sv;
        arr[static_cast<size_t>(operator_type::MINUS)]          = "MINUS"sv;
        arr[static_cast<size_t>(operator_type::MULTIPLICATION)] = "MULTIPLICATION"sv;
        arr[static_cast<size_t>(operator_type::DIVISION)]       = "DIVISION"sv;
        arr[static_cast<size_t>(operator_type::MODULO)]         = "MODULO"sv;
        arr[static_cast<size_t>(operator_type::INCREMENT)]      = "INCREMENT"sv;
        arr[static_cast<size_t>(operator_type::DECREMENT)]      = "DECREMENT"sv;


        return arr;
    }();

    static_assert(
        std::ranges::count_if(arr, [](std::string_view str) { return str == ""sv; })
            == 0,
        "operator_type missing value");

    return arr;
}();


inline void to_json(json& j, const operator_type& node)
{
    j = json{LUT_OPERATOR_TYPE_TO_STRING[static_cast<size_t>(node)]};
};
#endif    // SRC_FRONTEND_OPERATOR_TYPE_TYPE_HPP_
