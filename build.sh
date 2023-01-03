#!/usr/bin/bash

# script assumes that the basename of the current directory (repository root)
# is the same name as the folder containing the projects source files.

PROJECT_NAME="$(basename $(pwd))"
SOURCE_FILES=$(find "./$PROJECT_NAME" -name "*.c")
mkdir bin -p
gcc -c $SOURCE_FILES -o "bin/$PROJECT_NAME"

