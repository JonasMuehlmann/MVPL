#!/usr/bin/sh
cmake -S . -B cmake-build-debug && cd cmake-build-debug/benchmarks && make && ./MVPL_benchmarks
