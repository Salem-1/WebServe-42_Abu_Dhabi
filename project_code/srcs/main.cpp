
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:21 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/24 15:37:22 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserve.hpp"

void    running_one_server(std::map<std::string, std::string> one_server)
{
    Listner binded_sock(one_server);

    printf("server waiting for connection....\n");
    Kque socket_manager(binded_sock.sockfd);
    socket_manager.watch_fds(one_server);
}

struct ServerRunner
{
    std::map<std::string, std::string> server;

    ServerRunner(const std::map<std::string, std::string>& server)
        : server(server)
    {
    }

    void operator()()
    {
        running_one_server(server);
    }
};

int main()
{
    Config  servers;
    
    std::vector<std::thread> servers_threads;

    for (unsigned long i = 0; i < servers.servers.size(); i++)
    {
        ServerRunner serverRunner(servers.servers[i]);
		// std::thread(serverRunner);

        // servers_threads.push_back(std::thread(serverRunner));  
    }
    for (unsigned long i = 0; i < servers_threads.size(); i++)
    {
        servers_threads[i].join();
    }
    visualize_servers(servers.servers);
    return (0);
}


