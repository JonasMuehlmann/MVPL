#include <fstream>
#include <iostream>
#include <ostream>
#include <streambuf>
#include <string>

#include "lexeme.hpp"
#include "lexer.hpp"
#include "util.hpp"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        return 1;
    }
    std::ifstream source_stream(argv[1]);
    std::string   source_code((std::istreambuf_iterator<char>(source_stream)),
                            std::istreambuf_iterator<char>());

    std::vector<lexeme> token_stream = lexer::build_token_stream(source_code);

    print_token_stream(token_stream);
    return 0;
}
