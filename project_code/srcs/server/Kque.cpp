/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kque.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:44 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/24 18:15:07 by ahsalem          ###   ########.fr       */
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
        add_read_write_event(*it);
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
        //modify this to loop over the active clients only
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
                    add_read_write_event(client_socket);
                    clients[client_socket] = Client(client_socket, servers);
                    active_clients.insert(client_socket);
                }
            }
            else if (in_active_clients(tmp_fd))
                handle_request_by_client(events[i]);
        }
        // kill_timeouted_clients();
    }
}

bool    Kque::in_active_clients(int tmp_fd)
{
    for(std::set<int>::iterator it = active_clients.begin(); 
        it != active_clients.end(); it++)
    {
        std::cout << "active fd " << *it << std::endl;
    }
    for(std::set<int>::iterator it = active_clients.begin(); 
        it != active_clients.end(); it++)
    {
        if (*it == tmp_fd)
            return (true);
    }
    return (false);
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

void    Kque::handle_request_by_client(struct kevent event)
{
    int client_socket = event.ident;
    active_clients.insert(client_socket);
    
    clients[client_socket].handle_request(event);

    std::cout << "inside kque after handling request state = ";
    std::cout << clients[client_socket].state << std::endl;
    std::cout << "client " << client_socket << " is open" << std::endl;
    clients[client_socket].get_port(client_socket);
    if (event.filter == EVFILT_READ)
        std::cout << "Open for read" << std::endl;
    else if (event.filter == EVFILT_WRITE)
        std::cout << "Open for write" << std::endl;
    if (clients[client_socket].state == KILL_CONNECTION)
    {
        // std::cout << "closing the connection and deleting client "<< client_socket << " inside kqueue\n";
        // pthread_join(clients[client_socket].responder.sendThread, NULL);
        delete_fd_event(client_socket);
        clients.erase(client_socket);
        active_clients.erase(client_socket);
        return ;
    }
}

void    Kque::add_read_write_event(int fd)
{
    EV_SET(&event[0], fd, EVFILT_READ , EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event[0], 1, NULL, 0, NULL) < 0)
        kque_error("failed to add socket to event kque: ");
    std::cout << "added read event client " << fd << std::endl;
    EV_SET(&event[1], fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event[1], 1, NULL, 0, NULL) < 0)
        kque_error("failed to add socket to event kque: ");
    std::cout << "added write event client " << fd << std::endl;
    // sleep(1);
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
    EV_SET(&event[0], fd, EVFILT_READ , EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &event[0], 1, NULL, 0, NULL) < 0)
        kque_error("failed to remove socket from event kque: ");
    std::cout << "deleted read event client " << fd << std::endl;
    EV_SET(&event[1], fd, EVFILT_WRITE , EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &event[1], 1, NULL, 0, NULL) < 0)
        kque_error("failed to remove socket from event kque: ");
    std::cout << "deleted write event client " << fd << std::endl;
    close(fd);
    // sleep(1);
}

void   Kque::kque_error(std::string msg)
{
    perror(msg.c_str());
    // throw(std::runtime_error("Kque error"));
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