#!/bin/bash
function build {
    cd unit
    cmake -S . -B build
    cmake --build build
}

if ! build ; then
    echo Build failed!
fi
