cmake_minimum_required(VERSION 3.19)
project(MVPL CXX)

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

include_directories(src
                    src/frontend
                    src/frontend/lexer/
                    src/frontend/parser
                    src/frontend/parser/ast_operations

                    src/backend
                    src/backend/code_generator
                    src/backend/interpreter

                    src/common
                    )

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)

add_executable(MVPL src/main.cpp)
target_link_libraries(MVPL PRIVATE MVPL_lib)
