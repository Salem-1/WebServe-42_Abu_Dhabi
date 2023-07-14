/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kque.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:44 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/14 22:50:42 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kque.hpp"

Kque::Kque(std::vector<int> socket_fds): kq(kqueue()), server_sockets(socket_fds)
{
    if (kq < 0)
        kque_error("failed to start kq system call: ");
    for (std::vector<int>::iterator it = server_sockets.begin();
            it != server_sockets.end(); ++it)
    {
        add_read_event(*it);
    }
};

Kque::~Kque()
{}

void    Kque::watch_fds(conf &servers)
{
   while (1)
    {
        active_fds = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (active_fds == -1)
            kque_error("Kevent loop failed: ");
        for (int i = 0; i < active_fds; i++)
        {
            tmp_fd = events[i].ident;
            if (tmp_fd_in_server_socket(tmp_fd))
            {
                if (events[i].filter == EVFILT_READ)
                {
                    client_socket = accepting(tmp_fd);
                    std::string hero_port = socket_info(client_socket);
                    if(client_socket < 0)
                        continue ;
                    add_read_event(client_socket);
                    clients[client_socket] = Client(client_socket, servers);
                    active_clients.insert(client_socket);
                }
            }
            else
                handle_request_by_client(tmp_fd);
        }
        // kill_timeouted_clients();
    }
}

bool Kque::tmp_fd_in_server_socket(int tmp_fd)
{
        for (std::vector<int>::iterator it = server_sockets.begin();
                it != server_sockets.end(); ++it)
        {
            if (tmp_fd == *it)
                return (true);
        }
        return (false);
}

void    Kque::kill_timeouted_clients()
{
    int active = 0;
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        for (std::set<int>::iterator sit = active_clients.begin(); sit != active_clients.end(); ++sit)
        {
            if (it->first == *sit)
            {
                active = 1;
                break ;
            }
        }
        if  (active)
        {
            active = 0;
            continue ;
        }
        else
        {
            if (it->second.get_timeout() > PERSISTANCE)
            {
                delete_fd_event(it->first);
                clients.erase(it->first);
            }
        }
    }
    active_clients.clear();
};

void    Kque::handle_request_by_client(int tmp_fd)
{
    active_clients.insert(tmp_fd);
//I believe thread should be here 
    clients[tmp_fd].handle_request();
    std::cout << "inside kque after handling request state = ";
    std::cout << clients[tmp_fd].state << std::endl;
    if (clients[tmp_fd].state == KILL_CONNECTION)
    {
        std::cout << "closing the connection and deleting client "<< tmp_fd << " inside kqueue\n";
        delete_fd_event(tmp_fd);
        clients.erase(tmp_fd);
        return ;
    }
}

void    Kque::add_read_event(int fd)
{
    EV_SET(&event, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event, 1, NULL, 0, NULL) < 0)
        kque_error("failed to add socket to event kque: ");
}

int    Kque::accepting(int  fd)
{

    int accepted_connection = accept(fd, (struct sockaddr*)&client_address,
                                 &client_address_len);
    if (accepted_connection < 0)
        perror("accept failed");
    return (accepted_connection);
}

void    Kque::delete_fd_event(int fd)
{
    EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &event, 1, NULL, 0, NULL) < 0)
        kque_error("failed to remove socket from event kque: ");
    close(fd);
}

void   Kque::kque_error(std::string msg)
{
    perror(msg.c_str());
    throw(std::runtime_error("Kque error"));
}


std::string  Kque::socket_info(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockname(sockfd, (struct sockaddr*)&addr, &addr_len) == -1)
    {
        perror("getsockname");
        throw(std::runtime_error("getsockname error"));
    }
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip_str, INET_ADDRSTRLEN);
    
    std::cout << "Socket Local Address: " << ip_str << std::endl;
    std::cout << "Socket Local Port: " <<  ntohs(addr.sin_port)<< std::endl;  
    std::stringstream ss;
    ss << ntohs(addr.sin_port);

    std::string port = ss.str();
    return (port);
}