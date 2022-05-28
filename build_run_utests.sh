#!/bin/bash
cd unit
cmake -S . -B build
cmake --build build
cd build && ctest
