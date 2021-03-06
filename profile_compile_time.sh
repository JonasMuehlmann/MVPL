#!/usr/bin/env sh

trace_dir="cmake-build-debug/traces/"
output_file="clang_build_analyzer.txt"

touch "${output_file}"

rm cmake-build-debug/CMakeCache.txt

export MVPL_PROFILE_COMPILE_TIME=1
./build_main.sh

fd cpp.json cmake-build-debug/src | xargs cp -t "${trace_dir}"

ClangBuildAnalyzer --all "${trace_dir}" "${output_file}"
ClangBuildAnalyzer --analyze "${output_file}"
