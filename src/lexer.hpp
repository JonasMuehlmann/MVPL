#ifndef SRC_LEXER_HPP
#define SRC_LEXER_HPP
#include <string>
#include <vector>

#include "lexeme.hpp"
namespace lexer
{
    std::vector<lexeme> build_token_stream(std::string source_code);
}    // namespace lexer
#endif    // SRC_LEXER_HPP
