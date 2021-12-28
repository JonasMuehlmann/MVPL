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
#include "util.hpp"

#include <iostream>
#include <stdexcept>

#include "enum_range.hpp"
#include "token_type.hpp"


void print_token_stream(const std::vector<token>& token_stream)
{
    std::cout << "{\n\"token_stream\": [\n    ";

    for (const auto& token_ : token_stream)
    {
        std::cout << "{\"token_type\": \""
                  << LUT_TOKEN_TO_STRING[static_cast<size_t>(token_.type)]
                  << "\", \"value\": \"" << token_.value
                  << "\", \"line\":" << token_.line << ", \"col\": " << token_.col
                  << "}";


        if (&token_ != &token_stream.back())
        {
            std::cout << ",\n    ";
        }
    }
    std::cout << "\n]}";
}
