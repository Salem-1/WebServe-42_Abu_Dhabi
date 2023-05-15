#ifndef BINDINGSOCKET_HPP
# define BINDINGSOCKET_HPP
# include "SimpleSocket.hpp"

namespace ft
{
    class BindingSocket: public SimpleSocket
    {
        public:
            BindingSocket(int domain,
                            int service, int protocol,
                            int port, u_long interface);
            int connect_to_network_as_bind_or_connect(int socke, 
                                struct sockaddr_in address) ;
    };
};
#endif