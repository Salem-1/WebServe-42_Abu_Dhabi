/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kque.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:40 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/25 01:28:16 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KQUE_HPP
# define KQUE_HPP

# include "../../includes/libs.hpp"
# include "../client/Client.hpp"
//Will need to keep track for the time out and keep alive to disconnect the clients inshalla
//I am thinking about make it in big map that hold all client data and keep track of time
class Kque
{
    public:
        Kque(std::vector<int> socket_fds);
        ~Kque();
        void                    watc_fds(conf &servers);
    
    private:
        void                    kque_error(std::string msg);
        void                    add_read_write_event(int fd);
        void                    delete_fd_event(int fd);
        int                     accepting(int fd);
        void                    handle_request_by_client
                                    (struct kevent event);
        std::string                    socket_info(int sockfd);
        bool                     tmp_fd_in_server_socket(int tmp_fd);
    private:
        void                    kill_timeouted_clients();
        std::map<int, Client>   clients;
        std::set<int>           active_clients;
        struct sockaddr_in      client_address;
        struct kevent           event[2];
        struct kevent           events[MAX_EVENTS];
        socklen_t               client_address_len;
        int                     kq;
        int                     active_fds;
        int                     tmp_fd;
        int                     client_socket;
        std::vector<int>        server_sockets;

};
#endif