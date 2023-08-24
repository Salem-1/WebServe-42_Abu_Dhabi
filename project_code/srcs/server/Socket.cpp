/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:28 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/24 09:24:30 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(): try_again(1)
{
    std::cout << "Hello from the socket" << std::endl;
}

Socket::Socket(std::string port): port(port), sockfd(0), try_again(1)
{

    getMyAddinfo();
    for (struct  addrinfo    *tr_addr = res; tr_addr != NULL && try_again; tr_addr = tr_addr->ai_next)
    {
        try_addr = tr_addr;
        openSocket(tr_addr);
    }
    if (!try_addr)
        errorInSocket("couldn't open socket: ");
    nonBlockSock();
};

void    Socket::tryOpenSocketAgain( struct  addrinfo *try_another_info)
{
    for (struct addrinfo* addr = try_another_info; addr != NULL && try_again; addr = addr->ai_next)
    {
        try_addr = addr;
        openSocket(addr);
    }
    if (!try_addr || !try_another_info)
        errorInSocket("couldn't open socket: ");
    nonBlockSock();
}

Socket::~Socket()
{}


void    Socket::getMyAddinfo(void)
{
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags= AI_PASSIVE;
    if (getaddrinfo(NULL, port.c_str(), &hints, &res) < 0)
        errorInSocket("Addrinfo inside socket Error: ");
}

void    Socket::openSocket(struct  addrinfo    *try_addr)
{
    sockfd = socket(try_addr->ai_family,  try_addr->ai_socktype, try_addr->ai_protocol);

    if (sockfd < 0)
        print_error("openSocket Error: ");
    else
        try_again = 0;
}

void    Socket::nonBlockSock(void)
{
    int yes = 1;

    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
        errorInSocket("fcntl nonblock error: ");
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
        errorInSocket("error setting socket option: ");
    std::cout << "Socket set to non block" << std::endl;
}
void    Socket::errorInSocket(std::string err)
{
    print_error(err.c_str());
    freeaddrinfo(res);
    throw(std::runtime_error("Socket opening error"));  
}

