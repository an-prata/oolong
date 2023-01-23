#!/usr/bin/bash

./build.sh
SOURCE_FILES=$(find ./tests/ -name "*.c")
mkdir build -p
gcc $SOURCE_FILES ./build/oolong.a -o build/test
./build/test
rm build/test

