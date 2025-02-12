#ifndef KQUE_HPP
# define KQUE_HPP

# include "../../includes/libs.hpp"
# include "../../includes/webserve.hpp"
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
		bool		inActiveClients(int tmp_fd);
        bool        removeClient(int client_socket);
        void		killTimeoutedClients(void);
        bool        closeServer(void);

    private:
		
        std::map<int, Client>	clients;
        std::set<int>			active_clients;
        struct sockaddr_in		client_address;
        struct kevent			event[2];
        struct kevent			events[MAX_EVENTS];
        int						kq;
        int						active_fds;
        int						tmp_fd;
        int						client_socket;
        std::vector<int>		server_sockets;

};
#endif