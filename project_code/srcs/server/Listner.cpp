#include "Listner.hpp"



Listner::Listner(conf  server): Socket(server)
{
    while (bind(sockfd, try_addr->ai_addr, try_addr->ai_addrlen) == -1)
    {
        close(sockfd);
        perror("server: Listner");
        try_open_socket_again(try_addr->ai_next);
    }
    std::cout << "SOCKET binded " << std::endl;
    printAddrInfo(try_addr);
    freeaddrinfo(res);
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen error");
        exit(1);
    }
}

Listner::~Listner()
{

}

int Listner::get_sockfd()
{
    return (sockfd);
}