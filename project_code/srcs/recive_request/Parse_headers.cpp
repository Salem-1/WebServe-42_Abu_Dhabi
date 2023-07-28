/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse_headers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:06 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/27 21:27:36 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

int    Parser::check_headers()
{
    std::vector<std::string> status;
    std::cout << "visualizing inside Get request" << std::endl;
    visualize_request_packet();
   
    if (!valid_packet_headers())
    {
        status.push_back("400");
        status.push_back("Bad Request");
        request["Status-code"] = status;
        std::cout << "invalid packet" <<  std::endl;
        return (0);
    }
    else
    {
        status.push_back("200");
        status.push_back("Ok");
        request["Status-code:"] = status;
    }
    return (1);
}

int Parser::valid_packet_headers()
{
    bool    valid = false;
    std::set<std::string>::iterator vit = valid_headers.begin();
    for (packet_map::iterator it= request.begin(); it != request.end(); ++it)
    {
        for (vit = valid_headers.begin(); vit != valid_headers.end(); ++vit)
        {
            if (*vit == it->first)
            {
                valid = true;
                break ;
            }
        }
        if (valid)
        {
            valid = false;
            continue ;
        }
        else
        {
            std::cout << BOLDGREEN << "Header <" << it->first << "> not a valid header" << RESET <<std::endl;
            return (0);
        }
    }
    return (1);
}



void  Parser::fill_valid_headers()
{
    valid_headers.insert("GET");
	valid_headers.insert("POST");
	valid_headers.insert("DELETE");
    valid_headers.insert("Standard headers:");
    valid_headers.insert("accept-encoding:");
    valid_headers.insert("A-IM:");
    valid_headers.insert("Accept:");
    valid_headers.insert("Accept-Charset:");
    valid_headers.insert("Accept-Encoding:"); 
    valid_headers.insert("Accept-Language:");
    valid_headers.insert("Accept-Datetime:");
    valid_headers.insert("Access-Control-Request-Method:");
    valid_headers.insert("Access-Control-Request-Headers:");
    valid_headers.insert("Authorization:");
    valid_headers.insert("Cache-Control:");
    valid_headers.insert("Connection:");
    valid_headers.insert("Content-Length:");
    valid_headers.insert("content-length:");
    valid_headers.insert("Content-Type:");
    valid_headers.insert("Cookie:");
    valid_headers.insert("Date:");
    valid_headers.insert("Expect:");
    valid_headers.insert("Forwarded:");
    valid_headers.insert("From:");
    valid_headers.insert("Host:");
    valid_headers.insert("If-Match:");
    valid_headers.insert("If-Modified-Since:");
    valid_headers.insert("If-None-Match:");
    valid_headers.insert("If-Range:");
    valid_headers.insert("If-Unmodified-Since:");
    valid_headers.insert("Max-Forwards:");
    valid_headers.insert("Origin:");
    valid_headers.insert("Pragma:");
    valid_headers.insert("Proxy-Authorization:");
    valid_headers.insert("Range:");
    valid_headers.insert("Referer:");
    valid_headers.insert("TE:");
    valid_headers.insert("User-Agent:");
    valid_headers.insert("Upgrade:");
    valid_headers.insert("Via:");
    valid_headers.insert("Warning:");
    valid_headers.insert("Non-standard headers:");
    valid_headers.insert("Dnt:");
    valid_headers.insert("X-Requested-With:");
    valid_headers.insert("X-CSRF-Token:");
    valid_headers.insert("Sec-Fetch-Dest:");
    valid_headers.insert("Sec-Fetch-Dest:");
    valid_headers.insert("Sec-Fetch-Mode:");
    valid_headers.insert("Sec-Fetch-Site:");
    valid_headers.insert("Sec-Fetch-User:");
    valid_headers.insert("Upgrade-Insecure-Requests:");
    valid_headers.insert("sec-ch-ua-mobile:");
    valid_headers.insert("sec-ch-ua-platform:");
    valid_headers.insert("sec-ch-ua:");
    valid_headers.insert("Purpose:");
}

void    Parser::visualize_request_packet()
{
    std::cout << BOLDRED << "visualizing requesest packet API\n\n" << std::endl;
    std::cout << "{" << std::endl << RESET;
    for (packet_map::iterator hit = request.begin();
            hit != request.end(); ++hit)
    {
        std::cout << BOLDGREEN << "\"" << hit->first << "\": [" << RESET;
        for (std::vector<std::string>::iterator it = hit->second.begin();
                it != hit->second.end(); ++it)
        {
            std::cout << " " << *it;
        }
        std::cout << BOLDGREEN << "]" << std::endl << RESET;
    }
    std::cout << BOLDRED << "}\n packet visualization ends\n\n\n" << RESET;
}
