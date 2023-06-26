#include "../../includes/webserve.hpp"



void    try_kqueue(int server_socket);
int main()
{

    Listner binded_sock("3490");

    printf("server waiting for connection....\n");
    Kque socket_manager(binded_sock.sockfd);
    socket_manager.watch_fds();

    // return (0);
}