#ifndef CONNECTINGSOCKET_HPP
# define CONNECTINGSOCKET_HPP
# include "SimpleSocket.hpp"

namespace ft
{
    class ConnectingSocket: public SimpleSocket
    {
        public:
            ConnectingSocket(int domain,
                            int service, int protocol,
                            int port, u_long interface)
                        :SimpleSocket(
                            domain, service,
                            protocol, port, interface);
            int connect_to_network_as_bind_or_connect(int socke, 
                                struct sockaddr_in address) ;
    };
};
#endif