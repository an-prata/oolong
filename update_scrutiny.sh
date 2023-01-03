#!/usr/bin/bash

git clone https://github.com/an-prata/scrutiny
mkdir tests/include/ -p

PREVIOUS_VERSION_FILES=$(ls -1 tests/include/)

for FILE in $PREVIOUS_VERSION_FILES; do
    mv tests/include/$FILE tests/include/$FILE.backup
done

mv scrutiny/scrutiny/scrutiny* tests/include/
rm -rf scrutiny

