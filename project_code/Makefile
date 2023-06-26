NAME = web_serve

HEADER = includes/libs.hpp includs/webserve.hpp

CC = c++

CFLAGS =  -Wall -Wextra -Werror

SUBDIRS = logs srcs srcs/server srcs/client srcs/recive_request

_SRC = Kque.cpp Listner.cpp Socket.cpp Client.cpp Parser.cpp Receive.cpp logs.cpp main.cpp


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