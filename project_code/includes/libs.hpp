/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:15 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/06 11:20:47 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LIBS_HPP
# define LIBS_HPP

# include <iostream>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fstream>
# include <string>
# include <string.h>
# include <sstream>
# include <dirent.h>
# include <algorithm>
# include <map>
# include <vector>
# include <set>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <netdb.h>
# include <fcntl.h>
# include <ctime>
# include <errno.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <signal.h>
#ifdef __APPLE__
# include <sys/event.h>
#endif
#ifdef __linux__
# include <event2/event.h>
# include <kqueue/sys/event.h>
#include <event.h>
#endif
// # include<thread>
// # include <chrono>

typedef std::vector<std::map<std::string, std::string> >	conf; 
typedef std::map<std::string, std::string>					stringmap;
typedef std::map<std::string, std::vector<std::string> >	packet_map;
typedef std::map<std::string, std::vector<std::string> >	response_packet;
typedef std::map<std::string, std::vector<std::string> >	response_type;

# define BACKLOG 256
# define MAX_EVENTS 256
# define BUFFER_SIZE 26000
# define PERSISTANCE 75
# define TIME_PER_SEC 10000
# define TIME_PER_MILLI_SEC 10
# define HEADER_MAX_LENGTH 8000
# define MAX_BODY_SIZE 10485760 // 10 MB Will be part of the configuration file
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

typedef struct s_body 
{
	std::string		header;
	std::string		body;
	size_t			body_content_length;
	int				request_is_valid;
}				t_request;


enum connection_state
{
    KILL_CONNECTION = 0,
    KEEP_ALIVE = 1
};

std::string					getTimeBuffer();
std::vector<std::string>	split(std::string str, std::string delimeter);
void						printAddrInfo( struct addrinfo *ai);
void						visualize_servers(
					std::vector<std::map<std::string, std::string> > servers);
void						visualize_string_map(std::map<std::string, std::string>  &map);
void						vis_str(std::string packet, std::string message);
//---------------------------------------------







#endif