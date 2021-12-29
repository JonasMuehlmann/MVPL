cmake_minimum_required(VERSION 3.19)
project(MVPL)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fstack-protector-strong -fstack-clash-protection -Wnull-dereference -Wstack-protector)
add_link_options(-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=float-cast-overflow -fstack-protector-strong)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(src src/frontend src/frontend/lexer/ src/frontend/parser src/backend src/backend/code_generator src/backend/interpreter src/common)
add_executable(MVPL src/backend/code_generator/code_generator.cpp src/backend/code_generator/code_generator.hpp src/frontend/lexer/lexer.cpp src/frontend/lexer/lexer.hpp src/main.cpp src/frontend/parser/parser.cpp src/frontend/parser/parser.hpp src/backend/interpreter/register_machine.cpp src/backend/interpreter/register_machine.hpp src/backend/opcode.hpp src/frontend/lexer/token_type.hpp src/backend/interpreter/builtin_functions.cpp src/backend/interpreter/builtin_functions.hpp src/backend/instruction.cpp src/backend/instruction.hpp src/frontend/lexer/token.hpp src/common/util.cpp src/common/util.hpp src/backend/trap.hpp src/common/enum_range.hpp src/frontend/parser/ast_node_type.hpp src/frontend/parser/ast_node.hpp src/frontend/parser/state.hpp src/frontend/parser/finite_state_machine.cpp src/frontend/parser/finite_state_machine.hpp)

