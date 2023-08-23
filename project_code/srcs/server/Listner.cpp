/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listner.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:37 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/23 09:03:22 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listner.hpp"



Listner::Listner(std::string port): Socket(port)
{
    while (bind(sockfd, try_addr->ai_addr, try_addr->ai_addrlen) == -1)
    {
        close(sockfd);
        print_error("server: Listner");
        tryOpenSocketAgain(try_addr->ai_next);
    }
    std::cout << "SOCKET binded " << std::endl;
    printAddrInfo(try_addr);
    freeaddrinfo(res);
    if (listen(sockfd, BACKLOG) == -1)
    {
        print_error("listen error");
        throw(std::runtime_error("Listen error"));
    }
}

Listner::~Listner()
{

}

int Listner::getSockfd()
{
    return (sockfd);
}