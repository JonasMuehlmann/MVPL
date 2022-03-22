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

#include <algorithm>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "common/enum_range.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

enum class token_type
{
    // Syntax
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,
    SEMICOLON,
    COLON,
    COMMA,
    IDENTIFIER,
    LITERAL,
    ASSIGN,
    LBLOCKCOMMENT,
    RBLOCKCOMMENT,
    LINECOMMENT,
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
    ELSE,
    SWITCH,
    CASE,
    // Misc
    LET,
    FUNCTION,
    PROCEDURE,
    RETURN,
    END_TOKEN
};

// TODO: Make all of this constepxr
// TODO: try building a  static hash map with an std::array and a hash function
const std::array DYNAMIC_TOKENS{token_type::IDENTIFIER, token_type::LITERAL};
const std::array NON_VALUE_TOKENS{token_type::END_TOKEN};

const int NUM_TOKENS = []() {
    EnumRange<token_type, token_type::END_TOKEN> range;

    return range.size();
}();

// PERF: Should all other arrays be spans to save memory?
const auto ALL_TOKENS = enum_to_array<token_type, token_type::END_TOKEN>();

// TODO: Try using ranges and views here
const auto STATIC_TOKENS = []() {
    std::array<token_type, NUM_TOKENS> arr;

    std::ranges::set_difference(ALL_TOKENS, DYNAMIC_TOKENS, begin(arr));

    std::ranges::set_difference(arr, NON_VALUE_TOKENS, begin(arr));

    return arr;
}();


const auto LUT_TOKEN_TO_LEXEME = []() {
    using namespace std::literals::string_view_literals;

    constexpr auto arr = []() {
        std::array<std::string_view, NUM_TOKENS> arr{};
        arr.fill(""sv);


        arr[static_cast<size_t>(token_type::LPAREN)]         = "("sv;
        arr[static_cast<size_t>(token_type::RPAREN)]         = ")"sv;
        arr[static_cast<size_t>(token_type::LBRACKET)]       = "["sv;
        arr[static_cast<size_t>(token_type::RBRACKET)]       = "]"sv;
        arr[static_cast<size_t>(token_type::LBRACE)]         = "{"sv;
        arr[static_cast<size_t>(token_type::RBRACE)]         = "}"sv;
        arr[static_cast<size_t>(token_type::SEMICOLON)]      = ";"sv;
        arr[static_cast<size_t>(token_type::COLON)]          = ":"sv;
        arr[static_cast<size_t>(token_type::COMMA)]          = ","sv;
        arr[static_cast<size_t>(token_type::ASSIGN)]         = "="sv;
        arr[static_cast<size_t>(token_type::LBLOCKCOMMENT)]  = "/*"sv;
        arr[static_cast<size_t>(token_type::RBLOCKCOMMENT)]  = "*/"sv;
        arr[static_cast<size_t>(token_type::LINECOMMENT)]    = "//"sv;
        arr[static_cast<size_t>(token_type::LESS)]           = "<"sv;
        arr[static_cast<size_t>(token_type::LESSEQ)]         = "<="sv;
        arr[static_cast<size_t>(token_type::GREATER)]        = ">"sv;
        arr[static_cast<size_t>(token_type::GREATEREQ)]      = ">="sv;
        arr[static_cast<size_t>(token_type::EQUAL)]          = "=="sv;
        arr[static_cast<size_t>(token_type::NEQUAL)]         = "!="sv;
        arr[static_cast<size_t>(token_type::NOT)]            = "!"sv;
        arr[static_cast<size_t>(token_type::LOGICAL_AND)]    = "&&"sv;
        arr[static_cast<size_t>(token_type::LOGICAL_OR)]     = "||"sv;
        arr[static_cast<size_t>(token_type::BINARY_AND)]     = "&"sv;
        arr[static_cast<size_t>(token_type::BINARY_OR)]      = "|"sv;
        arr[static_cast<size_t>(token_type::LSHIFT)]         = "<<"sv;
        arr[static_cast<size_t>(token_type::RSHIFT)]         = ">>"sv;
        arr[static_cast<size_t>(token_type::XOR)]            = "^"sv;
        arr[static_cast<size_t>(token_type::PLUS)]           = "+"sv;
        arr[static_cast<size_t>(token_type::MINUS)]          = "-"sv;
        arr[static_cast<size_t>(token_type::MULTIPLICATION)] = "*"sv;
        arr[static_cast<size_t>(token_type::DIVISION)]       = "/"sv;
        arr[static_cast<size_t>(token_type::MODULO)]         = "%"sv;
        arr[static_cast<size_t>(token_type::INCREMENT)]      = "++"sv;
        arr[static_cast<size_t>(token_type::DECREMENT)]      = "--"sv;
        arr[static_cast<size_t>(token_type::WHILE)]          = "while"sv;
        arr[static_cast<size_t>(token_type::FOR)]            = "for"sv;
        arr[static_cast<size_t>(token_type::IF)]             = "if"sv;
        arr[static_cast<size_t>(token_type::ELSE)]           = "else"sv;
        arr[static_cast<size_t>(token_type::SWITCH)]         = "switch"sv;
        arr[static_cast<size_t>(token_type::CASE)]           = "case"sv;
        arr[static_cast<size_t>(token_type::LET)]            = "let"sv;
        arr[static_cast<size_t>(token_type::FUNCTION)]       = "function"sv;
        arr[static_cast<size_t>(token_type::PROCEDURE)]      = "procedure"sv;
        arr[static_cast<size_t>(token_type::RETURN)]         = "return"sv;

        return arr;
    }();

    static_assert(
        std::ranges::count_if(arr, [](std::string_view str) { return str == ""sv; })
            == DYNAMIC_TOKENS.size() + NON_VALUE_TOKENS.size(),
        "Static token_type missing value");

    return arr;
}();

const auto LUT_LEXEME_TO_TOKEN = []() {
    std::unordered_map<std::string_view, token_type> map;

    for (auto t : STATIC_TOKENS)
    {
        map.insert({LUT_TOKEN_TO_LEXEME[static_cast<size_t>(t)], t});
    }

    return map;
}();

const auto LUT_TOKEN_TO_STRING = []() {
    using namespace std::literals::string_view_literals;

    constexpr auto arr = []() {
        std::array<std::string_view, NUM_TOKENS> arr{};
        arr.fill(""sv);


        arr[static_cast<size_t>(token_type::LPAREN)]         = "LPAREN"sv;
        arr[static_cast<size_t>(token_type::RPAREN)]         = "RPAREN"sv;
        arr[static_cast<size_t>(token_type::LBRACKET)]       = "LBRACKET"sv;
        arr[static_cast<size_t>(token_type::RBRACKET)]       = "RBRACKET"sv;
        arr[static_cast<size_t>(token_type::LBRACE)]         = "LBRACE"sv;
        arr[static_cast<size_t>(token_type::RBRACE)]         = "RBRACE"sv;
        arr[static_cast<size_t>(token_type::SEMICOLON)]      = "SEMICOLON"sv;
        arr[static_cast<size_t>(token_type::COLON)]          = "COLON"sv;
        arr[static_cast<size_t>(token_type::COMMA)]          = "COMMA"sv;
        arr[static_cast<size_t>(token_type::LITERAL)]        = "LITERAL"sv;
        arr[static_cast<size_t>(token_type::IDENTIFIER)]     = "IDENTIFIER"sv;
        arr[static_cast<size_t>(token_type::ASSIGN)]         = "ASSIGN"sv;
        arr[static_cast<size_t>(token_type::LBLOCKCOMMENT)]  = "LBLOCKCOMMENT"sv;
        arr[static_cast<size_t>(token_type::RBLOCKCOMMENT)]  = "RBLOCKCOMMENT"sv;
        arr[static_cast<size_t>(token_type::LINECOMMENT)]    = "LINECOMMENT"sv;
        arr[static_cast<size_t>(token_type::LESS)]           = "LESS"sv;
        arr[static_cast<size_t>(token_type::LESSEQ)]         = "LESSEQ"sv;
        arr[static_cast<size_t>(token_type::GREATER)]        = "GREATER"sv;
        arr[static_cast<size_t>(token_type::GREATEREQ)]      = "GREATEREQ"sv;
        arr[static_cast<size_t>(token_type::EQUAL)]          = "EQUAL"sv;
        arr[static_cast<size_t>(token_type::NEQUAL)]         = "NEQUAL"sv;
        arr[static_cast<size_t>(token_type::NOT)]            = "NOT"sv;
        arr[static_cast<size_t>(token_type::LOGICAL_AND)]    = "LOGICAL_AND"sv;
        arr[static_cast<size_t>(token_type::LOGICAL_OR)]     = "LOGICAL_OR"sv;
        arr[static_cast<size_t>(token_type::BINARY_AND)]     = "BINARY_AND"sv;
        arr[static_cast<size_t>(token_type::BINARY_OR)]      = "BINARY_OR"sv;
        arr[static_cast<size_t>(token_type::LSHIFT)]         = "LSHIFT"sv;
        arr[static_cast<size_t>(token_type::RSHIFT)]         = "RSHIFT"sv;
        arr[static_cast<size_t>(token_type::XOR)]            = "XOR"sv;
        arr[static_cast<size_t>(token_type::PLUS)]           = "PLUS"sv;
        arr[static_cast<size_t>(token_type::MINUS)]          = "MINUS"sv;
        arr[static_cast<size_t>(token_type::MULTIPLICATION)] = "MULTIPLICATION"sv;
        arr[static_cast<size_t>(token_type::DIVISION)]       = "DIVISION"sv;
        arr[static_cast<size_t>(token_type::MODULO)]         = "MODULO"sv;
        arr[static_cast<size_t>(token_type::INCREMENT)]      = "INCREMENT"sv;
        arr[static_cast<size_t>(token_type::DECREMENT)]      = "DECREMENT"sv;
        arr[static_cast<size_t>(token_type::WHILE)]          = "WHILE"sv;
        arr[static_cast<size_t>(token_type::FOR)]            = "FOR"sv;
        arr[static_cast<size_t>(token_type::IF)]             = "IF"sv;
        arr[static_cast<size_t>(token_type::ELSE)]           = "ELSE"sv;
        arr[static_cast<size_t>(token_type::SWITCH)]         = "SWITCH"sv;
        arr[static_cast<size_t>(token_type::CASE)]           = "CASE"sv;
        arr[static_cast<size_t>(token_type::LET)]            = "LET"sv;
        arr[static_cast<size_t>(token_type::FUNCTION)]       = "FUNCTION"sv;
        arr[static_cast<size_t>(token_type::PROCEDURE)]      = "PROCEDURE"sv;
        arr[static_cast<size_t>(token_type::RETURN)]         = "RETURN"sv;
        arr[static_cast<size_t>(token_type::END_TOKEN)]      = "END_TOKEN"sv;

        return arr;
    }();

    static_assert(
        std::ranges::count_if(arr, [](std::string_view str) { return str == ""sv; })
            == 0,
        "Static token_type missing value");

    return arr;
}();

const auto LUT_STRING_TO_TOKEN = []() {
    std::unordered_map<std::string_view, token_type> map;

    for (auto t : STATIC_TOKENS)
    {
        map.insert({LUT_TOKEN_TO_STRING[static_cast<size_t>(t)], t});
    }

    return map;
}();

const auto KEYWORD_LEXEMES = []() {
    std::unordered_set<std::string_view> lexemes;

    for (auto t : STATIC_TOKENS)
    {
        auto keyword = LUT_TOKEN_TO_LEXEME[static_cast<size_t>(t)];

        if (std::ranges::all_of(keyword, [](char c) {
                return std::isalpha(static_cast<unsigned char>(c)) == 1;
            }))
        {
            lexemes.insert(keyword);
        };
    }

    return lexemes;
}();

const auto OPERATOR_LEXEMES = []() {
    std::unordered_set<std::string_view> lexemes;

    for (auto t : STATIC_TOKENS)
    {
        auto keyword = LUT_TOKEN_TO_LEXEME[static_cast<size_t>(t)];

        if (std::ranges::all_of(keyword, [](char c) {
                return std::isalpha(static_cast<unsigned char>(c)) == 0;
            }))
        {
            lexemes.insert(keyword);
        };
    }

    return lexemes;
}();

inline void to_json(json& j, const token_type& node)
{
    j = LUT_TOKEN_TO_STRING[static_cast<size_t>(node)];
}
