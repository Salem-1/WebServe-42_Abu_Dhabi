#ifndef KQUE_HPP
# define KQUE_HPP

# include "../../includes/libs.hpp"
# include "../../includes/webserve.hpp"
// # include "../client/Client.hpp"
//Will need to keep track for the time out and keep alive to disconnect the clients inshalla
//I am thinking about make it in big map that hold all client data and keep track of time
class Kque
{
    public:
        Kque(std::vector<int> socket_fds);
        ~Kque();
        void	watchFds(conf &servers);
    
    private:
        void		kqueError(std::string msg);
        void		addReadWriteEvent(int fd);
        void		deleteFdEvent(int fd);
        int			accepting(int fd);
        void		handleRequestByClient(struct kevent event);
        std::string	socketInfo(int sockfd);
        bool		tmpFdInServerSocket(int tmp_fd);
		bool		inactiveClients(int tmp_fd);
    private:
        void		killTimeoutedClients();
		
        std::map<int, Client>	clients;
        std::set<int>			active_clients;
        struct sockaddr_in		client_address;
        struct kevent			event[2];
        struct kevent			events[MAX_EVENTS];
        socklen_t				client_address_len;
        int						kq;
        int						active_fds;
        int						tmp_fd;
        int						client_socket;
        std::vector<int>		server_sockets;

};
#endif