/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lib.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:09 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/24 21:49:04 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_lib.hpp"

std::vector<std::string> split(std::string str, std::string delimeter)
{
    std::vector<std::string>    result;

    if (!str.length())
        result.push_back(str);
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
    if (str[0]) // .substr(0, str.find(delimeter))[0])
        result.push_back(str.substr(0, str.find(delimeter)));
    return (result);
}

std::set<std::string> split_to_set(std::string str, std::string delimeter)
{
    std::set<std::string>    result;

    if (!str.length())
        return (result);
   else if (str.find(delimeter) == std::string::npos || !delimeter.length())
        result.insert(str);
    if (str.find(delimeter) == std::string::npos || !str.length() || !delimeter.length())
        return (result);
    while (str.find(delimeter) != std::string::npos)
    {
        if (str.substr(0, str.find(delimeter))[0])
            result.insert(str.substr(0, str.find(delimeter)));
        str = str.substr(str.find(delimeter) + delimeter.length(), str.length());   
    }
	if (str[0] != '\0')
    if (str.substr(0, str.find(delimeter))[0])
        result.insert(str.substr(0, str.find(delimeter)));
    return (result);
}

int fillStatuCode(response_packet &response, std::string status_code, std::string message)
{
    response["Status-code"].clear();
    response["Status-code"].push_back(status_code);
    response["Status-code"].push_back(message);
    return (1);
}

void    fillPath(packet_map &request,response_packet &response, std::string method)
{
    //decide on absouloute or origin option path 
    //400 if wrong
    response["Path"].clear();
	if (request[method].size() != 2)
		if (fillStatuCode(response, "400", "Invalid number of items inside Method"))
			return ;
    if ((request[method][0].find("/") == std::string::npos ||
            request[method][0][0] != '/')
        && (fillStatuCode(response, "400", "bad origin path format")))
        return ;
    if (request[method][0].find("?") == std::string::npos)
    {
        response["Path"].push_back("absolute");
        response["Path"].push_back(request[method][0]);
    }
    else
    {
        if ((request[method][0][0] !=  '/' || request[method][0].find("&&") != std::string::npos)
            && (fillStatuCode(response, "400", "bad origin path format")))
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


void    print_error(std::string msg)        
{
    const char* errorMessage = strerror(errno);
    std::cerr << msg << errorMessage << std::endl;   
}

void handle_pipes(int sig)
{
    if (sig == SIGPIPE)
        std::cout << MAGENTA  << "\nBroken pipe: Client disconnected during sending " << RESET <<std::endl;
    else if (sig == SIGSEGV)
        std::cout << MAGENTA  << "\nSEGV: I should be running at all cost " << RESET <<std::endl;
    else if (sig == 15)
        std::cout << MAGENTA  << "\nSEGINTERRUPT: I should be running at all cost " << RESET <<std::endl;
}

void    runAtAllCost()
{
    signal(SIGPIPE, &handle_pipes);
    // signal(SIGSEGV, &handle_pipes);
    // signal(15, &handle_pipes);
}

void visualize_tokens(tokenized_conf &tokens)
{
    for (tokenized_conf::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        std::cout << BOLDYELLOW << "essentials: " << RESET << std::endl;
        std::cout << BOLDMAGENTA << it->first << RESET << std::endl;
        for (size_t i = 0; i < it->second.size(); i++)
        {
            std::cout << BOLDYELLOW << "Location " << i << RESET << std::endl;
            std::cout << it->second[i] << RESET << std::endl;
        }
    }
}