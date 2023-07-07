# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/06 17:54:02 by ayassin           #+#    #+#              #
#    Updated: 2023/07/07 16:29:09 by ayassin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = web_serve

HEADER = includes/libs.hpp includs/webserve.hpp

CC = c++

CFLAGS =  -Wall -Wextra -Werror

SUBDIRS = srcs logs includes srcs/server srcs/client srcs/recive_request srcs/GET \
	srcs/DELETE srcs/response srcs/configuration srcs/parsing

_SRC = Kque.cpp Listner.cpp Socket.cpp Client.cpp Parser.cpp \
		logs.cpp Receive.cpp Parse_headers.cpp \
		Parser_utils.cpp GET.cpp GET_response.cpp DELETE.cpp DELETE_res.cpp \
		Respond.cpp Config.cpp logger.cpp parsing_lib.cpp

MAIN = main.cpp

SRC = 
$(foreach dir,$(SUBDIRS), \
	$(foreach file,$(_SRC), \
		$(if $(shell test -e "$(dir)/$(file)" && echo hi), \
			$(eval SRC += $(dir)/$(file)), \
			\
		) \
	) \
)
SRC += $(MAIN)
OBJDIR = obj
_OBJS = $(SRC:.cpp=.o)
OBJS = $(addprefix $(OBJDIR)/,$(_OBJS))

all: $(NAME)

$(OBJDIR)/%.o : %.cpp
	@if [ ! -d $(OBJDIR) ]; then \
		mkdir -p $(OBJDIR); \
	fi;
	@for dir in $(SUBDIRS); do \
		if [ ! -d $(OBJDIR)/$$dir ]; then \
			mkdir -p $(OBJDIR)/$$dir; \
		fi; \
	done
	$(CC) $(CFLAGS) -c $^ -o $@

$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re

