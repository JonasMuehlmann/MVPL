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
#ifndef SRC_COMMON_MACROS_HPP_
#define SRC_COMMON_MACROS_HPP_

#include "nlohmann/json.hpp"
// From https://github.com/nlohmann/json/discussions/2397#discussioncomment-680069
/*!
@brief macro added to work with ordered_json -- hutch
@def NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED
*/
#define NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(Type, ...)                    \
    inline void to_json(nlohmann::ordered_json& nlohmann_json_j,                   \
                        const Type&             nlohmann_json_t)                   \
    {                                                                              \
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__))   \
    }                                                                              \
    inline void from_json(const nlohmann::ordered_json& nlohmann_json_j,           \
                          Type&                         nlohmann_json_t)           \
    {                                                                              \
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) \
    }

#endif    // SRC_COMMON_MACROS_HPP_
