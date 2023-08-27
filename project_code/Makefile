# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/06 17:54:02 by ayassin           #+#    #+#              #
#    Updated: 2023/08/27 15:20:45 by ayassin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++

CFLAGS =  -Wall -Wextra -Werror -std=c++98 -MMD -MP

SUBDIRS = srcs logs includes srcs/server srcs/client srcs/recive_request srcs/GET \
	srcs/DELETE srcs/response srcs/configuration srcs/parsing srcs/POST srcs/PUT \
	srcs/ChildCgi
	
_SRC = Kque.cpp Listner.cpp Socket.cpp Client.cpp Parser.cpp \
		logs.cpp Receive.cpp Parse_headers.cpp \
		Parser_utils.cpp GET.cpp GET_response.cpp DELETE.cpp DELETE_res.cpp \
		Respond.cpp Config.cpp logger.cpp parsing_lib.cpp ErrResponse.cpp main.cpp Post.cpp \
		CgiResponse.cpp PostCgiResponse.cpp GET_response_strings.cpp response_utils.cpp PUT.cpp \
		ChildExec.cpp  ConfigHandler.cpp ServerFill.cpp serverRunners.cpp
		
SRC = 
$(foreach dir,$(SUBDIRS), \
	$(foreach file,$(_SRC), \
		$(if $(shell test -e "$(dir)/$(file)" && echo hi), \
			$(eval SRC += $(dir)/$(file)), \
			\
		) \
	) \
)
OBJDIR = obj
_OBJS = $(SRC:.cpp=.o)
OBJS = $(addprefix $(OBJDIR)/,$(_OBJS))


# INCS = $(DEPS)
DEPS = $(OBJS:.o=.d)
ifeq ($(shell uname), Linux)
	INCS += -levent -lkqueue
endif
INC_DIRS = -Iincludes


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
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(INC_DIRS)

run : $(NAME)
	./$(NAME)
	
-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re

