#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP
# include "SimpleServer.hpp"
# include <unistd.h>
namespace ft
{
    class TestServer: public SimpleServer
    {
        public:
            TestServer();
            char    buffer[30000] = {0};
            int     new_socket;
            ListeningSocket *so;
            int addrlen;
            struct sockaddr_in address;
        private:
            void    accepter();
            void    handler();
            void    responder();
            void    launch();
    };
};
#endif