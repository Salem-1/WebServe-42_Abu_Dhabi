#ifndef LIBS_HPP
#define LIBS_HPP

# include <iostream>
# include <string>
# include <map>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# define BACKLOG 256
# define MAX_EVENTS 256
void printAddrInfo( struct addrinfo *ai);

#endif