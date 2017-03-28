#!/bin/bash

printf 'The test file, test.cpp is ...\n'
cd "Lab 1"
printf '\n\n'
cat test.cpp
printf '\n\n'

printf 'The file after removing all comments unnecessary spaces is shown below...\n\n\n'
./main

printf '\n\n\n'

cd ..
cd "Lab 2"
printf 'The Keywords found are shown below...\n\n\n'
./main

printf '\n\n\n'

cd ..
cd "Lab 3"
printf 'The Identifiers found are shown below...\n\n\n'
./main

printf '\n\n'
