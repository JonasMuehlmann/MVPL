#!/usr/bin/sh

cmake -S . -B cmake-build-debug && cd cmake-build-debug/tests && make MVPL_tests
