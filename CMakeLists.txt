cmake_minimum_required(VERSION 3.19)
project(MVPL CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#****************************************************************************#
#                                  Variables                                 #
#****************************************************************************#
execute_process(COMMAND nproc --all OUTPUT_VARIABLE N_CORES)

set({CMAKE_BUILD_PARALLEL_LEVEL} N_CORES)

message(STATUS "Using ${N_CORES} cores.")


set(MVPL_compile_flags
    -g
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
    -fno-optimize-sibling-calls

    -fno-omit-frame-pointer
    -Wnull-dereference
    -Wshadow
    -Wpointer-arith
    -Wswitch-default
    -Wswitch-enum
    -Wconversion
    -Wuninitialized
    -Weffc++
    -Wstack-protector
)

if (DEFINED ENV{MVPL_PROFILE_COMPILE_TIME})
    list(APPEND MVPL_compile_flags ${MVPL_compile_flags} -ftime-trace)
endif()


set(MVPL_link_flags
    -fsanitize=address
    -fsanitize=pointer-compare
    -fsanitize=pointer-subtract
    -fsanitize=leak
    -fsanitize=undefined
    -fsanitize=float-cast-overflow
    
    -fstack-protector-strong
    -fno-omit-frame-pointer
 )

set(MVPL_include_dirs "")

foreach(dir IN ITEMS
        src
        src/frontend
        src/frontend/lexer
        src/frontend/parser
        src/frontend/parser/ast_operations

        src/backend
        src/backend/code_generator
        src/backend/interpreter

        src/common
        # Yeah, this could be ${json_SOURCE_DIR}/single_include, 
        # but this var seems to be needed exactly here but is not defined here
        ${CMAKE_CURRENT_BINARY_DIR}/_deps/json-src/single_include
)
    get_filename_component(dir ${dir} ABSOLUTE)
    list(APPEND MVPL_include_dirs  ${dir})
endforeach()

#****************************************************************************#
#                                Configuration                               #
#****************************************************************************#

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)

include_directories(${MVPL_include_dirs})
add_executable(MVPL src/main.cpp)
target_link_libraries(MVPL PRIVATE MVPL_lib)
target_compile_options(MVPL PRIVATE ${MVPL_compile_flags})
target_link_options(MVPL PRIVATE  ${MVPL_compile_flags})
