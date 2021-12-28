cmake_minimum_required(VERSION 3.19)
project(MVPL)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fstack-protector-strong -fstack-clash-protection -Wnull-dereference -Wstack-protector)
add_link_options(-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=float-cast-overflow -fstack-protector-strong)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(src src/types)
add_executable(MVPL src/code_generator.cpp  src/code_generator.hpp  src/lexer.cpp  src/lexer.hpp  src/main.cpp  src/parser.cpp  src/parser.hpp  src/register_machine.cpp  src/register_machine.hpp src/types/opcode.hpp src/types/token_type.hpp src/builtin_functions.cpp src/builtin_functions.hpp src/instruction.cpp src/instruction.hpp src/types/token.hpp src/util.cpp src/util.hpp src/types/trap.hpp src/types/enum_range.hpp src/typesast_node_type.hpp)

