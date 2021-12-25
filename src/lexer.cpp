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
                lexeme{token::FUNCTION, "function", cur_line, cur_col});
        }
        std::advance(cur_char, 8);
        skip_whitespace();
    }

    return token_stream;
}
