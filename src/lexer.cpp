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
#include "lexer.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <exception>
#include <iterator>
#include <stdexcept>
#include <string_view>

#include "token.hpp"
#include "token_type.hpp"
lexer::lexer(std::string_view source_code) : source_code{source_code} {}


bool lexer::is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\v' || c == '\f';
}
bool lexer::is_word(char c)
{
    return std::isalnum(static_cast<unsigned char>(c)) == 1 || c == '_';
}
std::string_view lexer::peek_next_word()
{
    // FIX: This seems to be buggy, it substrings until the end of the file
    return source_code.substr(
        0,
        std::distance(begin(source_code), std::ranges::find_if(source_code, [](char c) {
                          return !is_word(c);
                      })));
}

void lexer::skip_whitespace()
{
    auto n_whitespace_chars = std::distance(
        cur_char, std::find_if_not(cur_char, end(source_code), [](char c) {
            return is_whitespace(c);
        }));

    auto n_line_breaks = std::distance(
        cur_char,
        std::find_if_not(cur_char, end(source_code), [](char c) { return c == '\n'; }));

    std::advance(cur_char, n_whitespace_chars);
    cur_col += n_whitespace_chars;

    if (n_line_breaks > 0)
    {
        cur_col = 0;
        cur_line += n_line_breaks;
    }

    std::advance(cur_char, n_line_breaks);
}
std::vector<token> lexer::lex()
{
    skip_whitespace();

    while (cur_char < end(source_code))
    {
        skip_whitespace();

        std::string_view next_word = peek_next_word();

        // Handle keywords
        if (LUT_STRING_VALUE_TO_TOKEN.contains(next_word))
        {
            token_stream.emplace_back(token(
                LUT_STRING_VALUE_TO_TOKEN.at(next_word), next_word, cur_line, cur_col));
        }
        // Handle literals
        else if (std::ranges::all_of(next_word,
                                     [](unsigned char c) { return std::isdigit(c); }))
        {
            token_stream.emplace_back(
                token(token_type::LITERAL, next_word, cur_line, cur_col));
        }
        // Handle identifiers
        else if (std::ranges::all_of(next_word, [](unsigned char c) {
                     return (std::isalpha(c) != 0) || c == '_';
                 }))
        {
            token_stream.emplace_back(
                token(token_type::IDENTIFIER, next_word, cur_line, cur_col));
        }
        // Handle double char operators
        else if (auto lexeme = source_code.substr(0, 2);
                 OPERATOR_LEXEMES.contains(lexeme))
        {
            token_stream.emplace_back(
                token(LUT_STRING_VALUE_TO_TOKEN.at(lexeme), lexeme, cur_line, cur_col));
        }
        // Handle single char operators
        else if (lexeme = source_code.substr(0, 1); OPERATOR_LEXEMES.contains(lexeme))
        {
            token_stream.emplace_back(
                token(LUT_STRING_VALUE_TO_TOKEN.at(lexeme), lexeme, cur_line, cur_col));
        }
        else
        {
            throw std::runtime_error(
                std::string("Invalid token_type ") + std::string(next_word)
                + std::string(" at line ") + std::to_string(cur_line)
                + std::string(", column ") + std::to_string(cur_col));
        }

        std::advance(cur_char, next_word.size());
        skip_whitespace();
    }

    return token_stream;
}

size_t try_tokenize_keyword()
{
    return 0;
}
size_t try_tokenize_keyword_function()
{
    return 0;
}
