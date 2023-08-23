/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:12 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/23 15:13:13 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "GET.hpp"


GET::GET(packet_map &request, response_packet &response): request(request), response(response)
{
}
GET::~GET()
{

}

void    GET::prepareGetResponse(stringmap &server_info)
{
    (void)server_info;
    fillRequestLine(request);
    fillPath(request, response, "GET");
}

// the two functions below belong to get
void GET::fillRequestLine(packet_map &request)
{
	if ((request.find("POST") != request.end() || request.find("DELETE") != request.end())
				&& fillStatuCode(response, "400", "Invalid multiple methods inside GET"))
		return ;
	if (request["GET"].size() != 2)
		if (fillStatuCode(response, "400", "Invalid number of items inside GET"))
			return ;
    if ((request["GET"][0].length() + request["GET"][1].length()) > HEADER_MAX_LENGTH
            && fillStatuCode(response, "414", "URI Too Long"))
        return ;
    if (request["GET"][1].find("HTTP/1.") == std::string::npos
        && fillStatuCode(response, "505", "version not supported"))
        return ;
    if ((request["GET"].size() != 2) 
        && (fillStatuCode(response, "400", "get vec has wrong number items bad request")))
        return ;
    else
        response["HTTP version"].push_back(request["GET"][1]);
    response["method"].push_back("GET");
}


