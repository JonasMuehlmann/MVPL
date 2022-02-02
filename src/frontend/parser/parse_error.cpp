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
#include "frontend/parser/parse_error.hpp"

#include <string>

#include "common/source_location.hpp"

using json = nlohmann::ordered_json;
using namespace std::literals::string_view_literals;

parse_error::parse_error(std::string_view parsed_structure) :
    parsed_structure_(parsed_structure),
    token_(token_type::END_TOKEN, ""sv, source_location())
{}

parse_error::parse_error(std::string_view parsed_structure, token token_) :
    parsed_structure_(parsed_structure), token_(token_)
{}

void parse_error::throw_() const
{
    json j;
    to_json(j, token_);

    std::string message = "Could not parse ";

    message += parsed_structure_;
    message += " at token ";
    message += j.dump(4);

    throw std::runtime_error(message);
}
