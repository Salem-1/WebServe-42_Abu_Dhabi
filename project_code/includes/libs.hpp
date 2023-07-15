/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:15 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/15 01:36:56 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBS_HPP
#define LIBS_HPP

# include <iostream>
# include <fstream>
# include <string>
// # include<thread>
# include <map>
# include <set>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <fcntl.h>
// # include <chrono>
# include <ctime>
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
#ifdef __APPLE__
# include <sys/event.h>
#endif
#ifdef __linux__
# include <event2/event.h>
# include <kqueue/sys/event.h>
#include <event.h>
#endif
# include <sys/time.h>
# include <fstream>
# include <cstdio>
# include "../srcs/parsing/parsing_lib.hpp"
# include <algorithm>
#include <math.h>
#include <map>

# define BACKLOG 256
# define MAX_EVENTS 256
# define BUFFER_SIZE 260000
# define PERSISTANCE 75
# define TIME_PER_SEC 10000
# define TIME_PER_MILLI_SEC 10
# define HEADER_MAX_LENGTH 8000
// # define PORT "3490"
# define DEFAULT_PATH "/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/website"
# define DEFAULT_LOCATION "index.html"
#define CRLF "\r\n"

enum LogLevel {
    server,
    request,
    response,
    client,
    FATAL
};
class Logger;

typedef std::vector<std::map<std::string, std::string> > conf; 
enum connection_state
{
    KILL_CONNECTION = 0,
    KEEP_ALIVE = 1
};

std::vector<std::string> split(std::string str, std::string delimeter);
void printAddrInfo( struct addrinfo *ai);
void    visualize_servers(
    std::vector<std::map<std::string, std::string> > servers);

void    visualize_string_map(std::map<std::string, std::string>  &map);

//---------------------------------------------







#endif