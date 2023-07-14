/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listner.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:37 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/14 03:02:21 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listner.hpp"



Listner::Listner(std::string port): Socket(port)
{
    while (bind(sockfd, try_addr->ai_addr, try_addr->ai_addrlen) == -1)
    {
        close(sockfd);
        perror("server: Listner");
        try_open_socket_again(try_addr->ai_next);
    }
    std::cout << "SOCKET binded " << std::endl;
    printAddrInfo(try_addr);
    freeaddrinfo(res);
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen error");
        throw(std::runtime_error("Listen error"));
    }
}

Listner::~Listner()
{

}

int Listner::get_sockfd()
{
    return (sockfd);
}