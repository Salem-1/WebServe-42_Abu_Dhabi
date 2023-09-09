





/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:24 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/14 14:46:34 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int  client_socket, conf  servers): state(KEEP_ALIVE), client_socket(client_socket),
    start_time(clock()), receiver(client_socket), responder(client_socket), servers(servers)
{
    std::cout << "connectino started with " << client_socket << std::endl;
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

int Client::getTimeout()
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


void Client::handleRequest(struct kevent &event)
{
    start_time = clock();
    receiver.read_sock = client_socket;
    responder.client_socket = client_socket;

    if (event.filter == EVFILT_WRITE && responder.sending 
        && receiver.state == KEEP_ALIVE)
    {
        responder.sendAll(receiver.state);
        if (!responder.sending)
		{
			if (receiver.parser.request.find("connection:") != receiver.parser.request.end() 
					&& receiver.parser.request["connection:"].front() == "keep-alive")
				state = KEEP_ALIVE;
            receiver.flushReceive();
		}
    }
    else if (event.filter == EVFILT_READ 
        && receiver.state == KEEP_ALIVE)
    {
		try
		{
			responder.sending = false;
			receiver.receiveAll();
			
			if (!receiver.parser.read_again && receiver.state == KEEP_ALIVE)
			{
				responder.respond(receiver.parser.request, 
					receiver.parser.full_request, servers, getPort(client_socket));
			}
		}
		catch(const std::exception& e)
		{
			receiver.parser.read_again = 0;
			ErrResponse err;
			stringmap server_info = responder.getServerInfo(receiver.parser.request, servers, getPort(client_socket));
			responder.response_string = err.code(server_info, e.what());
            responder.sending = true;
			std::cerr << BOLDRED<<  e.what() << '\n' << RESET;
		}
		
    }    
    if (receiver.state == KILL_CONNECTION)
    {
        this->state = KILL_CONNECTION;
        return ;
    }
}

std::string Client::getPort(int client_socket)
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Get the local address that the socket is bound to
    if (getsockname(client_socket, (struct sockaddr*)&addr, &addr_len) == -1) {
        print_error("getsockname");
        return ("no port attached");
    }
    // Convert the IP address to a human-readable string
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip_str, INET_ADDRSTRLEN);

    std::stringstream ss;
	ss << ntohs(addr.sin_port);
	std::string port(ss.str());
    return (port);
}