#ifndef KQUEUE_SERVER_HPP
# define KQUEUE_SERVER_HPP
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/event.h>
# include <netinet/in.h>
# include <fcntl.h>
# define MAX_EVENTS 10
# define BUFFER_SIZE 1024

typedef struct server_vars 
{
    int                 server_socket;
    int                 client_socket;
    int                 kq;
    int                 n;
    int                 fd;
    int                 filter;
    struct kevent       event;
    struct kevent       events[MAX_EVENTS];
    struct sockaddr_in  server_address;
    struct sockaddr_in  client_address;
    socklen_t client_address_len;
}   server_vars;

#endif
