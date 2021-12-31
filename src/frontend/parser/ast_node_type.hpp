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
#ifndef SRC_FRONTEND_PARSER_AST_NODE_TYPE_HPP_
#define SRC_FRONTEND_PARSER_AST_NODE_TYPE_HPP_

#include <string_view>

#include "common/enum_range.hpp"

enum class ast_node_type
{
    // EXPRESSION,        // x + 1
    BINARY_OP,         // 1 + 1
    UNARY_OP,          // i--
    FUNC_DEF,          // function x(a,b,c){return 1;}
    PROCEDURE_DEF,     // procedure x(){}
    FUNC_SIGNATURE,    // function x(a,b,c)
    RETURN_STMT,       // return x;
    PARAMETER_DEF,     // a,b,c
    VAR_DEF,           // x
    VAR_INIT,          // let x = 5;
    VAR_ASSIGNMENT,    // x = 5;
    CALL,              // f(2, 5)
    PARAMETER_PASS,    // 2,5
    BLOCK,             // { x = 5; y = x;}//
    CONTROL_BLOCK,     // while(x){x--;}
    CONTROL_HEAD,      // while(x)
    PROGRAM            // Root node
};

const int NUM_AST_NODE_TYPES = []() {
    EnumRange<ast_node_type, ast_node_type::PROGRAM> range;

    return range.size();
}();

const std::array ALL_AST_NODE_TYPES =
    enum_to_array<ast_node_type, ast_node_type::PROGRAM>();

const auto LUT_AST_NODE_TYPE_TO_STRING = []() {
    using namespace std::literals::string_view_literals;

    constexpr auto arr = []() {
        std::array<std::string_view, NUM_AST_NODE_TYPES> arr{};

        arr.fill(""sv);

        // arr[static_cast<size_t>(ast_node_type::EXPRESSION)]     = "EXPRESSION"sv;
        arr[static_cast<size_t>(ast_node_type::BINARY_OP)]      = "BINARY_OP"sv;
        arr[static_cast<size_t>(ast_node_type::UNARY_OP)]       = "UNARY_OP"sv;
        arr[static_cast<size_t>(ast_node_type::FUNC_DEF)]       = "FUNC_DEF"sv;
        arr[static_cast<size_t>(ast_node_type::PROCEDURE_DEF)]  = "PROCEDURE_DEF"sv;
        arr[static_cast<size_t>(ast_node_type::FUNC_SIGNATURE)] = "FUNC_SIGNATURE"sv;
        arr[static_cast<size_t>(ast_node_type::RETURN_STMT)]    = "RETURN_STMT"sv;
        arr[static_cast<size_t>(ast_node_type::PARAMETER_DEF)]  = "PARAMETER_DEF"sv;
        arr[static_cast<size_t>(ast_node_type::VAR_DEF)]        = "VAR_DEF"sv;
        arr[static_cast<size_t>(ast_node_type::VAR_INIT)]       = "VAR_INIT"sv;
        arr[static_cast<size_t>(ast_node_type::VAR_ASSIGNMENT)] = "VAR_ASSIGNMENT"sv;
        arr[static_cast<size_t>(ast_node_type::CALL)]           = "CALL"sv;
        arr[static_cast<size_t>(ast_node_type::PARAMETER_PASS)] = "PARAMETER_PASS"sv;
        arr[static_cast<size_t>(ast_node_type::BLOCK)]          = "BLOCK"sv;
        arr[static_cast<size_t>(ast_node_type::CONTROL_BLOCK)]  = "CONTROL_BLOCK"sv;
        arr[static_cast<size_t>(ast_node_type::CONTROL_HEAD)]   = "CONTROL_HEAD"sv;
        arr[static_cast<size_t>(ast_node_type::PROGRAM)]        = "PROGRAM"sv;

        return arr;
    }();

    static_assert(
        std::ranges::count_if(arr, [](std::string_view str) { return str == ""sv; })
            == 0,
        "ast_node_type missing string representation");
    return arr;
}();
#endif    // SRC_FRONTEND_PARSER_AST_NODE_TYPE_HPP_
