#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>

int main()
{
    fd_set current_socket, ready_socket;
    int sock = socket(AF_INET, SOCK_STREAM,  0);
    char buffer[2000];
    int ac_fd = 0;
    if (sock < 0)
        return (1);
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080); 

    int new_sock = bind(sock, (struct sockaddr *)&address, sizeof(address));
    if (new_sock < 0)
        return (2);
    if (listen(sock, 10) < 0)
        return (3);
    
    FD_ZERO(&current_socket);
    FD_ZERO(&ready_socket);
    FD_SET(sock, &current_socket);
    while (1)
    {
        ready_socket = current_socket;
        if (select(sock + 1, &ready_socket, NULL, NULL, NULL) < 0)
            return (-1);
        printf("Server Listening\n");
   
        if (FD_ISSET(sock, &ready_socket))
        {
            ac_fd = accept(sock, (struct sockaddr *)&address, (socklen_t *)(&addrlen));
            if (ac_fd < 0)
                return (4);   
            FD_SET(ac_fd, &current_socket);     
           read(ac_fd, buffer, 2000);
            printf("%s\n", buffer);
            write(ac_fd, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 26\n\nHello from Ahmed's server!",  200);
            FD_CLR(ac_fd, &current_socket);
            close (ac_fd);   
        }
        printf("Message recieved\n");
    }
}