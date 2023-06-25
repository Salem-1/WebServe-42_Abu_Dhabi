/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:12 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/24 20:08:36 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "GET.hpp"


GET::GET(packet_map &request, response_packet &response): request(request), response(response)
{
}
GET::~GET()
{

}

void    GET::prepare_get_response(std::map<std::string, std::string> &server_info)
{
    (void)server_info;
    fill_request_line(request);
    fill_path(request, response, "GET");
}

// the two functions below belong to get
void GET::fill_request_line(packet_map &request)
{
    if ((request.find("POST") != request.end() || request.find("DELETE") != request.end())
        && fill_status_code(response, "400", "Invalid multiple methods inside GET"))
        return ;
    if ((request["GET"][0].length() + request["GET"][1].length()) > HEADER_MAX_LENGTH
            && fill_status_code(response, "414", "URI Too Long"))
        return ;
    if (!(request["GET"][1] == "HTTP/1.1")
        && (fill_status_code(response, "505", "version not supported")))
        return ;
    if ((request["GET"].size() != 2) 
        && (fill_status_code(response, "400", "get vec has wrong number items bad request")))
        return ;
    else
        response["HTTP version"].push_back(request["GET"][1]);
    response["method"].push_back("GET");
}


