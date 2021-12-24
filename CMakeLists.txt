cmake_minimum_required(VERSION 3.19)
project(MVPL)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fstack-protector-strong -fstack-clash-protection -Wnull-dereference -Wstack-protector)
add_link_options(-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=float-cast-overflow -fstack-protector-strong)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(MVPL src/code_generator.cpp  src/code_generator.hpp  src/lexer.cpp  src/lexer.hpp  src/main.cpp  src/parser.cpp  src/parser.hpp  src/register_machine.cpp  src/register_machine.hpp src/types/opcodes.hpp src/types/tokens.hpp src/builtin_functions.cpp src/builtin_functions.hpp src/instruction.cpp src/instruction.hpp)

