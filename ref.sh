#!/bin/bash
set -x

gcc $1
./a.out
echo "Return code $?"
rm -f a.out
