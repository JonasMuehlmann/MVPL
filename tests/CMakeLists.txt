cmake_minimum_required(VERSION 3.19)
#project(MVPL_tests CXX)
project(MVPL_tests C CXX)

set(CMAKE_CXX_STANDARD 20)

execute_process(COMMAND nproc --all OUTPUT_VARIABLE N_CORES)

set({CMAKE_BUILD_PARALLEL_LEVEL} N_CORES)

message(STATUS "Using ${N_CORES} cores.")

add_compile_options(-g
                    -O0
                    -Wall
                    -Wextra
                    -Wpedantic

                    -fsanitize=address
                    -fsanitize=pointer-compare
                    -fsanitize=pointer-subtract
                    -fsanitize=leak
                    -fsanitize=undefined
                    -fsanitize=float-divide-by-zero
                    -fsanitize=float-cast-overflow
                    -fstack-protector-strong
                    -fstack-clash-protection

                    -fno-omit-frame-pointer
                    -Wnull-dereference
                    -Wstack-protector)

add_link_options(-fsanitize=address
                 -fsanitize=pointer-compare
                 -fsanitize=pointer-subtract
                 -fsanitize=leak
                 -fsanitize=undefined
                 -fsanitize=float-cast-overflow

                 -fstack-protector-strong
                 -fno-omit-frame-pointer
                 )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(Threads REQUIRED)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(../src
                    ../src/frontend
                    ../src/frontend/lexer/
                    ../src/frontend/parser
                    ../src/frontend/parser/ast_operations

                    ../src/backend
                    ../src/backend/code_generator
                    ../src/backend/interpreter

                    ../src/common
                    )

add_executable(MVPL_tests frontend/parser/parser_tests.cpp)

target_link_libraries(MVPL_tests PUBLIC Threads::Threads gtest gtest_main MVPL_lib)
