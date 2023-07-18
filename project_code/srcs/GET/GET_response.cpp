/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET_response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 16:33:09 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/18 14:37:55 by ahsalem          ###   ########.fr       */
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
    response_packet = "";
    std::string file_path = construct_path(server_info);
    std::cout << "requested file path = " << file_path << std::endl;
    if (!sanitized_path(file_path)&& fill_bad_path(server_info))
        return ;
    std::cout << "constructed path = " << file_path << std::endl;
    DIR *dir;
    std::string full_file_to_string;
    if ((dir  = opendir(file_path.c_str())) != NULL)
    {
        struct dirent *files;
        std::vector<std::string> ls;
        ls.push_back(file_path);
        while ((files = readdir(dir)) != NULL)
            ls.push_back(files->d_name);
        construct_dir_response(ls, full_file_to_string);
    }
    else
    {
        std::ifstream infile(file_path.c_str());
        if (infile.fail() && fill_bad_path(server_info))
            return ;
        std::stringstream content_stream;
        content_stream  << infile.rdbuf();
        full_file_to_string = content_stream.str();
    }
    filling_response_packet(full_file_to_string);
}

void    GET_response::construct_dir_response(std::vector<std::string> &ls,
    std::string &full_file_to_string)
{
    std::string file_name = ls[0].substr(ls[0].rfind("/"), ls[0].length());
    full_file_to_string = "<!DOCTYPE html>";
    full_file_to_string += "<html>";
    full_file_to_string += "<head>";
    full_file_to_string += "    <title>Index of" +  file_name + "</title>";
    full_file_to_string += "</head>";
    full_file_to_string += "<body>";
    full_file_to_string += "  <h1>Index of " + file_name +"</h1>";
    full_file_to_string += "<ul>";
    full_file_to_string += "    <ul>";
    for (std::vector<std::string>::iterator it = ++ls.begin();
        it != ls.end(); ++it) 
        full_file_to_string += "        <li>" + *it +"</li>";
    full_file_to_string += "    </ul>";
    full_file_to_string += "</body>";
    full_file_to_string += "</html>";
    
}
void     GET_response::filling_response_packet(std::string &full_file_to_string)
{
    response_packet = "HTTP/1.1 200 OK \r\n";
    response_packet += "Server: webserve/1.0\r\n";
    response_packet += "Date: ";
    response_packet += err.get_timebuffer();
    response_packet += "Content-Type: text/html\r\n";
	std::stringstream ss;
	ss << full_file_to_string.length();
    response_packet += "Content-Length: " + ss.str() + "\r\n\r\n";
    response_packet += full_file_to_string;
    
}

bool    GET_response::fill_bad_path(std::map<std::string, std::string> &server_info)
{
        fill_status_code(reponse_check , "403", "file not found ya basha!");
        response_packet = err.code(server_info, reponse_check["Status-code"][0]);
        vis_str(response_packet, "inside GET_response has large response not gonna visualize\n");
        return (true);

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