NAME = web_serve

_HEADER = libs.hpp webserve.hpp

HEADERS = $(addprefix includes,$(_HEADER))

CC = c++

CFLAGS =  -Wall -Wextra -Werror

SUBDIRS = srcs/server

_SRC = Kque.cpp Listner.cpp Socket.cpp
SRC = $(addprefix $(SUBDIRS)/,$(_SRC)) 


OBJDIR = obj
_OBJS = $(SRC:.cpp=.o)
OBJS = $(addprefix $(OBJDIR)/,$(_OBJS))

all: $(NAME)

$(OBJDIR)/%.o : %.cpp
	if [ ! -d $(OBJDIR)]; then \
		mkdir -p $(OBJDIR); \
	fi;
	for dir in $(SUBDIRS); do \
		if [ ! -d $(OBJDIR)/$$dir ]; then \
			mkdir -p $(OBJDIR)/$$dir; \
		fi; \
	done
	$(CC) $(CFLAGS) -c $^ -o $@

$(NAME): $(OBJS)
	# $(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re