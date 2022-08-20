#!/bin/bash

./parse $1 | ./exec $2
echo "exit code: $?"
