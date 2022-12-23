CMAKE_MINIMUM_REQUIRED(VERSION 3.19)

PROJECT(antlr CXX)
SET (CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find_package(antlr4-generator REQUIRED)
find_package(antlr4-runtime REQUIRED)

include_directories( ${ANTLR4_INCLUDE_DIR} ${ANTLR4_INCLUDE_DIR_antlrcpptest_lexer} ${ANTLR4_INCLUDE_DIR_antlrcpptest_parser} "out")
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/out/*.cpp ${PROJECT_SOURCE_DIR}/out/*.h)
add_executable(antlr parser.cpp ${ANTLR4_SRC_FILES_antlrcpptest_lexer} ${ANTLR4_SRC_FILES_antlrcpptest_parser} ${SRC_FILES})
add_dependencies(antlr antlr4_shared)
 target_link_libraries( antlr PRIVATE
                        antlr4_shared)
# target_link_directories(antlr PRIVATE"out")
