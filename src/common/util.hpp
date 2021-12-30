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
#ifndef SRC_COMMON_UTIL_HPP
#define SRC_COMMON_UTIL_HPP
#include <string>
#include <vector>

#include "frontend/lexer/token.hpp"
#include "frontend/lexer/token_type.hpp"
#include "frontend/parser/ast_node.hpp"
#include "source_location.hpp"

std::string token_to_str(const token_type t);
token_type  str_to_token(const std::string& str);
void        print_token_stream(const std::vector<token>& token_stream);
void        print_ast(const ast_node& ast);
void        print_source_location(const source_location& source_location,
                                  const size_t           nesting);
#endif    // SRC_COMMON_UTIL_HPP
