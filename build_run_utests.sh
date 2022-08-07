#!/bin/bash
function build {
    cd unit
    cmake -S . -B build
    cmake --build build
}

if build ; then
    cd build && ctest --output-on-failure
else
    echo Build failed!
    exit -1
fi

