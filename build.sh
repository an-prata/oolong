#!/usr/bin/bash

# script assumes that the basename of the current directory (repository root)
# is the same name as the folder containing the projects source files.

PROJECT_NAME="$(basename $(pwd))"
SOURCE_FILES=$(find "./$PROJECT_NAME/" -name "*.c")
mkdir bin -p

for FILE in $SOURCE_FILES; do
    gcc -c $FILE -o "bin/$(basename $FILE).o"
done

OBJECT_FILES=$(find "./bin/" -name "*.o" -printf "%p ")

ar rcs "bin/$PROJECT_NAME.a" $OBJECT_FILES
