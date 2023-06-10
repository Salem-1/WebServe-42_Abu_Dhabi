#include "Listner.hpp"



Listner::Listner(std::string PORT): Socket(PORT)
{
    while (bind(sockfd, try_addr->ai_addr, try_addr->ai_addrlen) == -1)
    {
        close(sockfd);
        perror("server: Listner");
        try_open_socket_again(try_addr->ai_next);
    }
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