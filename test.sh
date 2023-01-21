#!/usr/bin/bash

SOURCE_FILES=$(find ./ -name "*.c")
mkdir build -p
gcc $SOURCE_FILES -o build/test
./build/test
rm build/test

