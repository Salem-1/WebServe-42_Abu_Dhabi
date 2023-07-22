/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:15 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/22 21:25:44 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LIBS_HPP
# define LIBS_HPP

# include <iostream>
# include <fstream>
# include <string>
#include <dirent.h>
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
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

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
void    vis_str(std::string packet, std::string message);
//---------------------------------------------







#endif