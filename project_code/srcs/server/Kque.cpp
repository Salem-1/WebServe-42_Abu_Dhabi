#include "Kque.hpp"

Kque::Kque(int    socket_fd): kq(kqueue()), server_socket(socket_fd)
{
    if (kq < 0)
        kque_error("failed to start kq system call: ");
    add_read_event(server_socket);
};

Kque::~Kque()
{};


void   Kque::kque_error(std::string msg)
{
    perror(msg.c_str());
    exit(1);
}

void    Kque::add_read_event(int fd)
{
    EV_SET(&event, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event, 1, NULL, 0, NULL) < 0)
        kque_error("failed to add socket to event kque: ");
}
void    Kque::delete_fd_event(int fd)
{
    EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &event, 1, NULL, 0, NULL) < 0)
        kque_error("failed to remove socket from event kque: ");
    close(fd);
}

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
            }
            else
                handle_send_and_recieve_till_making_class_for_them_inshalla
                    (tmp_fd, client_socket);
        }
    }
}

int    Kque::accepting(int  fd)
{

    int accepted_connection = accept(fd, (struct sockaddr*)&client_address,
                                 &client_address_len);
    if (accepted_connection < 0)
        perror("accept failed");
    return (accepted_connection);
}



void    Kque::handle_send_and_recieve_till_making_class_for_them_inshalla
        (int tmp_fd, int client_socket)
{
    (void)client_socket;
    char                buffer[800];
    ssize_t             reuest_bytes_read;
    ssize_t             response_bytes_sent;
    std::string         response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\nresponse from our server!";

    //it's a client socket event
    //request reading and parsing class here inshalla
    reuest_bytes_read = recv(tmp_fd, buffer, 870
        , 0);
    //request class end
    if (reuest_bytes_read == 0)
        delete_fd_event(tmp_fd);
    else
    {
        //class response
        printf("recieved data from client");
        printf("%.*s/n", (int)reuest_bytes_read, buffer);
       //class send
       response_bytes_sent = send(tmp_fd, response.c_str(), 87, 0);
       if (response_bytes_sent < 0)
           perror("sent failed");
       //end class send
       //end class respone
    }
}
