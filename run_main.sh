#!/usr/bin/sh

file="$(realpath data/main.mvpl)"

cd cmake-build-debug && ./MVPL "${file}"
