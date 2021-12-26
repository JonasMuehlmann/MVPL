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

const constexpr auto all_tokens = enum_to_array<token, token::RETURN>();
#endif    // SRC_TYPES_TOKEN_HPP_
