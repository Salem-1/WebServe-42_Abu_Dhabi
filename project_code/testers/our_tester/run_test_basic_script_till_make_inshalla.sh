#!/bin/bash

c++  -Wall -Wextra -Werror \
    ../../logs/logs.cpp  main.cpp ../../srcs/server/Socket.cpp \
    ../../srcs/server/Listner.cpp \
    ../../srcs/server/Kque.cpp \
    -o test.exe && ./test.exe && \
sleep 1 && \
echo && \
echo Checking leaks && \
echo && \
echo && \
valgrind ./test.exe
