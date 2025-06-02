#!/bin/bash

shopt -s xpg_echo

if [ -d "build" ]; then
    rm -rf build
fi

PROJECT_NAME="qt-shared-memory"

echo "\nBuilding $PROJECT_NAME...\n"

# -S: Source directory, -B: Build directory
cmake -S . -B build

make -C ./build
