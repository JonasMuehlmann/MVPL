cmake_minimum_required(VERSION 3.19)
#project(MVPL_tests CXX)
project(MVPL_tests C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#****************************************************************************#
#                                  Variables                                 #
#****************************************************************************#
execute_process(COMMAND nproc --all OUTPUT_VARIABLE N_CORES)

set({CMAKE_BUILD_PARALLEL_LEVEL} N_CORES)

message(STATUS "Using ${N_CORES} cores.")

#****************************************************************************#
#                                  Dependencies                              #
#****************************************************************************#

find_package(Threads REQUIRED)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

#****************************************************************************#
#                                Configuration                               #
#****************************************************************************#

include_directories(${MVPL_include_dirs})
add_executable(MVPL_tests frontend/parser/parser_tests.cpp)
target_compile_options(MVPL_tests PRIVATE ${MVPL_compile_flags})
target_link_options(MVPL_tests PRIVATE  ${MVPL_compile_flags})
target_link_libraries(MVPL_tests PUBLIC Threads::Threads gtest gtest_main MVPL_lib)
