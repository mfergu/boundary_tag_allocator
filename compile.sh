#!/bin/bash
make clean
clear
make &> compile_out.txt
echo -e "clang -Weverything -g -c -o temp_compile pfl1.c"
clang -Weverything -g -c -o temp_compile pfl2.c
