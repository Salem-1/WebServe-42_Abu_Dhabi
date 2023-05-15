#include "SimpleSocket.hpp"

ft::SimpleSocket::SimpleSocket(
    int domain, int service, int protocol, int port, u_long interface)
{
    address.sin_family = domain;
    //change the bits of the port to allign with the actual network
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    //establishing connection
    sock = socket(domain, service, protocol);

}
ft::SimpleSocket::~SimpleSocket()
{

};

void    ft::SimpleSocket::test_connection(int test_item)
{
    if (test_item < 0)
    {
        perror("Failed tp connect.....");
        exit(EXIT_FAILURE);
    }
}
struct sockaddr_in  ft::SimpleSocket::get_address()
{
    return (address);
}
int                 ft::SimpleSocket::get_sock()
{
    return (sock);
}
int                 ft::SimpleSocket::get_connection()
{
    return (connection);
}
 void   ft::SimpleSocket::set_address(struct sockaddr_in  given_address)
{
    address = given_address;
}
 void    ft::SimpleSocket::set_sock(int  input_socket)
{
    input_socket = sock;
}
void    ft::SimpleSocket::set_connection(int input_connection)
{
    input_connection = connection;
}