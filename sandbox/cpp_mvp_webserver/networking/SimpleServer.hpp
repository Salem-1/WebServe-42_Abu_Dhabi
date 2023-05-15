#ifndef SIMPLESERVER_HPP
# define SIMPLESERVER_HPP
#include "../ftlibc.hpp"

namespace ft
{
    class SimpleServer
    {
        public:
            SimpleServer(int domain,
                        int service, int protocol,
                        int port, u_long interface, int bklog);
            ListeningSocket * get_socket();
            struct sockaddr_in yalla_address();
            int yalla_sock();
        private:
            ListeningSocket *socket;
            virtual void    accepter() = 0;
            virtual void    handler() = 0;
            virtual void    responder() = 0;
            virtual void    launch() = 0;
    };
};
#endif