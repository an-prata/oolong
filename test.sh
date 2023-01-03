#!/usr/bin/bash

SOURCE_FILES=$(find ./ -name "*.c")
mkdir bin -p
gcc $SOURCE_FILES -o bin/test
./bin/test
rm bin/test

