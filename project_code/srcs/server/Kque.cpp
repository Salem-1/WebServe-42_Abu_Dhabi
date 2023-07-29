/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kque.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:44 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/29 22:51:35 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kque.hpp"

Kque::Kque(std::vector<int> socket_fds): kq(kqueue()), server_sockets(socket_fds)
{
    if (kq < 0)
        kqueError("failed to start kq system call: ");
    for (std::vector<int>::iterator it = server_sockets.begin();
            it != server_sockets.end(); ++it)
    {
        addReadWriteEvent(*it);
    }
};

Kque::~Kque()
{}

void    Kque::watchFds(conf &servers)
{
   while (1)
    {
        active_fds = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (active_fds == -1)
            kqueError("Kevent loop failed: ");
        for (int i = 0; i < active_fds; i++)
        {
            tmp_fd = events[i].ident;
            if (tmpFdInServerSocket(tmp_fd))
            {
                if (events[i].filter == EVFILT_READ)
                {
                    client_socket = accepting(tmp_fd);
                    std::string hero_port = socketInfo(client_socket);
                    if(client_socket < 0)
                        continue ;
                    addReadWriteEvent(client_socket);
                    clients[client_socket] = Client(client_socket, servers);
                    active_clients.insert(client_socket);
                }
            }
            else if (inactiveClients(tmp_fd))
                handleRequestByClient(events[i]);
        }
        // killTimeoutedClients();
    }
}

bool    Kque::inactiveClients(int tmp_fd)
{
    // for(std::set<int>::iterator it = active_clients.begin(); 
    //     it != active_clients.end(); it++)
    // {
    //     std::cout << "active fd " << *it << std::endl;
    // }
    for(std::set<int>::iterator it = active_clients.begin(); 
        it != active_clients.end(); it++)
    {
        if (*it == tmp_fd)
            return (true);
    }
    return (false);
}

bool Kque::tmpFdInServerSocket(int tmp_fd)
{
        for (std::vector<int>::iterator it = server_sockets.begin();
                it != server_sockets.end(); ++it)
        {
            if (tmp_fd == *it)
                return (true);
        }
        return (false);
}

void    Kque::killTimeoutedClients()
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
            if (it->second.getTimeout() > PERSISTANCE)
            {
                deleteFdEvent(it->first);
                clients.erase(it->first);
            }
        }
    }
    active_clients.clear();
};

void    Kque::handleRequestByClient(struct kevent event)
{
    int client_socket = event.ident;
    active_clients.insert(client_socket);
    clients[client_socket].handleRequest(event);
    // visualization only
    // std::cout << "inside kque after handling request state = ";
    // std::cout << clients[client_socket].state << std::endl;
    // std::cout << "client " << client_socket << " is open" << std::endl;
    clients[client_socket].getPort(client_socket);
    // if (event.filter == EVFILT_READ)
    //     std::cout << "Open for read" << std::endl;
    // else if (event.filter == EVFILT_WRITE)
    //     std::cout << "Open for write" << std::endl;
	//visualization ends
    if (clients[event.ident].state == KILL_CONNECTION)
    {
        std::cout << "closing the connection and deleting client "<< event.ident << " inside kqueue\n";
        // pthread_join(clients[event.ident].responder.sendThread, NULL);
        deleteFdEvent(client_socket);
        clients.erase(client_socket);
        active_clients.erase(client_socket);
        return ;
    }
}

void    Kque::addReadWriteEvent(int fd)
{
    EV_SET(&event[0], fd, EVFILT_READ ,  EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event[0], 1, NULL, 0, NULL) < 0)
        kqueError("failed to add socket to event kque: ");
    EV_SET(&event[1], fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event[1], 1, NULL, 0, NULL) < 0)
        kqueError("failed to add socket to event kque: ");
}

int    Kque::accepting(int  fd)
{

    int accepted_connection = accept(fd, (struct sockaddr*)&client_address,
                                 &client_address_len);
    if (accepted_connection < 0)
        perror("accept failed");
    return (accepted_connection);
}

void    Kque::deleteFdEvent(int fd)
{
    EV_SET(&event[0], fd, EVFILT_READ , EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &event[0], 1, NULL, 0, NULL) < 0)
        kqueError("failed to remove socket from event kque: ");
    EV_SET(&event[1], fd, EVFILT_WRITE , EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &event[1], 1, NULL, 0, NULL) < 0)
        kqueError("failed to remove socket from event kque: ");
    close(fd);
    // sleep(1);
}

void   Kque::kqueError(std::string msg)
{
    perror(msg.c_str());
    // throw(std::runtime_error("Kque error"));
}


std::string  Kque::socketInfo(int sockfd)
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