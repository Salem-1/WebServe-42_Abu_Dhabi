#ifndef LIBS_HPP
#define LIBS_HPP

# include <iostream>
# include <string>
# include <map>
# include <set>
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
# include <ctime>
#include <fstream>
#include "../srcs/parsing/parsing_lib.hpp"
# define BACKLOG 256
# define MAX_EVENTS 256
# define BUFFER_SIZE 1000240
# define PERSISTANCE 75
# define TIME_PER_SEC 10000
# define TIME_PER_MILLI_SEC 10
# define HEADER_MAX_LENGTH 8000

# define DEFAULT_PATH "/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/website"

std::vector<std::string> split(std::string str, std::string delimeter);
void printAddrInfo( struct addrinfo *ai);
enum connection_state
{
    KILL_CONNECTION = 0,
    KEEP_ALIVE = 1
};

#endif