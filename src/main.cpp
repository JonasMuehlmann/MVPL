#include <fstream>
#include <streambuf>
#include <string>

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        return 1;
    }
    std::ifstream source_stream(argv[1]);
    std::string   source_code((std::istreambuf_iterator<char>(source_stream)),
                            std::istreambuf_iterator<char>());
    return 0;
}
