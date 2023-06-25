/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Respond.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:35:48 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/25 12:38:31 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Respond.hpp"

Respond::Respond()
{};

Respond::Respond(int    client_socket) :client_socket(client_socket)
{
}

Respond::~Respond()
{
}

void    Respond::respond(packet_map &request,  std::map<std::string, std::string> &server_info)
{
    
    fill_response(request,  server_info);
    send_all();
    flush_response();

}

void    Respond::flush_response()
{
    response.clear();
    response_packet = "";

}

void    Respond::fill_response(packet_map &request,  std::map<std::string, std::string> &server_info)
{
    if ((response.find("Content-Length:") != response.end() && response.find("Transfer-Encoding:") != response.end())
        || check_poisoned_url(request))
    {
        fill_status_code("400", "request method not supported");
        return ;
    }
    response["Status-code"].push_back("200");
    if (request.find("GET") != request.end())
    {
        GET GET_fill(request, response);
        GET_fill.prepare_get_response(server_info);
        response_packet = GET_response(GET_fill.response).fill_get_response(server_info);
    }
    else if (request.find("POST") != request.end())
    {
        std::cout << "POST request under construction" << std::endl;
    }
    else if (request.find("DELETE") != request.end())
    {
        DELETE DELETE_response;
        response_packet = DELETE_response.delete_response_filler(request, response, server_info);
    }
    else
        fill_status_code("400", "request method not supported");
    
};

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
    std::cout << "\nVisualizing reponse API\n" << std::endl;
    std::cout << "{" << std::endl;
    for (response_pack::iterator it = response.begin(); it != response.end(); it++)
    {
        std::cout << "  \"" << it->first << "\": [";

         for (std::vector<std::string>::iterator vit = it->second.begin(); vit != it->second.end(); ++vit)
        {
            std::cout << "\"" << *vit << "\", ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "}" << std::endl;
}


void    Respond::send_all()
{
    size_t             response_bytes_sent = 0;
    visualize_response();

    std::cout << "visualizign response \n" << response_packet << std::endl;
    while (response_bytes_sent < response_packet.length())
            response_bytes_sent += send(client_socket, response_packet.c_str(), response_packet.length(), 0);
    std::cout << "sent = " << response_bytes_sent << " length is " << response_packet.length() << std::endl;
    if (response_bytes_sent < 0)
       perror("sent failed");

}

int Respond::fill_status_code(std::string status_code, std::string message)
{
    response["Status-code"].clear();
    response["Status-code"].push_back(status_code);
    response["Status-code"].push_back(message);
    return (1);
}
