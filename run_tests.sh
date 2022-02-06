#!/usr/bin/sh

test_filter=""

if [[ "$#" -eq 1 ]]; then
    test_filter="--gtest_filter=${1}"
fi

cd cmake-build-debug/tests && ./MVPL_tests "${test_filter}"
