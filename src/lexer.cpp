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

#include "lexeme.hpp"
#include "token.hpp"
lexer::lexer(const std::string& source_code) : source_code{source_code} {}


void lexer::skip_whitespace()
{
    auto n_whitespace_chars = std::distance(
        cur_char, std::find_if_not(cur_char, end(source_code), [](char c) {
            return c == ' ' || c == '\t' || c == '\v' || c == '\f';
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
std::vector<lexeme> lexer::lex()
{
    skip_whitespace();

    while (cur_char < end(source_code))
    {
        skip_whitespace();

        if (std::string_view(cur_char, end(source_code)).starts_with("function"))
        {
            token_stream.emplace_back(
                lexeme(token::FUNCTION, "function", cur_line, cur_col));
        }
        std::advance(cur_char, 8);
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
