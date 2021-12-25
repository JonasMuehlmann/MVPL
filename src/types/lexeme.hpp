#ifndef SRC_TYPES_LEXEME_HPP
#define SRC_TYPES_LEXEME_HPP

#include <string>
#include <string_view>

#include "token.hpp"
struct lexeme
{
    token            token;
    std::string_view source;
    size_t           line;
    size_t           col;
};
#endif    // SRC_TYPES_LEXEME_HPP
