/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Respond.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:35:48 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/22 21:23:41 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Respond.hpp"

Respond::Respond()
{};

Respond::Respond(int    client_socket) :client_socket(client_socket),  sending(false)
{
}

Respond::~Respond()
{
}

// void* send_all_thread(void* arg)
// {
//     Respond* respondObj = static_cast<Respond*>(arg);
//     respondObj->send_all();
//     pthread_exit(NULL);
// }

void    Respond::respond(packet_map &request,  conf &servers, std::string port)
{
    
    //here should extract the port and hostname to give to the corresponding method
    std::map<std::string, std::string> server_info = get_server_info(request, servers, port);
    
    visualize_string_map(server_info);
    fill_response(request, server_info);
    send_all();
    // pthread_create(&sendThread, NULL, send_all_thread, this);
    // usleep(5000);
    flush_response();

}

void    Respond::flush_response()
{
    response.clear();
    response_packet = "";

}


void    Respond::fill_response(packet_map &request, std::map<std::string, std::string> &server_info)
{
    if ((response.find("Content-Length:") != response.end() && response.find("Transfer-Encoding:") != response.end())
        || check_poisoned_url(request))
    {
        fill_status_code("400", "request method not supported");
        return ;
    }
    response["Status-code"].push_back("200");
    if (request.find("GET") != request.end())
        response_packet = normal_GET_Response(request, server_info);
    else if (request.find("POST") != request.end())
    {
        Post apost(request, server_info);
        // response_packet = apost.get_response();
    }
    else if (request.find("DELETE") != request.end())
    {
        DELETE DELETE_response;
        response_packet = DELETE_response.delete_response_filler(request, response, server_info);
    }
    else
        fill_status_code("400", "request method not supported");   
};

std::string     Respond::normal_GET_Response(packet_map &request, std::map<std::string, std::string> &server_info)
{
    GET GET_fill(request, response);
    GET_fill.prepare_get_response(server_info);
    return (GET_response(GET_fill.response).fill_get_response(server_info));
}

int Respond::check_poisoned_url(packet_map &request)
{
    if (request.find("GET") != request.end())
    {
        if (request["GET"][0].find(" ") != std::string::npos)
            return (1);
    }
    if (request.find("POST") != request.end())
    {
        if (request["POST"][0].find(" ") != std::string::npos)
             return (1);
    }
    if (request.find("DELETE") != request.end())
    {
        if (request["DELETE"][0].find(" ") != std::string::npos)
          return (1);
    }
    return (0);
}


void    Respond::visualize_response()
{
    std::cout << BOLDRED << "\nVisualizing reponse API\n" << std::endl;
    std::cout << "{" << std::endl << RESET;
    for (response_pack::iterator it = response.begin(); it != response.end(); it++)
    {
        if ((it->first).length() < 10000)
            std::cout << "  \"" << it->first << "\": [";
        else
            std::cout << "\"" << "large packet not gonna visualize" << "\", ";

         for (std::vector<std::string>::iterator vit = it->second.begin(); vit != it->second.end(); ++vit)
        {
            if ((*vit).length() < 10000)
                std::cout << "\"" << *vit << "\", ";
            else
                std::cout << "\"" << "large packet not gonna visualize" << "\", ";
        }
        std::cout << BOLDBLUE << "]" << std::endl << RESET;
    }
    std::cout << BOLDRED << "}" << std::endl << RESET;
}


void    Respond::send_all()
{
    size_t  response_bytes_sent = 0;
    size_t  packet_len = response_packet.length(); 
    std::cout << "inside send all" << std::endl;
    const char *a = response_packet.c_str();
    // sending = true ;
    visualize_response();
    if (packet_len < 10000)
        std::cout << BOLDRED << "visualizign response \n" << WHITE << response_packet << std::endl << RESET;
    else
        std::cout << "large response packet not gonna visualize\n" ;
    std::cout << "COnversion ends" << std::endl;
    while (response_bytes_sent < packet_len)
    {
        if (packet_len - response_bytes_sent > BUFFER_SIZE)
            response_bytes_sent += send(client_socket, a, BUFFER_SIZE, 0);
        else
            response_bytes_sent += send(client_socket, a, packet_len - response_bytes_sent, 0);  
        // usleep(100);
        if (response_bytes_sent <= 0)
        {
            perror("sent failed");
            break ;
        }
    }
    // sending = false;
}

int Respond::fill_status_code(std::string status_code, std::string message)
{
    response["Status-code"].clear();
    response["Status-code"].push_back(status_code);
    response["Status-code"].push_back(message);
    return (1);
}

std::map<std::string, std::string>  Respond::get_server_info(packet_map &request,conf &servers, std::string port)
{
    std::vector<int>            nominated_servers;
    std::vector<std::string>    server_names;
    std::cout << BOLDGREEN << "port  = " << port << std::endl;
    unsigned long n = servers.size();
    std::cout << "we have " << n << "servers\n" << RESET;
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        std::cout << "config port = " << servers[i]["Port"] << std::endl ;
        std::cout << "1" << std::endl;
        if (servers[i]["Port"] == port)
        {
            std::cout << "2" << std::endl;
            nominated_servers.push_back(i);
        }
    }
    
    for (unsigned long i = 0; i < nominated_servers.size(); i++)
    {
        server_names =  split(servers[nominated_servers[i]]["server_name"], " ");
        for (unsigned long j = 0; j < server_names.size(); j++)
        {   
            std::vector<std::string> tmp_vec;
            if (request.find("Host:") == request.end() || request["Host:"].size() != 1)  
            {
                tmp_vec.push_back("400");
                tmp_vec.push_back("No host");
                response["Status-code"] = tmp_vec;
                
                return (servers[0]);
            }
            
            std::cout << "our hostname is " << request["Host:"][0] << std::endl;
            std::cout << "server hostname " << server_names[j] << std::endl;
            std::cout << "trimmed host = " << request["Host:"][0].substr(0, server_names[j].length()) << std::endl;
            if (server_names[j] == request["Host:"][0].substr(0, server_names[j].length()))
            {
                std::cout << "our server is " << nominated_servers[j] << std::endl;
                return (servers[nominated_servers[i]]);
            }
        }
    }
    return (servers[0]);
}
