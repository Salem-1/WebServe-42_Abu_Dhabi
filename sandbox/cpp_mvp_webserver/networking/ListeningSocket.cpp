#include "ListeningSocket.hpp"



ft::ListeningSocket::ListeningSocket(int domain,
                            int service, int protocol,
                            int port, u_long interface, int backlog): BindingSocket(domain, service,
                            protocol, port, interface)
    {
        this->backlog = backlog;
        start_listening();
        test_connection(listening);
    };
                        
void    ft::ListeningSocket::start_listening()
{
    listening = listen(get_sock(), backlog);
}