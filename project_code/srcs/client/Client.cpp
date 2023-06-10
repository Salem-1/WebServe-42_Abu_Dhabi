#include "Client.hpp"

Client::Client(int  client_socket): client_socket(client_socket)
{

};
Client &Client::operator= (const Client &obj2)
{
    if (this != &obj2)
    {
        this->client_socket = obj2.client_socket;
        this->time = obj2.time;
        this->state = obj2.state;
    }
    return (*this);
};

Client::Client(const Client &obj2)
{
    *this = obj2; 
};

Client::~Client()
{};

void Client::handle_request()
{
    char                buffer[800];
    ssize_t             reuest_bytes_read;
    ssize_t             response_bytes_sent;
    std::string         response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\nresponse from our server!";

    reuest_bytes_read = recv(client_socket, buffer, 870
        , 0);
    if (reuest_bytes_read == 0)
        state = KILL_CONNECTION;
    else
    {
        printf("recieved data from client");
        printf("%.*s/n", (int)reuest_bytes_read, buffer);
       response_bytes_sent = send(client_socket, response.c_str(), 87, 0);
       if (response_bytes_sent < 0)
           perror("sent failed");
    }
}
