# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jonas/CLionProjects/MVPL/antlr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jonas/CLionProjects/MVPL/antlr/build

# Include any dependencies generated for this target.
include CMakeFiles/antlr.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/antlr.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/antlr.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/antlr.dir/flags.make

CMakeFiles/antlr.dir/parser.cpp.o: CMakeFiles/antlr.dir/flags.make
CMakeFiles/antlr.dir/parser.cpp.o: /home/jonas/CLionProjects/MVPL/antlr/parser.cpp
CMakeFiles/antlr.dir/parser.cpp.o: CMakeFiles/antlr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jonas/CLionProjects/MVPL/antlr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/antlr.dir/parser.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/antlr.dir/parser.cpp.o -MF CMakeFiles/antlr.dir/parser.cpp.o.d -o CMakeFiles/antlr.dir/parser.cpp.o -c /home/jonas/CLionProjects/MVPL/antlr/parser.cpp

CMakeFiles/antlr.dir/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/antlr.dir/parser.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jonas/CLionProjects/MVPL/antlr/parser.cpp > CMakeFiles/antlr.dir/parser.cpp.i

CMakeFiles/antlr.dir/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/antlr.dir/parser.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jonas/CLionProjects/MVPL/antlr/parser.cpp -o CMakeFiles/antlr.dir/parser.cpp.s

CMakeFiles/antlr.dir/out/exprBaseListener.cpp.o: CMakeFiles/antlr.dir/flags.make
CMakeFiles/antlr.dir/out/exprBaseListener.cpp.o: /home/jonas/CLionProjects/MVPL/antlr/out/exprBaseListener.cpp
CMakeFiles/antlr.dir/out/exprBaseListener.cpp.o: CMakeFiles/antlr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jonas/CLionProjects/MVPL/antlr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/antlr.dir/out/exprBaseListener.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/antlr.dir/out/exprBaseListener.cpp.o -MF CMakeFiles/antlr.dir/out/exprBaseListener.cpp.o.d -o CMakeFiles/antlr.dir/out/exprBaseListener.cpp.o -c /home/jonas/CLionProjects/MVPL/antlr/out/exprBaseListener.cpp

CMakeFiles/antlr.dir/out/exprBaseListener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/antlr.dir/out/exprBaseListener.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jonas/CLionProjects/MVPL/antlr/out/exprBaseListener.cpp > CMakeFiles/antlr.dir/out/exprBaseListener.cpp.i

CMakeFiles/antlr.dir/out/exprBaseListener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/antlr.dir/out/exprBaseListener.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jonas/CLionProjects/MVPL/antlr/out/exprBaseListener.cpp -o CMakeFiles/antlr.dir/out/exprBaseListener.cpp.s

CMakeFiles/antlr.dir/out/exprLexer.cpp.o: CMakeFiles/antlr.dir/flags.make
CMakeFiles/antlr.dir/out/exprLexer.cpp.o: /home/jonas/CLionProjects/MVPL/antlr/out/exprLexer.cpp
CMakeFiles/antlr.dir/out/exprLexer.cpp.o: CMakeFiles/antlr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jonas/CLionProjects/MVPL/antlr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/antlr.dir/out/exprLexer.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/antlr.dir/out/exprLexer.cpp.o -MF CMakeFiles/antlr.dir/out/exprLexer.cpp.o.d -o CMakeFiles/antlr.dir/out/exprLexer.cpp.o -c /home/jonas/CLionProjects/MVPL/antlr/out/exprLexer.cpp

CMakeFiles/antlr.dir/out/exprLexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/antlr.dir/out/exprLexer.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jonas/CLionProjects/MVPL/antlr/out/exprLexer.cpp > CMakeFiles/antlr.dir/out/exprLexer.cpp.i

CMakeFiles/antlr.dir/out/exprLexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/antlr.dir/out/exprLexer.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jonas/CLionProjects/MVPL/antlr/out/exprLexer.cpp -o CMakeFiles/antlr.dir/out/exprLexer.cpp.s

CMakeFiles/antlr.dir/out/exprListener.cpp.o: CMakeFiles/antlr.dir/flags.make
CMakeFiles/antlr.dir/out/exprListener.cpp.o: /home/jonas/CLionProjects/MVPL/antlr/out/exprListener.cpp
CMakeFiles/antlr.dir/out/exprListener.cpp.o: CMakeFiles/antlr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jonas/CLionProjects/MVPL/antlr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/antlr.dir/out/exprListener.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/antlr.dir/out/exprListener.cpp.o -MF CMakeFiles/antlr.dir/out/exprListener.cpp.o.d -o CMakeFiles/antlr.dir/out/exprListener.cpp.o -c /home/jonas/CLionProjects/MVPL/antlr/out/exprListener.cpp

CMakeFiles/antlr.dir/out/exprListener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/antlr.dir/out/exprListener.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jonas/CLionProjects/MVPL/antlr/out/exprListener.cpp > CMakeFiles/antlr.dir/out/exprListener.cpp.i

CMakeFiles/antlr.dir/out/exprListener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/antlr.dir/out/exprListener.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jonas/CLionProjects/MVPL/antlr/out/exprListener.cpp -o CMakeFiles/antlr.dir/out/exprListener.cpp.s

CMakeFiles/antlr.dir/out/exprParser.cpp.o: CMakeFiles/antlr.dir/flags.make
CMakeFiles/antlr.dir/out/exprParser.cpp.o: /home/jonas/CLionProjects/MVPL/antlr/out/exprParser.cpp
CMakeFiles/antlr.dir/out/exprParser.cpp.o: CMakeFiles/antlr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jonas/CLionProjects/MVPL/antlr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/antlr.dir/out/exprParser.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/antlr.dir/out/exprParser.cpp.o -MF CMakeFiles/antlr.dir/out/exprParser.cpp.o.d -o CMakeFiles/antlr.dir/out/exprParser.cpp.o -c /home/jonas/CLionProjects/MVPL/antlr/out/exprParser.cpp

CMakeFiles/antlr.dir/out/exprParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/antlr.dir/out/exprParser.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jonas/CLionProjects/MVPL/antlr/out/exprParser.cpp > CMakeFiles/antlr.dir/out/exprParser.cpp.i

CMakeFiles/antlr.dir/out/exprParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/antlr.dir/out/exprParser.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jonas/CLionProjects/MVPL/antlr/out/exprParser.cpp -o CMakeFiles/antlr.dir/out/exprParser.cpp.s

# Object files for target antlr
antlr_OBJECTS = \
"CMakeFiles/antlr.dir/parser.cpp.o" \
"CMakeFiles/antlr.dir/out/exprBaseListener.cpp.o" \
"CMakeFiles/antlr.dir/out/exprLexer.cpp.o" \
"CMakeFiles/antlr.dir/out/exprListener.cpp.o" \
"CMakeFiles/antlr.dir/out/exprParser.cpp.o"

# External object files for target antlr
antlr_EXTERNAL_OBJECTS =

antlr: CMakeFiles/antlr.dir/parser.cpp.o
antlr: CMakeFiles/antlr.dir/out/exprBaseListener.cpp.o
antlr: CMakeFiles/antlr.dir/out/exprLexer.cpp.o
antlr: CMakeFiles/antlr.dir/out/exprListener.cpp.o
antlr: CMakeFiles/antlr.dir/out/exprParser.cpp.o
antlr: CMakeFiles/antlr.dir/build.make
antlr: /usr/lib/libantlr4-runtime.so.4.11.1
antlr: CMakeFiles/antlr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jonas/CLionProjects/MVPL/antlr/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable antlr"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/antlr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/antlr.dir/build: antlr
.PHONY : CMakeFiles/antlr.dir/build

CMakeFiles/antlr.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/antlr.dir/cmake_clean.cmake
.PHONY : CMakeFiles/antlr.dir/clean

CMakeFiles/antlr.dir/depend:
	cd /home/jonas/CLionProjects/MVPL/antlr/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jonas/CLionProjects/MVPL/antlr /home/jonas/CLionProjects/MVPL/antlr /home/jonas/CLionProjects/MVPL/antlr/build /home/jonas/CLionProjects/MVPL/antlr/build /home/jonas/CLionProjects/MVPL/antlr/build/CMakeFiles/antlr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/antlr.dir/depend

