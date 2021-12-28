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
#include <iostream>
#include <iterator>
#include <ostream>
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
    return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_';
}

std::string_view lexer::peek_next_word()
{
    // FIX: This seems to be buggy, it substrings until the end of the file
    return source_code.substr(
        0,
        std::distance(begin(source_code),
                      std::ranges::find_if_not(source_code, is_word)));
}

void lexer::skip_whitespace()
{
    while (is_whitespace(source_code[0]) || source_code[0] == '\n')
    {
        auto n_whitespace_chars = std::distance(
            begin(source_code), std::ranges::find_if_not(source_code, is_whitespace));

        // FIX: This probably does not handle whitespaces after line breaks
        auto n_line_breaks = std::distance(
            begin(source_code),
            std::ranges::find_if_not(source_code, [](char c) { return c == '\n'; }));

        source_code.remove_prefix(n_whitespace_chars);
        cur_col += n_whitespace_chars;

        if (n_line_breaks > 0)
        {
            cur_col = 0;
            cur_line += n_line_breaks;
        }

        source_code.remove_prefix(n_line_breaks);
    }
}

void lexer::skip_block_comment()
{
    using namespace std::string_view_literals;

    auto block_comment_end =
        LUT_TOKEN_TO_STRING_VALUE.at(static_cast<size_t>(token_type::RBLOCKCOMMENT));

    size_t n_commented_out_chars = 0;
    auto   decision_point        = begin(source_code);
    auto   new_decision_point    = begin(source_code);

    source_code.remove_prefix(
        LUT_TOKEN_TO_STRING_VALUE.at(static_cast<size_t>(token_type::LBLOCKCOMMENT))
            .length());

    while (!source_code.starts_with(block_comment_end))
    {
        decision_point = std::ranges::find_if(source_code, [block_comment_end](char c) {
            return c == block_comment_end[0] || c == '\n';
        });

        if (decision_point == end(source_code)
            || decision_point == end(source_code) - 1)
        {
            throw std::runtime_error("Reached EOF before closing block comment");
        }

        // Found the first char of the end marker for the block comment
        if (*decision_point == block_comment_end[0])
        {
            new_decision_point = std::ranges::mismatch(
                                     std::string_view(decision_point, end(source_code)),
                                     block_comment_end)
                                     .in1;

            // Found the remaining chars of the end marker for the block comment
            if (static_cast<size_t>(std::distance(decision_point, new_decision_point))
                == block_comment_end.length())
            {
                decision_point = new_decision_point;

                n_commented_out_chars =
                    std::distance(begin(source_code), decision_point);
                source_code.remove_prefix(n_commented_out_chars);
                cur_col += n_commented_out_chars;

                return;
            }
        }
        else if (decision_point < end(source_code) - 1)
        {
            decision_point++;
            cur_col = 0;
            cur_line++;
        }

        n_commented_out_chars = std::distance(begin(source_code), decision_point);
        source_code.remove_prefix(n_commented_out_chars);
        cur_col += n_commented_out_chars;
    }
}

void lexer::skip_line_comment()
{
    auto comment_length =
        std::distance(begin(source_code), std::ranges::find(source_code, '\n'));

    cur_col += comment_length;
    source_code.remove_prefix(comment_length);
}

std::vector<token> lexer::lex()
{
    skip_whitespace();

    std::string_view next_lexeme;

    while (begin(source_code) < end(source_code))
    {
        skip_whitespace();

        next_lexeme = source_code.substr(0, 2);

        if (next_lexeme
            == LUT_TOKEN_TO_STRING_VALUE.at(
                static_cast<size_t>(token_type::LINECOMMENT)))
        {
            skip_line_comment();
            continue;
        }
        else if (next_lexeme
                 == LUT_TOKEN_TO_STRING_VALUE.at(
                     static_cast<size_t>(token_type::LBLOCKCOMMENT)))
        {
            skip_block_comment();
            continue;
        }

        next_lexeme = peek_next_word();

        // Handle keywords
        if (LUT_STRING_VALUE_TO_TOKEN.contains(next_lexeme))
        {
            token_stream.emplace_back(token(LUT_STRING_VALUE_TO_TOKEN.at(next_lexeme),
                                            next_lexeme,
                                            cur_line,
                                            cur_col));
        }
        // Handle literals
        else if ((next_lexeme.length() != 0U)
                 && std::ranges::all_of(next_lexeme, [](char c) {
                        return std::isdigit(static_cast<unsigned char>(c));
                    }))
        {
            token_stream.emplace_back(
                token(token_type::LITERAL, next_lexeme, cur_line, cur_col));
        }
        // Handle identifiers
        else if (next_lexeme.length() != 0U
                 && std::ranges::all_of(next_lexeme, is_word))
        {
            token_stream.emplace_back(
                token(token_type::IDENTIFIER, next_lexeme, cur_line, cur_col));
        }
        // Handle double char operators
        else if (next_lexeme = source_code.substr(0, 2);
                 OPERATOR_LEXEMES.contains(next_lexeme))
        {
            token_stream.emplace_back(token(LUT_STRING_VALUE_TO_TOKEN.at(next_lexeme),
                                            next_lexeme,
                                            cur_line,
                                            cur_col));
        }
        // Handle single char operators
        else if (next_lexeme = source_code.substr(0, 1);
                 OPERATOR_LEXEMES.contains(next_lexeme))
        {
            token_stream.emplace_back(token(LUT_STRING_VALUE_TO_TOKEN.at(next_lexeme),
                                            next_lexeme,
                                            cur_line,
                                            cur_col));
        }
        else
        {
            throw std::runtime_error(
                std::string("Invalid token_type ") + std::string(next_lexeme)
                + std::string(" at line ") + std::to_string(cur_line)
                + std::string(", column ") + std::to_string(cur_col));
        }

        source_code.remove_prefix(next_lexeme.size());
        cur_col += next_lexeme.size();
        skip_whitespace();
    }

    return token_stream;
}
