/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 16:31:00 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/12 19:01:07 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"

DELETE::DELETE()
{
    std::cout << "DELETE under construction" << std::endl;

}
DELETE::~DELETE()
{

}

std::string    DELETE::delete_response_filler(packet_map &request, response_packet &response, std::map<std::string, std::string> &server_info)

{

    fill_request_line(request, response);
    fill_path(request, response, "DELETE");
    return (fill_delete_response(response, server_info));
}

void    DELETE::fill_request_line(packet_map &request, response_packet &response)
{
    if ((request.find("POST") != request.end() || request.find("GET") != request.end())
        && fill_status_code(response, "400", "Invalid multiple methods inside GET"))
        return ;
    if ((request["DELETE"][0].length() + request["DELETE"][1].length()) > HEADER_MAX_LENGTH
            && fill_status_code(response, "414", "URI Too Long"))
        return ;
    if (!(request["DELETE"][1] == "HTTP/1.1")
        && (fill_status_code(response, "505", "version not supported")))
        return ;
    if ((request["DELETE"].size() != 2) 
        && (fill_status_code(response, "400", "DELETE vec has wrong number items bad request")))
        return ;
    else
        response["HTTP version"].push_back(request["DELETE"][1]);
    response["method"].push_back("DELETE");

}

std::string     DELETE::fill_delete_response(response_packet &response,std::map<std::string, std::string>  &server_info)
{
    if (response["Status-code"][0] != "200")
        return (err.code(server_info, response["Status-code"][0]));
    else if (response["Path"].size() < 2 && fill_status_code(response , "400", "bad file path format"))
        return (err.code(server_info, response["Status-code"][0]));
    else
        return (fill_ok_response(response, server_info)); 
}


std::string DELETE::fill_ok_response(response_packet &response, std::map<std::string, std::string> &server_info)
{
    //all the heavy lifting is here inshalla
    std::string path = construct_path(response, server_info);
    std::cout << "path to delete = " << path << std::endl;
    if (!sanitized_path(path, server_info))
        return (err.code(server_info, "403"));
    if (access(path.c_str(), F_OK) != 0)
    {
        std::cout << "file not exist" << std::endl;
        return (err.code(server_info, "404"));
    }
    if (std::remove(path.c_str()) != 0)
        return (err.code(server_info, "404"));
    return (successful_delete_packet());
}

std::string DELETE::successful_delete_packet()
{
    std::string success_delete = "HTTP/1.1 204 No Content\n";
    success_delete += "Date: " + err.get_timebuffer() +"\n";
    success_delete += "Server: Webserv 1.0\n";
    return (success_delete);
}
bool DELETE::sanitized_path(std::string path, std::map<std::string, std::string> &server_info)
{
    std::vector<std::string> malicous_inputs;
    malicous_inputs.push_back("..");
    malicous_inputs.push_back("*");
    malicous_inputs.push_back("!");
    malicous_inputs.push_back("~");
    malicous_inputs.push_back("//");
    for (std::vector<std::string>::iterator it = malicous_inputs.begin();  it != malicous_inputs.end(); ++it)
    {
        if (path.find(*it) != std::string::npos)
        {
            std::cout << "malicous part in path = <" << *it << ">" << std::endl;
            return (false);
        }
    }
    if (path.find(server_info["DELETE path"]) == std::string::npos)
        {
            return (false);
        }
    return (true);
}

std::string    DELETE::construct_path(response_packet &response , std::map<std::string, std::string> &server_info)
{

    std::string path = response["Path"][1];
    std::cout << "construcing path = " << path << std::endl ;
    if (path == "/")
        return (server_info[path]);
    if (std::count(path.begin(), path.end(), '/') < 2)
    {
        if (server_info.find(path) != server_info.end())
            return (server_info[path + " index"]);
        else
            return (server_info["root"] + path);
    }
    std::string dir = path.substr(0, path.substr(1, path.length()).find("/") + 1);
    std::cout << "dir == " << dir << " path = " << path << std::endl;
    // /images/ case
    if (path[path.length() - 1] == '/' && dir.length() == path.length() - 1)
    {
        std::cout << "yes it's only dir" << std::endl;
        if (server_info.find(dir) != server_info.end())
            return (server_info[dir + " index"]);
    }
    // images/cat.jpeg
    std::string rest_of_path = path.substr(dir.length() + 1, path.length());
    std::cout << "rest of path = " << rest_of_path << std::endl;
    if (server_info.find(dir) != server_info.end())
        return (server_info[dir] + rest_of_path);
    return (server_info["root"] + path);
}
