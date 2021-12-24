#include "lexer.hpp"

#include "lexeme.hpp"
#include "token.hpp"
namespace lexer
{

    std::vector<lexeme> build_token_stream(const std::string& source_code)
    {
        return std::vector{lexeme{token::IDENTIFIER, "Foo"},
                           lexeme{token::LPAREN, ""},
                           lexeme{token::IDENTIFIER, "BAR"}};
    }
    namespace
    {}    // namespace
}    // namespace lexer
