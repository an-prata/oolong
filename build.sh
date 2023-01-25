#!/usr/bin/bash

# script assumes that the basename of the current directory (repository root)
# is the same name as the folder containing the projects source files.

PROJECT_NAME="$(basename $(pwd))"
SOURCE_FILES=$(find "./$PROJECT_NAME/" -name "*.c")
HEADER_FILES=$(find "./$PROJECT_NAME/" -name "*.h")
HEADER_LIST=""

for HEADER in $HEADER_FILES; do
    HEADER_LIST="$HEADER_LIST $HEADER"
done

mkdir build -p

for FILE in $SOURCE_FILES; do
    gcc -c $FILE -o "build/$(basename $FILE).o"

    if [ "$?" != "0" ]; then
        exit 1
    fi
done

tar --create --file build/oolong_headers.tar $HEADER_LIST 

OBJECT_FILES=$(find "./build/" -name "*.o" -printf "%p ")

ar rcs "build/$PROJECT_NAME.a" $OBJECT_FILES

if [ "$?" != "0" ]; then
    exit 1
fi

