cmake_minimum_required(VERSION 3.19)
project(MVPL_lib CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fstack-protector-strong -fstack-clash-protection -Wnull-dereference -Wstack-protector)

add_link_options(-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=float-cast-overflow -fstack-protector-strong)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include(FetchContent)
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.10.4)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

include_directories(frontend/ frontend/lexer/ frontend/parser/ frontend/parser/ast_operations/ backend backend/code_generator/ backend/interpreter/ common/)
set(SOURCE_FILES
        )
add_library(MVPL_lib STATIC ${SOURCE_FILES} backend/code_generator/code_generator.cpp backend/code_generator/code_generator.hpp frontend/lexer/lexer.cpp frontend/lexer/lexer.hpp frontend/parser/parser.hpp backend/interpreter/register_machine.cpp backend/interpreter/register_machine.hpp backend/opcode.hpp frontend/lexer/token_type.hpp backend/interpreter/builtin_functions.cpp backend/interpreter/builtin_functions.hpp backend/instruction.cpp backend/instruction.hpp frontend/lexer/token.hpp common/util.cpp common/util.hpp backend/trap.hpp common/enum_range.hpp frontend/parser/ast_node_type.hpp frontend/parser/ast_node.hpp frontend/parser/ast_node.cpp frontend/parser/state.hpp frontend/parser/finite_state_machine.cpp frontend/parser/finite_state_machine.hpp frontend/parser/operator_type.hpp frontend/parser/ast_operations/retrieve_source_location.hpp)

target_link_libraries(MVPL_lib PRIVATE nlohmann_json::nlohmann_json)
