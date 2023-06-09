#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "../../includes/libs.hpp"

class Socket
{
    public:
        Socket(std::string PORT);
        Socket( struct  addrinfo *try_another_info);
        virtual ~Socket();
        struct  addrinfo    hints;
        struct  addrinfo    *res;
        struct  addrinfo    *try_addr;
        std::string         servername;
        void                get_my_addinfo(void);
        void                open_socket(struct  addrinfo    *try_addr);
        void                try_open_socket_again( struct  addrinfo *try_another_info);
        void                non_block_sock(void);
        void                error_in_socket(std::string err);
        int                 sockfd;
        int                 try_again;
    private:
        Socket();


};
#endif