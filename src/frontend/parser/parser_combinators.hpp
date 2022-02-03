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
#include <span>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "ast_node.hpp"
#include "frontend/lexer/token.hpp"
#include "frontend/parser/ast_node.hpp"
#include "frontend/parser/ast_operations/retrieve_source_location.hpp"
#include "frontend/parser/util.hpp"
#include "nlohmann/json.hpp"
#include "parser.hpp"
#include "token_type.hpp"

namespace combinators
{
template <typename Parser>
struct expect;

template <typename Expector, typename Parser>
struct if_expect;

template <typename Parser>
struct optional;

template <typename... Parsers>
struct any;

template <typename Parser>
struct many;

template <typename OpeningParser, typename ClosingParser, typename... InnerParsers>
struct surrounded;

template <typename SeparatorParser, typename ItemParser>
struct separated;

template <typename... Parsers>
struct all;


template <typename Parser>
struct optional
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results{};


        if (!ts.empty() && try_add_parse_result(Parser::parse(ts), results, ts))
        {
            return results;
        }

        // If an error occurred after consuming at least one token,
        // there is no missing optional, there is an error, so we don't reset the token
        // stream
        // FIX: Well, this prevents showing wrong error locations,
        // but swallows errors and produces bad ASTs
        // The old approach might have been closer to the solution
        // TODO: A missing optional node should contain the errors it encountered
        // TODO: If a parser after a missing optional fails,
        // the missing optional's error should be returned
        // if (get_parse_error(results.back()).token_ != ts[0])
        // {
        // auto dist = std::distance(
        //     ts.begin(),
        //     std::ranges::find(ts, get_parse_error(results.back()).token_));

        // ts = ts.subspan(dist);
        // }
        parse_result new_node = parse_content{
            ts,
            std::make_unique<ast_node_t>(std::in_place_type<missing_optional_node>)};

        results.clear();
        results.push_back(std::move(new_node));

        return results;
    }
};
template <typename... Parsers>
struct any
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results;

        (try_add_parse_result(Parsers::parse(ts), results, ts, true) || ...);

        return results;
    }
};
template <typename Parser>
struct many
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results;

        results.reserve(10);
        if (!try_add_parse_result(Parser::parse(ts), results, ts))
        {
            return results;
        }


        while (!ts.empty() && try_add_parse_result(Parser::parse(ts), results, ts))
        {}

        // When the loop stops, it always adds a parse error!
        if (std::holds_alternative<parse_error>(results.back()))
        {
            results.erase(results.end());
        }
        return results;
    }
};
template <typename... Parsers>
struct all
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results;
        results.reserve(sizeof...(Parsers));


        (try_add_parse_result(Parsers::parse(ts), results, ts) && ...);


        return results;
    }
};
template <typename SeparatorParser, typename ItemParser>
struct separated
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results;
        results.reserve(10);


        if (!try_add_parse_result(
                combinators::many<combinators::all<ItemParser, SeparatorParser>>::parse(
                    ts),
                results,
                ts))
        {
            return results;
        }

        // NOTE: The previous block always fails to parse the last item,
        // because it expects trailing commas, hence we parse the last item manually
        try_add_parse_result(ItemParser::parse(ts), results, ts);

        return results;
    }
};
template <typename OpeningParser, typename ClosingParser, typename... InnerParsers>
struct surrounded
{
    static std::vector<parse_result> parse(std::span<token> ts)
    {
        std::vector<parse_result> results;
        // 2 stands for the two surrounder parsers
        results.reserve(2 + sizeof...(InnerParsers));


        if (!try_add_parse_result(OpeningParser::parse(ts), results, ts))
        {
            return results;
        }


        if (!(try_add_parse_result(InnerParsers::parse(ts), results, ts) && ...))
        {
            return results;
        }

        try_add_parse_result(ClosingParser::parse(ts), results, ts);

        return results;
    }
};

};    // namespace combinators
