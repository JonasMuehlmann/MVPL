#ifndef SRC_TYPES_LEXEME_HPP
#define SRC_TYPES_LEXEME_HPP

#include <string>

#include "token.hpp"
struct lexeme
{
    std::string source;
    token       token;
    int         line;
    int         col;
};
#endif    // SRC_TYPES_LEXEME_HPP
