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

#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <optional>
#include <retrieve_source_location.hpp>
#include <span>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "parse_error.hpp"

//****************************************************************************//
//                                    Types                                   //
//****************************************************************************//
template <typename TFunction>
concept Parser = requires(TFunction& function)
{
    {
        std::forward<TFunction>(function)()
        } -> std::convertible_to<bool>;
};

using parse_content = std::tuple<std::span<token>, std::unique_ptr<ast_node_t>>;
using parse_result  = std::variant<parse_content, parse_error>;

//****************************************************************************//
//                              Helper functions                              //
//****************************************************************************//
std::span<token>& get_token_stream(parse_result& result);

std::unique_ptr<ast_node_t>& get_node(parse_result& result);

parse_error get_parse_error(parse_result& result);

source_location get_source_location_from_compound(std::vector<parse_result>& nodes);

source_location get_source_location_from_compound(
    std::vector<parse_result>::iterator begin, std::vector<parse_result>::iterator end);

bool try_add_parse_result(parse_result&&             cur_result,
                          std::vector<parse_result>& results,
                          std::span<token>&          ts,
                          bool                       overwrite_errors = false);

bool try_add_parse_result(parse_result&&    cur_result,
                          parse_result&     result,
                          std::span<token>& ts,
                          bool              overwrite_errors = false);

bool try_add_parse_result(std::vector<parse_result>&& cur_results,
                          std::vector<parse_result>&  results,
                          std::span<token>&           ts,
                          bool                        overwrite_errors = false);

bool is_any_parse_result_valid(std::vector<parse_result>& results);
bool are_all_parse_results_valid(std::vector<parse_result>& results);

void log_parse_attempt(std::string_view parsed_structure, std::string_view next_lexeme);
void log_parse_attempt(std::string_view parsed_structure);


void log_parse_error(std::string_view parsed_structure,
                     std::string_view next_lexeme,
                     std::string_view wanted_lexeme);

void log_parse_error(std::string_view parsed_structure, std::string_view next_lexeme);
void log_parse_error(std::string_view parsed_structure);


void log_parse_success(std::string_view parsed_structure, std::string_view next_lexeme);
void log_parse_success(std::string_view parsed_structure);
