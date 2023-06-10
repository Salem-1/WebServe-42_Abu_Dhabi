#ifndef KQUE_HPP
# define KQUE_HPP

# include "../../includes/libs.hpp"

//Will need to keep track for the time out and keep alive to disconnect the clients inshalla
//I am thinking about make it in big map that hold all client data and keep track of time
class Kque
{
    public:
        Kque(int socket_fd);
        ~Kque();
        void            watch_fds();
    private:
        void                kque_error(std::string msg);
        void                add_read_event(int fd);
        void                delete_fd_event(int fd);
        int                 accepting(int fd);
        int                 kq;
        int                 active_fds;
        int                 tmp_fd;
        int                 client_socket;
        int                 server_socket;
        socklen_t           client_address_len;
        struct kevent       event;
        struct kevent       events[MAX_EVENTS];
        struct sockaddr_in  client_address;


void    handle_send_and_recieve_till_making_class_for_them_inshalla
        (int tmp_fd, int client_socket);
};
#endif