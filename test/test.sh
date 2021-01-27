#!/bin/bash

./slow5tools f2s test/data/raw/chr22_meth_example-subset-multi > /dev/null || exit 1

gcc -Wall test/endian_test.c -o test/endian_test && test/endian_test

gcc -Wall -g test/unit_test.c -o test/unit_test src/slow5.c src/misc.c src/slow5idx_clean.c src/press.c -I src/ -lz && echo "compiled"
valgrind test/unit_test && echo "success"
