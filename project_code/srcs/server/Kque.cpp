#include "Kque.hpp"

Kque::Kque(int    socket_fd): kq(kqueue()), server_socket(socket_fd)
{
    if (kq < 0)
        kque_error("failed to start kq system call: ");
    add_read_event(server_socket);
};

Kque::~Kque()
{}

void    Kque::watch_fds()
{
   while (1)
    {
        active_fds = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (active_fds == -1)
            kque_error("Kevent loop failed: ");
        for (int i = 0; i < active_fds; i++)
        {
            tmp_fd = events[i].ident;
            if (tmp_fd == server_socket)
            {
                client_socket = accepting(server_socket);
                if(client_socket < 0)
                    continue ;
                add_read_event(client_socket);
                clients[client_socket] = Client(client_socket);
                // clients.insert(std::pair<int, Client>(client_socket, Client(client_socket)));
                active_clients.insert(client_socket);
            }
            else
                handle_request_by_client(tmp_fd);
        }
        // kill_timeouted_clients();
    }
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
   
    clients[tmp_fd].handle_request();
    if (clients[tmp_fd].state == KILL_CONNECTION)
    {
        delete_fd_event(tmp_fd);
        clients.erase(tmp_fd);
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
    exit(1);
}
