#!/usr/bin/bash

# script assumes that the basename of the current directory (repository root)
# is the same name as the folder containing the projects source files.

PROJECT_NAME="$(basename $(pwd))"
SOURCE_FILES=$(find "./$PROJECT_NAME/" -name "*.c")
mkdir build -p

for FILE in $SOURCE_FILES; do
    gcc -c $FILE -o "build/$(basename $FILE).o"
done

OBJECT_FILES=$(find "./build/" -name "*.o" -printf "%p ")

ar rcs "build/$PROJECT_NAME.a" $OBJECT_FILES
