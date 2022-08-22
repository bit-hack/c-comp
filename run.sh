#!/bin/bash

./parse $1 > temp.bin
./exec temp.bin $2
echo "exit code: $?"
rm -f temp.bin
