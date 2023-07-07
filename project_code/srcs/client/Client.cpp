/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:24 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/08 00:37:08 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int  client_socket, conf servers): state(KEEP_ALIVE), client_socket(client_socket),
    start_time(clock()), receiver(client_socket), responder(client_socket), servers(servers)
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
        this->servers = obj2.servers;
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
    std::cout << "read again value  = " << receiver.parser.read_again << std::endl;
    if (receiver.state == KILL_CONNECTION)
    {
        std::cout << "Killing connection inside client" << std::endl;
        this->state = KILL_CONNECTION;
    }
    else if (!receiver.parser.read_again)
    {
        //stopped here should build respond clas
        std::cout << "\ninside client sending packet\n" << receiver.parser.packet;
        responder.respond(receiver.get_request_packet(), servers, get_port(client_socket));
    }
}

std::string Client::get_port(int client_socket)
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Get the local address that the socket is bound to
    if (getsockname(client_socket, (struct sockaddr*)&addr, &addr_len) == -1) {
        perror("getsockname");
        return ("no port attached");
    }

    // Convert the IP address to a human-readable string
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip_str, INET_ADDRSTRLEN);

    std::stringstream ss;
	ss << ntohs(addr.sin_port);
	std::string port(ss.str());
    // Print the local address information
    printf("Socket Local Address: %s\n", ip_str);
    printf("Socket Local Port: %d\n", ntohs(addr.sin_port));
    return (port);
}