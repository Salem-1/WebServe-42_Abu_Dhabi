NAME = web_serve

HEADER = includes/libs.hpp

CC = c++

CFLAGS =  -Wall -Wextra -Werror -std=c++98

INCS =
ifeq ($(shell uname), Linux)
	INCS += -levent -lkqueue
endif

SUBDIRS = srcs srcs/request

_SRC = main.cpp RequestHandler.cpp Request.cpp

# 
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
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(INCS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re
