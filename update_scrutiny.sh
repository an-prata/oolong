#!/usr/bin/bash

git clone https://github.com/an-prata/scrutiny
mkdir tests/include/ -p
rm tests/include/*
mv scrutiny/scrutiny/scrutiny* tests/include/
rm -rf scrutiny

