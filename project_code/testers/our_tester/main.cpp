#include "../../includes/webserve.hpp"



void    try_kqueue(int server_socket);
int main()
{
    Listner binded_sock("3490");





    //-------------------------------------TESTING_ONLY--------------//
    // int                          new_fd;
    // struct  sockaddr_storage    client_addr; //client address information
    // socklen_t                   sin_size;
    // char                        request[500];
    // char                        s[100];


    printf("server waiting for connection....\n");
    Kque socket_manager(binded_sock.sockfd);
    socket_manager.watch_fds();
    // while (1)
    // {
    //     sin_size = sizeof(client_addr);
    //     new_fd = accept(binded_sock.sockfd, (struct sockaddr *)&client_addr, &sin_size);
    //     if (new_fd == -1)
    //     {
    //         perror("accept");
    //         continue;
    //     }
    //     inet_ntop(client_addr.ss_family,  get_in_addr((struct sockaddr *)&client_addr), s, sizeof (s));
    //     recv(new_fd, request, 500, 0);
    //     printf("server: got connection from %s\n", request);
    //     if (send(new_fd, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\nHello from our server!", 87, 0) == -1)
    //         perror("send failed");
    //     close(new_fd);
    // }
    // return (0);
}

void    try_kqueue(int server_socket)
{
    int         max_events = 100;
    int                 n;
    int                 kq;
    int                 fd;
    int                 filter;
    int                 client_socket;
    struct kevent       event;
    struct kevent       events[100];
    char                buffer[800];
    ssize_t             bytes_read;
    struct sockaddr_in  client_address;
    ssize_t             bytes_sent;
    socklen_t           client_address_len;
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\nHello from our server!";
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
        n = kevent(kq, NULL, 0, events, max_events, NULL);
        if (n == -1)
        {
            perror("kevent failed");
            exit(1);
        }

        for (int i = 0; i < n; i++)
        {
            fd  = events[i].ident;
            filter = events[i].filter;
            (void)filter;
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
                bytes_read = recv(fd, buffer, 870
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
                    bytes_sent = send(fd, hello.c_str(), 87, 0);
                    if (bytes_sent < 0)
                        perror("sent failed");
                }
            }
        }
    }
        close(server_socket);
}