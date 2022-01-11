cmake_minimum_required(VERSION 3.19)
#project(MVPL_benchmarks CXX)
project(MVPL_benchmarks C CXX)

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

include(FetchContent)
FetchContent_Declare(
        googlebenchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.6.0.zip
)
FetchContent_MakeAvailable(googlebenchmark)

#****************************************************************************#
#                                Configuration                               #
#****************************************************************************#

include_directories(${MVPL_include_dirs})
add_executable(MVPL_benchmarks parser_benchmarks.cpp)
target_compile_options(MVPL_benchmarks PRIVATE ${MVPL_compile_flags})
target_link_options(MVPL_benchmarks PRIVATE  ${MVPL_compile_flags})
target_link_libraries(MVPL_benchmarks PUBLIC benchmark::benchmark MVPL_lib)
