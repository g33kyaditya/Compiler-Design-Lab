#!/bin/bash

flex $1
gcc lex.yy.c -lfl

if [ -z "$3" ]
then
    ./a.out < $2
else
    ./a.out < $2 > $3
fi
