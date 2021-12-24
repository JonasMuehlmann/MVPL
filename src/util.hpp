#ifndef SRC_UTIL_HPP
#define SRC_UTIL_HPP
#include <string>
#include <vector>

#include "lexeme.hpp"
#include "token.hpp"
std::string token_to_str(const token t);
token       str_to_token(const std::string& str);
void        print_token_stream(const std::vector<lexeme>& token_stream);
#endif    // SRC_UTIL_HPP // SRC_UTIL_HPP
