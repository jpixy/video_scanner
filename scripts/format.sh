#!/bin/bash

# Format all C++ files using clang-format
find src include tests -name "*.h" -o -name "*.cpp" | xargs clang-format -i -style=file