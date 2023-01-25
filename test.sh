#!/usr/bin/bash

./build.sh
SOURCE_FILES=$(find ./tests/ -name "*.c")
mkdir build -p
gcc $SOURCE_FILES ./build/oolong.a -o build/test
./build/test

if [ "$?" != "0" ]; then
    exit 1
fi

rm build/test

