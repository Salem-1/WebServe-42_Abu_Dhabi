#include "Client.hpp"

Client::Client(int  client_socket, std::map<std::string, std::string> &server): client_socket(client_socket),
    start_time(clock()), receiver(client_socket), responder(client_socket), server_info(server)
{

    connection_duration = static_cast<int>(start_time) / TIME_PER_SEC;
    std::cout << "connectino started at " << connection_duration << std::endl;
};
Client &Client::operator= (const Client &obj2)
{
    if (this != &obj2)
    {
        this->client_socket = obj2.client_socket;
        this->start_time = obj2.start_time;
        this->state = obj2.state;
        this->server_info = obj2.server_info;
    }
    return (*this);
};

int Client::get_timeout()
{
    connection_duration = (static_cast<int>(clock()) / TIME_PER_SEC)
        - (static_cast<int>(start_time) / TIME_PER_SEC);
    return (connection_duration);
}

Client::Client(const Client &obj2)
{
    *this = obj2; 
};

Client::~Client()
{};

void Client::handle_request()
{
    start_time = clock();
//receiver
    receiver.read_sock = client_socket;
    responder.client_socket = client_socket;
    receiver.receive_all();
    if (receiver.state == KILL_CONNECTION)
        this->state = KILL_CONNECTION;
    else
    {
        //stopped here should build respond clas
       responder.respond(receiver.get_request_packet(), server_info);
    }
}
