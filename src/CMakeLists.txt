cmake_minimum_required(VERSION 3.19)
project(MVPL_lib CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#****************************************************************************#
#                                  Variables                                 #
#****************************************************************************#
execute_process(COMMAND nproc --all OUTPUT_VARIABLE N_CORES)

set({CMAKE_BUILD_PARALLEL_LEVEL} N_CORES)

message(STATUS "Using ${N_CORES} cores.")

set(SOURCE_FILES
    backend/code_generator/code_generator.cpp
    backend/code_generator/code_generator.hpp

    backend/interpreter/register_machine.cpp
    backend/interpreter/register_machine.hpp

    backend/interpreter/builtin_functions.cpp
    backend/interpreter/builtin_functions.hpp

    backend/instruction.cpp
    backend/instruction.hpp

    backend/trap.hpp

    backend/opcode.hpp



    frontend/lexer/lexer.cpp
    frontend/lexer/lexer.hpp

    frontend/lexer/token_type.hpp

    frontend/lexer/token.hpp



    frontend/parser/parser.hpp

    frontend/parser/ast_node_type.hpp

    frontend/parser/ast_node.hpp
    frontend/parser/ast_node.cpp

    frontend/parser/operator_type.hpp
    
    frontend/parser/parse_error.hpp
    frontend/parser/parse_error.cpp
    
    frontend/parser/util.hpp
    frontend/parser/util.cpp
    
    frontend/parser/ast_operations/retrieve_source_location.hpp
    frontend/parser/ast_operations/list_nodes.hpp



    frontend/semantic_analysis/scope_node.hpp

    frontend/semantic_analysis/symbol.hpp

    frontend/semantic_analysis/symbol_table.hpp
    frontend/semantic_analysis/symbol_table.cpp



    common/util.cpp
    common/util.hpp

    common/enum_range.hpp
    )

#****************************************************************************#
#                                  Dependencies                              #
#****************************************************************************#

include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    GIT_TAG v3.10.5
)

FetchContent_MakeAvailable(json)

#****************************************************************************#
#                                Configuration                               #
#****************************************************************************#

add_library(MVPL_lib STATIC ${SOURCE_FILES})

include_directories(${MVPL_include_dirs})
target_link_libraries(MVPL_lib PRIVATE nlohmann_json::nlohmann_json)
target_compile_options(MVPL_lib PRIVATE ${MVPL_compile_flags})
target_link_options(MVPL_lib PRIVATE  ${MVPL_compile_flags})
