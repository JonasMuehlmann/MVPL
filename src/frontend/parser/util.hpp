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
std::span<token>& get_token_stream(parse_result& result)
{
    return std::get<0>(std::get<parse_content>(result));
}

std::unique_ptr<ast_node_t>& get_node(parse_result& result)
{
    return std::get<1>(std::get<parse_content>(result));
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

bool try_add_parse_result(parse_result&&             cur_result,
                          std::vector<parse_result>& results,
                          std::span<token>&          ts,
                          bool                       overwrite_errors = false)
{
    if (std::holds_alternative<parse_error>(cur_result))
    {
        if (std::ranges::none_of(results, [](auto& result) {
                return std::holds_alternative<parse_error>(result);
            }))
        {
            results.push_back(std::move(cur_result));
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
                          bool              overwrite_errors = false)
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
                          bool                        overwrite_errors = false)
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
