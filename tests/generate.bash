#!/bin/bash

rm -rf fuzz*.c

for i in {1..100}
do
    rm -f a.out
    # generate
    ./cgen.py $i > "fuzz$i.c"
    # compile with gcc
    gcc "fuzz$i.c"
    # try to run it
    timeout 1 ./a.out > /dev/null
    code=$?
    echo "code=$code"
    # if its a timeout or segfault then delete it
    if [[ $code -eq 124 || $code -eq 139 ]]; then
        rm -f "fuzz$i.c"
    fi
done
