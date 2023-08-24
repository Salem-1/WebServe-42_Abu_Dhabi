
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

struct ciLessLibC : public std::binary_function<std::string, std::string, bool> {
    bool operator()(const std::string &lhs, const std::string &rhs) const {
        return strcasecmp(lhs.c_str(), rhs.c_str()) < 0 ;
    }
};

typedef std::vector<std::pair<std::string, std::vector<std::string> > > tokenized_conf;
typedef std::map<std::string, std::vector<std::string>, ciLessLibC>	    packet_map;
typedef std::vector<std::map<std::string, std::string> >			    conf; 
typedef std::map<std::string, std::string>							    stringmap;
typedef std::map<std::string, std::vector<std::string> >			    response_packet;
typedef std::map<std::string, std::vector<std::string> >			    response_type;
# define BACKLOG 500
# define MAX_EVENTS 500
# define READ_BUFFER_SIZE 260000
# define SEND_BUFFER_SIZE 260000
# define PERSISTANCE 75
# define TIME_PER_SEC 10000
# define TIME_PER_MILLI_SEC 10
# define HEADER_MAX_LENGTH 8000
# define URL_MAX 8000
# define MAX_BODY_SIZE 100000000000000    
# define MAX_BODY_SIZE_STR "100000000000000" 
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


typedef struct locations_args
{
    std::vector<std::string>    location_options;
    std::vector<std::string>    tmp_directive;
    std::string                 path;
    std::set<std::string>       no_repeate_arg;
    std::set<std::string>       cgi_essential;
    
} locations_args;

std::vector<std::string>    split(std::string str, std::string delimeter);
std::string                 getTimeBuffer();
void                        run_server(char **env, char *conf_file);
void                        run_default(char **env);
void                        printAddrInfo( struct addrinfo *ai);
void                        visualize_servers(
                                std::vector<std::map<std::string, std::string> > servers);
void                        visualize_string_map(std::map<std::string, std::string>  &map);
void                        visualize_string_vector(std::vector<std::string> &vec, std::string vec_name);
void                        vis_str(std::string packet, std::string message);
void                        print_to_file(std::string, std::string);
bool                        inSet(std::set<std::string> data_set, std::string item);
bool                        inMap(stringmap data_set, std::string item);
bool                        inVector(std::vector<std::string> data_set, std::string item);
void                        fillEnvPath(conf &servers, char **env);
void                        print_error(std::string msg);
void                        handle_pipes(int sig);
void                        visualize_tokens(tokenized_conf &tokens);
//---------------------------------------------



#endif