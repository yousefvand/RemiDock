#!/usr/bin/env bash

rm -rf build
cmake -S . -B build -G Ninja
cmake --build build
cp build/bin/RemiDock ../RemiDock
