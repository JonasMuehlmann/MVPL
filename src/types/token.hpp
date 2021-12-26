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
#ifndef SRC_TYPES_TOKEN_HPP_
#define SRC_TYPES_TOKEN_HPP_

#include <algorithm>
#include <string_view>
#include <unordered_map>

#include "enum_range.hpp"

enum class token
{
    // Syntax
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LBRACAE,
    RBRACE,
    SEMICOLON,
    COMMA,
    IDENTIFIER,
    LITERAL,
    ASSIGN,
    // Operators
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
    DECREMENT,
    // Flow control
    WHILE,
    FOR,
    IF,
    ELIF,
    ELSE,
    // Misc
    FUNCTION,
    PROCEDURE,
    RETURN
};

const std::array DYNAMIC_TOKENS{token::IDENTIFIER, token::LITERAL};

const int NUM_TOKENS = []() {
    EnumRange<token, token::RETURN> range;

    return range.size();
}();

const auto ALL_TOKENS = enum_to_array<token, token::RETURN>();

const auto STATIC_TOKENS = []() {
    std::array<token, NUM_TOKENS> arr;

    std::set_difference(begin(ALL_TOKENS),
                        end(ALL_TOKENS),
                        begin(DYNAMIC_TOKENS),
                        end(DYNAMIC_TOKENS),
                        begin(arr));

    return arr;
}();

const auto LUT_TOKEN_TO_STRING = []() {
    using namespace std::literals::string_view_literals;

    constexpr auto arr = []() {
        std::array<std::string_view, NUM_TOKENS> arr{};
        arr.fill(""sv);


        arr[static_cast<size_t>(token::LPAREN)]         = "("sv;
        arr[static_cast<size_t>(token::RPAREN)]         = ")"sv;
        arr[static_cast<size_t>(token::LBRACKET)]       = "["sv;
        arr[static_cast<size_t>(token::RBRACKET)]       = "]"sv;
        arr[static_cast<size_t>(token::LBRACAE)]        = "{"sv;
        arr[static_cast<size_t>(token::RBRACE)]         = "}"sv;
        arr[static_cast<size_t>(token::SEMICOLON)]      = ";"sv;
        arr[static_cast<size_t>(token::COMMA)]          = ","sv;
        arr[static_cast<size_t>(token::ASSIGN)]         = "="sv;
        arr[static_cast<size_t>(token::LESS)]           = "<"sv;
        arr[static_cast<size_t>(token::LESSEQ)]         = "<="sv;
        arr[static_cast<size_t>(token::GREATER)]        = ">"sv;
        arr[static_cast<size_t>(token::GREATEREQ)]      = ">="sv;
        arr[static_cast<size_t>(token::EQUAL)]          = "=="sv;
        arr[static_cast<size_t>(token::NEQUAL)]         = "!="sv;
        arr[static_cast<size_t>(token::NOT)]            = "!"sv;
        arr[static_cast<size_t>(token::LOGICAL_AND)]    = "&&"sv;
        arr[static_cast<size_t>(token::LOGICAL_OR)]     = "||"sv;
        arr[static_cast<size_t>(token::BINARY_AND)]     = "&"sv;
        arr[static_cast<size_t>(token::BINARY_OR)]      = "|"sv;
        arr[static_cast<size_t>(token::LSHIFT)]         = "<<"sv;
        arr[static_cast<size_t>(token::RSHIFT)]         = ">>"sv;
        arr[static_cast<size_t>(token::XOR)]            = "^"sv;
        arr[static_cast<size_t>(token::PLUS)]           = "+"sv;
        arr[static_cast<size_t>(token::MINUS)]          = "-"sv;
        arr[static_cast<size_t>(token::MULTIPLICATION)] = "*"sv;
        arr[static_cast<size_t>(token::DIVISION)]       = "/"sv;
        arr[static_cast<size_t>(token::MODULO)]         = "%"sv;
        arr[static_cast<size_t>(token::INCREMENT)]      = "++"sv;
        arr[static_cast<size_t>(token::DECREMENT)]      = "--"sv;
        arr[static_cast<size_t>(token::WHILE)]          = "while"sv;
        arr[static_cast<size_t>(token::FOR)]            = "for"sv;
        arr[static_cast<size_t>(token::IF)]             = "if"sv;
        arr[static_cast<size_t>(token::ELIF)]           = "elif"sv;
        arr[static_cast<size_t>(token::ELSE)]           = "else"sv;
        arr[static_cast<size_t>(token::FUNCTION)]       = "function"sv;
        arr[static_cast<size_t>(token::PROCEDURE)]      = "procedure"sv;
        arr[static_cast<size_t>(token::RETURN)]         = "return"sv;

        return arr;
    }();

    static_assert(
        std::ranges::count_if(arr, [](std::string_view str) { return str == ""sv; })
            == DYNAMIC_TOKENS.size(),
        "Static token missing value");

    return arr;
}();

const auto LUT_STRING_TO_TOKEN = []() {
    std::unordered_map<std::string_view, token> map;

    std::for_each(begin(STATIC_TOKENS), end(STATIC_TOKENS), [&map](token t) {
        map.insert({LUT_TOKEN_TO_STRING[static_cast<size_t>(t)], t});
    });

    return map;
}();
#endif    // SRC_TYPES_TOKEN_HPP_
