/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:18 by ahsalem           #+#    #+#             */
/* ************************************************************************** */

#include "../includes/libs.hpp"

void printAddrInfo( struct addrinfo *ai)
{
     char ip[INET6_ADDRSTRLEN];
    printf("Flags: ");
    if (ai->ai_flags & AI_PASSIVE)
        printf("AI_PASSIVE ");
    if (ai->ai_flags & AI_CANONNAME)
        printf("AI_CANONNAME ");
    if (ai->ai_flags & AI_NUMERICHOST)
        printf("AI_NUMERICHOST ");
    if (ai->ai_flags & AI_NUMERICSERV)
        printf("AI_NUMERICSERV ");
    if (ai->ai_flags & AI_V4MAPPED)
        printf("AI_V4MAPPED ");
    if (ai->ai_flags & AI_ALL)
        printf("AI_ALL ");
    if (ai->ai_flags & AI_ADDRCONFIG)
        printf("AI_ADDRCONFIG ");
    printf("\n");

    printf("Family: ");
    switch (ai->ai_family) {
        case AF_UNSPEC:
            printf("AF_UNSPEC\n");
            break;
            case AF_INET:
                printf("AF_INET\n");
                break;
            case AF_INET6:
                printf("AF_INET6\n");
                break;
            default:
                printf("Unknown Address Family\n");
                break;
        }
    
        printf("Socket Type: ");
        switch (ai->ai_socktype) {
            case SOCK_STREAM:
                printf("SOCK_STREAM\n");
                break;
            case SOCK_DGRAM:
                printf("SOCK_DGRAM\n");
                break;
            case SOCK_RAW:
                printf("SOCK_RAW\n");
                break;
            default:
                printf("Unknown Socket Type\n");
                break;
        }
    
        printf("Protocol: ");
        switch (ai->ai_protocol) {
            case IPPROTO_TCP:
                printf("IPPROTO_TCP\n");
                break;
            case IPPROTO_UDP:
                printf("IPPROTO_UDP\n");
                break;
            default:
                printf("Unknown Protocol\n");
                break;
        }
    
        printf("Address Length: %u\n", (unsigned int) ai->ai_addrlen);
    
    if (ai->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) ai->ai_addr;
            inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
            printf("IP Address: %s\n", ip);
            printf("Port Number: %d\n", ntohs(ipv4->sin_port));
        } else if (ai->ai_family == AF_INET6) {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) ai->ai_addr;
            inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, INET6_ADDRSTRLEN);
            printf("IP Address: %s\n", ip);
            printf("Port Number: %d\n", ntohs(ipv6->sin6_port));
        } else {
        printf("Unknown Address Family\n");
            printf("Unknown Address Family\n");
        }
    
        printf("Next: %p\n", ai->ai_next);
}


void    visualize_servers(
    std::vector<std::map<std::string, std::string> > servers)
{
    std::cout << BOLDRED << "visualizing "  << servers.size() <<" servers\n";
    std::cout << "---------------------------------------\n\n" << RESET; 
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        std::cout << RED << "server " << i + 1 << " {\n" << RESET;
        for (std::map<std::string, std::string>::reverse_iterator it = servers[i].rbegin();
            it != servers[i].rend(); ++it)
        {
            std::cout << BOLD << "  \""<< it->first << RESET << "\": " << "\"" << it->second << "\",\n";
        }

        std::cout << " }\n";
    }
    std::cout << BOLDRED << "\n---------------------------------------\n\n";
    std::cout << "servers visualization ends" << std::endl << RESET;
}

void    visualize_string_map(std::map<std::string, std::string>  &map)
{
    std::cout << BOLDRED << "visualizing string map\n" << RESET << RED;
    if (map.size() == 0)
		std::cout << "The map you are trying to visualize is empty" << std::endl;
	else
		std::cout << "visualizing map of strings\n\n\n" << std::endl;
	std::cout << RESET;
    for (std::map<std::string, std::string>::reverse_iterator rit = map.rbegin();
        rit != map.rend(); ++rit)
    {
        std::cout << "\"" << BOLDBLUE << rit->first << RESET << "\":  \"" << rit->second << "\"" << std::endl;
    }
        std::cout << BOLDRED << "\n\nvisualization ends\n\n\n" << std::endl << RESET;
}

class Logger {
private:
    std::ofstream logFile;

    std::string getCurrentTimestamp() {
        std::time_t currentTime = std::time(NULL);
        return std::ctime(&currentTime);
    }

public:
    Logger(const std::string& filePath) {
        logFile.open(filePath.c_str(), std::ios::app);
    }

    ~Logger() {
        logFile.close();
    }

    void log(LogLevel level, const std::string& message) {
        std::string logLevelStr;
        switch (level) {
            case server:
                logLevelStr = "SERVER";
                break;
            case request:
                logLevelStr = "REQUEST";
                break;
            case response:
                logLevelStr = "RESPONSE";
                break;
            case client:
                logLevelStr = "CLIENT";
                break;
            case FATAL:
                logLevelStr = "FATAL";
                break;
            default:
                logLevelStr = "other";
                break;
        }

        std::string timestamp = getCurrentTimestamp();
        timestamp.erase(timestamp.length() - 1);  // Remove newline character from timestamp

        logFile << "[" << timestamp << "] [" << logLevelStr << "] " << message << std::endl;
        logFile.flush();  // Flush the buffer to ensure immediate write to the file
    }
};

void    vis_str(std::string packet, std::string message)
{
        if (packet.length() < 10000)
            std::cout << MAGENTA << packet << RESET << std::endl;
        else
            std::cout << MAGENTA  << message << RESET;
}

void    print_to_file(std::string path, std::string str)
{
    std::ofstream outFile(path, std::ios::app);
    if (!outFile.is_open()) {
        std::cout << "Error opening the file!" << std::endl;
    }
    outFile << str << std::endl;
    outFile.close(); 
}