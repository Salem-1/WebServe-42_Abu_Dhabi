#include "ConnectingSocket.hpp"

ft::ConnectingSocket::ConnectingSocket(int domain,
                int service, int protocol, int port, u_long interface):
                SimpleSocket(domain, service, protocol, port, interface)
{
    //Establishing network connection
    set_connection(connect_to_network_as_bind_or_connect(get_sock(), get_address()));
    test_connection(get_connection());
}

int ft::ConnectingSocket::connect_to_network_as_bind_or_connect(int sock, 
                                struct sockaddr_in address)
{
    return (connect(sock, (struct sockaddr*)&address, sizeof(address)));
}