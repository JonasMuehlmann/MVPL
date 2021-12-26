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
#ifndef SRC_LEXER_HPP_
#define SRC_LEXER_HPP_
#include <array>
#include <functional>
#include <string_view>
#include <vector>

#include "lexeme.hpp"
class lexer
{
 public:
    // Methods
    explicit lexer(std::string_view source_code);
    std::vector<lexeme> lex();

 private:
    // Variables
    std::string_view                 source_code;
    std::vector<lexeme>              token_stream;
    std::string_view::const_iterator cur_char = begin(source_code);
    size_t                           cur_line = 0;
    size_t                           cur_col  = 0;
    // Methods
    static bool      is_whitespace(char c);
    std::string_view peek_next_word();
    void             skip_whitespace();
    size_t           try_tokenize_keyword();
    size_t           try_tokenize_keyword_function();
};
#endif    // SRC_LEXER_HPP_
