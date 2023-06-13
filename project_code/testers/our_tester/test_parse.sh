#!/bin/bash

c++  -Wall -Wextra -Werror \
    parse.cpp \
    ../../srcs/parsing/parsing_lib.cpp -o test.exe \
 && ./test.exe && \
sleep 1 && \
echo && \
echo Checking leaks && \i
echo && \
echo && \
valgrind ./test.exe
