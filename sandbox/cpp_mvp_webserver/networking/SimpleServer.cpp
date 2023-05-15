#include "SimpleServer.hpp"

ft::SimpleServer::SimpleServer(int domain,
                        int service, int protocol,
                        int port, u_long interface, int bklog)
{
    std::cout << "allocating socket" << std::endl;
    ListeningSocket *socket = new ListeningSocket(domain,
                            service, protocol, port, interface, bklog);
    
    std::cout << "allocating socket" << std::endl;
}

struct sockaddr_in ft::SimpleServer::yalla_address()
{
    return socket->get_address();
}
int ft::SimpleServer::yalla_sock()
{
    return socket->get_sock();
}
ft::ListeningSocket *ft::SimpleServer::get_socket()
{
    std::cout << "getting socket" << std::endl;
    return (socket);
}