/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lib.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:09 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/25 12:07:49 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_lib.hpp"

std::vector<std::string> split(std::string str, std::string delimeter)
{
    std::vector<std::string>    result;

    if (!str.length())
        result.push_back("");
   else if (str.find(delimeter) == std::string::npos || !delimeter.length())
        result.push_back(str);
    if (str.find(delimeter) == std::string::npos || !str.length() || !delimeter.length())
        return (result);
    while (str.find(delimeter) != std::string::npos)
    {
        if (str.substr(0, str.find(delimeter))[0])
            result.push_back(str.substr(0, str.find(delimeter)));
        str = str.substr(str.find(delimeter) + delimeter.length(), str.length());   
    }
    if (str.substr(0, str.find(delimeter))[0])
        result.push_back(str.substr(0, str.find(delimeter)));
    return (result);
}

int fill_status_code(response_packet &response, std::string status_code, std::string message)
{
    response["Status-code"].clear();
    response["Status-code"].push_back(status_code);
    response["Status-code"].push_back(message);
    return (1);
}

void    fill_path(packet_map &request,response_packet &response, std::string method)
{
    //decide on absouloute or origin option path 
    //400 if wrong
    if ((request[method][0].find("/") == std::string::npos ||
            request[method][0][0] != '/')
        && (fill_status_code(response, "400", "bad origin path format")))
        return ;
    if (request[method][0].find("?") == std::string::npos)
    {
        response["Path"].push_back("absolute");
        response["Path"].push_back(request[method][0]);
    }
    else
    {
        if ((request[method][0][0] !=  '/' || request[method][0].find("&&") != std::string::npos)
            && (fill_status_code(response, "400", "bad origin path format")))
                return ;
        std::string base_path = request[method][0].substr(0, request[method][0].find("?"));
        response["Path"].push_back("origin");
        response["Path"].push_back(base_path);
        std::vector<std::string> tmp_origin = split(request[method][0], "?");
        tmp_origin.erase(tmp_origin.begin());
        tmp_origin = split(*(tmp_origin.begin()), "&");
        for (std::vector<std::string>::iterator it = tmp_origin.begin(); it != tmp_origin.end(); ++it)
        {
            response["Path"].push_back(*it);
        }
    }
}