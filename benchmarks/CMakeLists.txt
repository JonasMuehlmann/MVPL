cmake_minimum_required(VERSION 3.19)
#project(MVPL_benchmarks CXX)
project(MVPL_benchmarks C CXX)

set(CMAKE_CXX_STANDARD 20)

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


include(FetchContent)
FetchContent_Declare(
        googlebenchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.6.0.zip
)
FetchContent_MakeAvailable(googlebenchmark)

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
add_executable(MVPL_benchmarks parser_benchmarks.cpp)

target_link_libraries(MVPL_benchmarks PUBLIC benchmark::benchmark MVPL_lib)
