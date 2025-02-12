#!/bin/bash

c++  -Wall -Wextra -Werror \
    ../../logs/logs.cpp  test_driven.cpp ../../srcs/server/Socket.cpp \
    ../../srcs/server/Listner.cpp \
    ../../srcs/server/Kque.cpp \
    ../../srcs/client/Client.cpp \
    ../../srcs/recive_request/Receive.cpp \
    ../../srcs/recive_request/Parser.cpp \
    ../../srcs/recive_request/Parse_headers.cpp  \
    ../../srcs/recive_request/Parser_utils.cpp  \
    ../../srcs/GET/GET.cpp \
    ../../srcs/GET/GET_response.cpp  \
    ../../srcs/DELETE/DELETE.cpp \
    ../../srcs/DELETE/DELETE_res.cpp \
    ../../srcs/response/Respond.cpp\
       ../../srcs/response/ErrResponse.cpp\
    ../../srcs/configuration/Config.cpp \
    ../../includes/logger.cpp \
    ../../srcs/parsing/parsing_lib.cpp -o driven  && ./driven  

