#ifndef KQUE_HPP
# define KQUE_HPP

# include "../../includes/libs.hpp"
# include "../client/Client.hpp"
//Will need to keep track for the time out and keep alive to disconnect the clients inshalla
//I am thinking about make it in big map that hold all client data and keep track of time
class Kque
{
    public:
        Kque(int socket_fd);
        ~Kque();
        void                    watch_fds();
    
    private:
        void                    kque_error(std::string msg);
        void                    add_read_event(int fd);
        void                    delete_fd_event(int fd);
        int                     accepting(int fd);
        void                    handle_request_by_client
                                    (int tmp_fd);
    
    private:
        void                    kill_timeouted_clients();
        std::map<int, Client>   clients;
        std::set<int>           active_clients;
        struct sockaddr_in      client_address;
        struct kevent           event;
        struct kevent           events[MAX_EVENTS];
        socklen_t               client_address_len;
        int                     kq;
        int                     active_fds;
        int                     tmp_fd;
        int                     client_socket;
        int                     server_socket;

};
#endif