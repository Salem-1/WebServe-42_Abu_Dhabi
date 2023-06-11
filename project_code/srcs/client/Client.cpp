#include "Client.hpp"

Client::Client(int  client_socket): client_socket(client_socket),
    start_time(clock())
{
    connection_duration = static_cast<int>(start_time) / TIME_PER_MILLI_SEC;
    std::cout << "connectino started at " << connection_duration << std::endl;
};
Client &Client::operator= (const Client &obj2)
{
    if (this != &obj2)
    {
        this->client_socket = obj2.client_socket;
        this->start_time = obj2.start_time;
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
    ssize_t             response_bytes_sent;
    std::string         dummyresponse = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 22\n\nresponse from our server!";

//receiver
    Receive receiver(client_socket);
    receiver.receive_all();
    if (receiver.state == KILL_CONNECTION)
        this->state = KILL_CONNECTION;
    else
    {
//responder
       response_bytes_sent = send(client_socket, dummyresponse.c_str(), 87, 0);
       if (response_bytes_sent < 0)
           perror("sent failed");
    }
}
