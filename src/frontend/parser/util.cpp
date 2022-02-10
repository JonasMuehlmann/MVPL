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

#include "frontend/parser/util.hpp"

#include <iostream>


//****************************************************************************//
//                              Helper functions                              //
//****************************************************************************//
std::span<token>& get_token_stream(parse_result& result)
{
    return std::get<0>(std::get<parse_content>(result));
}

std::unique_ptr<ast_node_t>& get_node(parse_result& result)
{
    return std::get<1>(std::get<parse_content>(result));
}

parse_error get_parse_error(parse_result& result)
{
    return std::get<parse_error>(result);
}

source_location get_source_location_from_compound(std::vector<parse_result>& nodes)
{
    auto first_node = get_node(nodes.front()).get();
    auto last_node  = get_node(nodes.back()).get();

    auto start_location = std::visit(source_location_retriever_visitor{}, *first_node);
    auto end_location   = std::visit(source_location_retriever_visitor{}, *last_node);

    return {start_location.line_start,
            start_location.col_start,
            end_location.line_end,
            end_location.col_end};
}

source_location get_source_location_from_compound(
    std::vector<parse_result>::iterator begin, std::vector<parse_result>::iterator end)
{
    auto first_node = get_node(*begin).get();
    auto last_node  = get_node(*end).get();

    auto start_location = std::visit(source_location_retriever_visitor{}, *first_node);
    auto end_location   = std::visit(source_location_retriever_visitor{}, *last_node);

    return {start_location.line_start,
            start_location.col_start,
            end_location.line_end,
            end_location.col_end};
}

bool try_add_parse_result(parse_result&&             cur_result,
                          std::vector<parse_result>& results,
                          std::span<token>&          ts,
                          bool                       overwrite_errors)
{
    if (std::holds_alternative<parse_error>(cur_result))
    {
        if (std::ranges::none_of(results, [](auto& result) {
                return std::holds_alternative<parse_error>(result);
            }))
        {
            results.push_back(std::move(cur_result));
        }
        else
        {
            auto existing_error = std::ranges::find_if(results, [](auto& cur_result) {
                return std::holds_alternative<parse_error>(cur_result);
            });
            *existing_error     = std::move(cur_result);
        }
        return false;
    }


    if (overwrite_errors)
    {
        std::erase_if(results, [](auto& cur_result) {
            return std::holds_alternative<parse_error>(cur_result);
        });
    }

    results.push_back(std::move(cur_result));

    ts = get_token_stream(results.back());

    return true;
}

bool try_add_parse_result(parse_result&&    cur_result,
                          parse_result&     result,
                          std::span<token>& ts,
                          bool              overwrite_errors)
{
    result = std::move(cur_result);

    if (std::holds_alternative<parse_content>(result))
    {
        ts = get_token_stream(result);

        return true;
    }
    return false;
}

bool try_add_parse_result(std::vector<parse_result>&& cur_results,
                          std::vector<parse_result>&  results,
                          std::span<token>&           ts,
                          bool                        overwrite_errors)
{
    if (std::ranges::all_of(cur_results, [](auto& cur_result) {
            return std::holds_alternative<parse_content>(cur_result);
        }))
    {
        std::ranges::move(cur_results, std::back_inserter(results));

        if (overwrite_errors)
        {
            std::erase_if(results, [](auto& cur_result) {
                return std::holds_alternative<parse_error>(cur_result);
            });
        }
        ts = get_token_stream(results.back());

        return true;
    }

    if (std::ranges::none_of(results, [](auto& cur_result) {
            return std::holds_alternative<parse_error>(cur_result);
        }))
    {
        results.push_back(
            std::move(*std::ranges::find_if(cur_results, [](auto& cur_result) {
                return std::holds_alternative<parse_error>(cur_result);
            })));
    }

    return false;
}

bool is_any_parse_result_valid(std::vector<parse_result>& results)
{
    return std::ranges::any_of(results, [](auto& parse_result_) {
        return std::holds_alternative<parse_content>(parse_result_);
    });
}

bool are_all_parse_results_valid(std::vector<parse_result>& results)
{
    return std::ranges::all_of(results, [](auto& parse_result_) {
        return std::holds_alternative<parse_content>(parse_result_);
    });
}


void log_parse_attempt(std::string_view parsed_structure, std::string_view next_lexeme)
{
    std::cerr << "Attempting to parse " << parsed_structure << " at lexeme "
              << next_lexeme << '\n';
}
void log_parse_attempt(std::string_view parsed_structure)
{
    std::cerr << "Attempting to parse " << parsed_structure << '\n';
}


void log_parse_error(std::string_view parsed_structure,
                     std::string_view next_lexeme,
                     std::string_view wanted_lexeme)
{
    std::cerr << "Failed to parse " << parsed_structure << " at lexeme " << next_lexeme
              << " when expecting " << wanted_lexeme << '\n';
}
void log_parse_error(std::string_view parsed_structure, std::string_view next_lexeme)
{
    std::cerr << "Failed to parse " << parsed_structure << " at lexeme " << next_lexeme
              << '\n';
}
void log_parse_error(std::string_view parsed_structure)
{
    std::cerr << "Failed to parse " << parsed_structure << '\n';
}


void log_parse_success(std::string_view parsed_structure, std::string_view next_lexeme)
{
    std::cerr << "Successfully parsed " << parsed_structure << " at lexeme "
              << next_lexeme << '\n';
}
void log_parse_success(std::string_view parsed_structure)
{
    std::cerr << "Successfully parsed " << parsed_structure << '\n';
}
