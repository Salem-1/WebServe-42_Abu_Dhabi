
#include "kqueue_server.hpp"

int set_nonblocking(int fd);

int main()
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
    char                buffer[BUFFER_SIZE];
    ssize_t             bytes_read;
    ssize_t           bytes_sent;
    socklen_t client_address_len;
       char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 26\n\nHello from Ahmed's server!";

    client_address_len =sizeof(client_address);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("socket error");
        return (-1);
    }
    if (set_nonblocking(server_socket) < 0)
    {
        write(2, "nonblocking error\n", 20);
        exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_address
            , sizeof(server_address)) < 0)
    {
        perror("Bind failed!");
        return (-1);
    }
    if(listen(server_socket, SOMAXCONN)< 0)
    {
        perror("listen failed");
        exit(1);
    }

    kq = kqueue();
    if (kq == -1)
    {
        perror("kqueue");
        exit(1);
    }

    EV_SET(&event, server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event, 1, NULL, 0, NULL) == -1)
    {
        perror("kevent");
        exit(1);
    }
    while (1)
    {
        n = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (n == -1)
        {
            perror("kevent failed");
            exit(1);
        }

        for (int i = 0; i < n; i++)
        {
            fd  = events[i].ident;
            filter = events[i].filter;

            //is there new client connection
            if (fd == server_socket)
            {
                client_socket = accept(server_socket, 
                (struct sockaddr*)&client_address, &client_address_len);
                if(client_socket < 0)
                {
                    perror("accept failed");
                    continue ;
                }

                //adding client to the kqueue will watch this 
                //fd for events
                EV_SET(&event, client_socket, EVFILT_READ
                    ,EV_ADD, 0, 0, NULL);
                if (kevent(kq, &event, 1, NULL, 0, NULL) < 0)
                {
                    perror("kevent client failed");
                    exit(1);
                }
            }
            else
            {
                //it's a client socket event
                bytes_read = recv(fd, buffer, BUFFER_SIZE
                    , 0);
                if (bytes_read == 0)
                {
                    //client disconnected
                    EV_SET(&event, client_socket, 
                        EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    if(kevent(kq, &event, 1, NULL, 0, NULL) == -1)
                    {
                        perror("kevent failed in disconencting client");
                        exit(1);
                    }
                    close(fd);
                }
                else
                {
                    //handle the client request
                    printf("recieved data from client");
                    printf("%.*s/n", (int)bytes_read, buffer);
                    
                    //ECHO responding back to the client
                    bytes_sent = send(fd, hello, 91, 0);
                    if (bytes_sent < 0)
                        perror("sent failed");
                }
            }
        }
    }
        close(server_socket);
        return (0);
}


int set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl flags error");
        return (-1);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("fcntl error");
        return (-1);
    }
    return (0);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/event.h>

#define BUFFER_SIZE 1024

int create_server_socket(int port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating server socket");
        exit(1);
    }

    int optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("Error setting socket options");
        exit(1);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Error binding server socket");
        exit(1);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Error listening on server socket");
        exit(1);
    }

    return server_socket;
}

void handle_request(int client_socket) {
    char request_buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(client_socket, request_buffer, sizeof(request_buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("Error receiving request");
        exit(1);
    }

    // Process the request and generate a response
    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    if (send(client_socket, response, strlen(response), 0) < 0) {
        perror("Error sending response");
        exit(1);
    }

    close(client_socket);
}

int main() {
    int host_port = 8000;

    int server_socket = create_server_socket(host_port);

    int kq = kqueue();
    if (kq < 0) {
        perror("Error creating kqueue");
        exit(1);
    }

    struct kevent kev;
    EV_SET(&kev, server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &kev, 1, NULL, 0, NULL) < 0) {
        perror("Error registering server socket event");
        exit(1);
    }

    struct kevent events[10];
    while (1) {
        int ready_fds = kevent(kq, NULL, 0, events, sizeof(events) / sizeof(events[0]), NULL);
        if (ready_fds < 0) {
            perror("Error waiting for events");
            exit(1);
        }

        for (int i = 0; i < ready_fds; ++i) {
            if (events[i].ident == server_socket) 
            {
                if (events[i].filter == EVFILT_READ) 
                {
                    struct sockaddr_in client_address;
                    socklen_t client_address_len = sizeof(client_address);
                    int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
                    if (client_socket < 0) {
                        perror("Error accepting client connection");
                        exit(1);
                    }

                    EV_SET(&kev, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                    if (kevent(kq, &kev, 1, NULL, 0, NULL) < 0) {
                        perror("Error registering client socket event");
                        exit(1);
                    }
                }
            } else {
                if (events[i].filter == EVFILT_READ) {
                    handle_request(events[i].ident);
                }
            }
        }
    }

    return 0;
}
