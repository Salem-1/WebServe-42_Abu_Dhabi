#!/bin/bash

c++  -Wall -Wextra -Werror \
    ../../logs/logs.cpp  main.cpp ../../srcs/server/Socket.cpp \
    ../../srcs/server/Listner.cpp \
    ../../srcs/server/Kque.cpp \
    ../../srcs/client/Client.cpp \
    ../../srcs/recive_request/Receive.cpp \
    ../../srcs/recive_request/Parser.cpp \
    ../../srcs/GET/GET.cpp -o test.exe \
    ../../srcs/parsing/parsing_lib.cpp -o test.exe \
 && ./test.exe && \
sleep 1 && \
echo && \
echo Checking leaks && \i
echo && \
echo && \
valgrind ./test.exe
