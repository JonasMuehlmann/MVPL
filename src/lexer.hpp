#ifndef SRC_LEXER_HPP
#define SRC_LEXER_HPP
#include <array>
#include <functional>
#include <string>
#include <vector>

#include "lexeme.hpp"
class lexer
{
public:
    // Methods
    explicit lexer(const std::string& source_code);
    std::vector<lexeme> lex();

private:
    // Variables
    const std::string&          source_code;
    std::vector<lexeme>         token_stream;
    std::string::const_iterator cur_char = begin(source_code);
    size_t                      cur_line = 0;
    size_t                      cur_col  = 0;
    // Methods
    void skip_whitespace();
};
#endif    // SRC_LEXER_HPP
