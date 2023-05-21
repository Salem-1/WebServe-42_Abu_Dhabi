// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#define PORT 8080

    //  int
    //  poll(struct pollfd fds[], nfds_t nfds, int timeout);
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct pollfd fds[2];
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
   char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 26\n\nHello from Ahmed's server!";

    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))  < 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
 
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 2) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        int res = poll(fds, 1, -1);
        if (res < 0)
        {
            perror("poll failed");
            exit(EXIT_FAILURE);
        }
        else if (res == 0)
            printf("still waiting nothing changed");
        else
        {
            if (fds[0].revents & POLLIN)
            {
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
                {
                    perror("In accept");
                    exit(EXIT_FAILURE);
                }
                // fds[1].fd = new_socket;
                // fds[1].events = POLLIN;
                char buffer[30000] = {0};
                valread = read( new_socket , buffer, 30000);
                printf("%s\n",buffer );
                write(new_socket , hello , strlen(hello));
                printf("------------------Hello message sent-------------------\n");
                // sleep(5);
                close(new_socket);
            }
        }
    }
    return 0;
}