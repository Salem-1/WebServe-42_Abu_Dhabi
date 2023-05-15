#ifndef SIMPLESOCKET_HPP
# define SIMPLESOCKET_HPP
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
//domain  --> IP
//service --> socket type
//protocol -> TCP/IP UDP ..
namespace ft
{
    class SimpleSocket
    {
        private:
            struct sockaddr_in  address;
            int                 sock;
            int                 connection;
        public:
            SimpleSocket(
                int domain, int service, int protocol, int port, u_long interface);            
            ~SimpleSocket();
             int         connect_to_network_as_bind_or_connect(int socke, 
                                struct sockaddr_in address);
            struct sockaddr_in  get_address();
            void                test_connection(int);
            int                 get_sock();
            int                 get_connection();
            void                set_address(struct sockaddr_in  given_address);
            void                set_sock(int  input_socket);
            void                set_connection(int input_connection);
    };
};
#endif