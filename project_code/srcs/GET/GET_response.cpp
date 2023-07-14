/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET_response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 16:33:09 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/14 04:00:59 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET_response.hpp"

GET_response::GET_response(response_type &res): reponse_check(res)
{
};

GET_response::~GET_response()
{}

std::string     GET_response::fill_get_response(std::map<std::string, std::string> &server_info)
{
    if (*(reponse_check["Status-code"].begin()) != "200")
        return (err.code(server_info, reponse_check["Status-code"][0]));
    else if (reponse_check["Path"].size() < 2 && fill_status_code(reponse_check , "400", "bad file path format"))
        return (err.code(server_info, reponse_check["Status-code"][0]));
    else
        fill_ok_response(server_info); 
    return (response_packet);
}
void        GET_response::fill_ok_response(std::map<std::string, std::string> &server_info)
{
    /*
    chunking or storing bytes on vector instead of string, or array of strings should happen
        here |
             v  
    */

    response_packet = "";

    // std::string file_path = DEFAULT_PATH + *(++reponse_check["Path"].begin())+ DEFAULT_LOCATION;
    std::string file_path = construct_path(server_info);
    std::cout << "requested file path = " << file_path << std::endl;
    if (!sanitized_path(file_path))
    {
        fill_status_code(reponse_check , "403", "malicous header!");
        response_packet = err.code(server_info, reponse_check["Status-code"][0]);
        return ;
    }
    std::cout << "constructed path = " << file_path << std::endl;

    std::ifstream infile(file_path.c_str());
    if (infile.fail())
    {
        std::cout << file_path << std::endl;
        fill_status_code(reponse_check , "403", "file not found ya basha!");
        response_packet = err.code(server_info, reponse_check["Status-code"][0]);
        if (response_packet.length() < 10000)
            std::cout << response_packet << std::endl;
        else
            std::cout << "inside GET_response has large response not gonna visualize\n";
        return ;
    }
    std::stringstream content_stream;
    content_stream  << infile.rdbuf();
    std::string full_file_to_string = content_stream.str();
    //checking file size in bytes
    // infile.seekg(0, std::ios::end);
    // int file_size = infile.tellg();
    response_packet = "HTTP/1.1 " + *(reponse_check["Status-code"].begin()) 
        + " " + *(++reponse_check["Status-code"].begin()) + "\r\n";
    response_packet += "Server: webserve/1.0\r\n";
    response_packet += "Date: ";
    response_packet += err.get_timebuffer();
    response_packet += "Content-Type: text/html\r\n";
	std::stringstream ss;
	ss << full_file_to_string.length();
    response_packet += "Content-Length: " + ss.str() + "\r\n\r\n";
    response_packet += full_file_to_string;
}

std::string    GET_response::construct_path(std::map<std::string, std::string> &server_info)
{

    std::string path = reponse_check["Path"][1];
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


bool GET_response::sanitized_path(std::string path)
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
    return (true);
}