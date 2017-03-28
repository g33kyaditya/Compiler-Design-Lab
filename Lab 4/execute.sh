#!/bin/bash

g++ main.cpp -o main
./main

dot -Tpng dotFile -o NFA
gwenview ~/Compiler\ Design/Lab\ 4/NFA
