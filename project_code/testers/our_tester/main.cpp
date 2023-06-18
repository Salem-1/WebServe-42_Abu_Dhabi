#include "../../includes/webserve.hpp"


int main()
{
    Config  servers;

    visualize_servers(servers.servers);
    Listner binded_sock(servers.servers[0]);

    printf("server waiting for connection....\n");
    Kque socket_manager(binded_sock.sockfd);
    socket_manager.watch_fds(servers.servers[0]);

    return (0);
}
