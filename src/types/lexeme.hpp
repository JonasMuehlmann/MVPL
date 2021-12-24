#ifndef SRC_TYPES_LEXEME_HPP
#define SRC_TYPES_LEXEME_HPP

#include <string>

#include "token.hpp"
struct lexeme
{
    token       token;
    std::string value;
};
#endif    // SRC_TYPES_LEXEME_HPP
