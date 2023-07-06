/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET_response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 16:33:09 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/04 21:27:31 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET_response.hpp"

GET_response::GET_response(response_type res): reponse_check(res)
{
    void    status_codes();
};

GET_response::~GET_response()
{}

std::string     GET_response::fill_get_response(std::map<std::string, std::string> &server_info)
{
    if (*(reponse_check["Status-code"].begin()) != "200")
        return (errored_response());
    else if (reponse_check["Path"].size() < 2 && fill_status_code(reponse_check , "400", "bad file path format"))
        return (errored_response());
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
        errored_response();
        return ;
    }
    std::cout << "constructed path = " << file_path << std::endl;

    std::ifstream infile(file_path.c_str());
    if (infile.fail())
    {
        std::cout << file_path << std::endl;
        fill_status_code(reponse_check , "403", "file not found ya basha!");
        errored_response();
        return ;
    }
    std::stringstream content_stream;
    content_stream  << infile.rdbuf();
    std::string full_file_to_string = content_stream.str();
    //checking file size in bytes
    // infile.seekg(0, std::ios::end);
    // int file_size = infile.tellg();
    response_packet = "HTTP/1.1 " + *(reponse_check["Status-code"].begin()) 
        + " " + *(++reponse_check["Status-code"].begin()) + "\n";
    response_packet += "Server: webserve/1.0\n";
    response_packet += "Date: ";
    response_packet += get_timebuffer();
    response_packet += "Content-Type: text/html\n";
    response_packet += "Content-Length: " + std::to_string(full_file_to_string.length()) + "\n\n";
    response_packet += full_file_to_string;
}

std::string    GET_response::construct_path(std::map<std::string, std::string> &server_info)
{

    std::string path = reponse_check["Path"][1];
    std::cout << "construcing path = " << path << std::endl ;
    if (path == "/")
        return (server_info[path]);
    if (std::string::difference_type n = std::count(path.begin(), path.end(), '/') < 2)
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


std::string GET_response::get_timebuffer() {
    std::time_t current_time = std::time(nullptr);
    std::tm* time_info = std::gmtime(&current_time);

    char time_buffer[80];
    std::strftime(time_buffer, sizeof(time_buffer), "%a, %d %b %Y %H:%M:%S GMT\n", time_info);

    return std::string(time_buffer);
}

std::string GET_response::errored_response()
{
    response_packet = "HTTP/1.1 " + *(reponse_check["Status-code"].begin()) 
        + " " + *(++reponse_check["Status-code"].begin()) + "\n";
    response_packet += "Server: webserve/1.0\n";
    response_packet += "Date: ";
    response_packet += get_timebuffer();
    response_packet += "Content-Type: text/html text/javascript test/css; charset=utf-8\n";
    response_packet += "Content-Length: 1050\n\n";
    response_packet += "<!DOCTYPE html>\n";
    response_packet += "<html>\n";
    response_packet += "<head>\n";
    response_packet += "    <title>";
    response_packet += reponse_check["Status-code"][0] ;
    response_packet += " " + StatusCodes[reponse_check["Status-code"][0]]+ "</title>\n";
    response_packet += "</head>\n";
    response_packet += "<body>\n";
    response_packet += "    <h1>error under construction "+ reponse_check["Status-code"][0] + StatusCodes[reponse_check["Status-code"][0]] + "</h1>\n";
    response_packet += "</body>\n";
    response_packet += "</html>\n";
    response_packet += "<p>\n";
    response_packet += " -------- &&& &&  &amp; &amp; <br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;  && &amp;--&amp;-|&amp; (🍎)|- @, &amp;&amp; <br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;  &amp;--(-&amp;&amp;-||-&amp; -_-)_&amp;-_&amp;<br>\n";
    response_packet += "    &nbsp; &amp;(🍎) &amp;--&amp;|(🍎)|-&amp;-- '% &amp; ()🍌🐒<br>\n";
    response_packet += "    &nbsp;&amp;_-&amp;_&amp;&amp;_- |&amp; |&amp;&amp;-&amp;__%_-_&amp;&amp; <br>\n";
    response_packet += "    &&&   && &amp; &amp;| &amp;| -&amp; &amp;% ()&amp; -&&<br>\n";
    response_packet += "    &nbsp;(🍎)&_---(🍎)&amp;-&amp;-|&amp;&amp;-&amp;&amp;--%---(🍎)~<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp; &&     -|||<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; , -=-~  .-^- _<br>\n";
    response_packet += "</p>\n";

    return (response_packet);
}


void    GET_response::status_codes()
{
    StatusCodes["100"] =  "Continue";
    StatusCodes["101"] =  "Switching Protocols";
    StatusCodes["200"] =  "OK";
    StatusCodes["201"] =  "Created";
    StatusCodes["202"] =  "Accepted";
    StatusCodes["204"] =  "No Content";
    StatusCodes["300"] =  "Multiple Choices";
    StatusCodes["301"] =  "Moved Permanently";
    StatusCodes["302"] =  "Found";
    StatusCodes["304"] =  "Not Modified";
    StatusCodes["400"] =  "Bad Request";
    StatusCodes["401"] =  "Unauthorized";
    StatusCodes["403"] =  "Forbidden";
    StatusCodes["404"] =  "Not Found";
    StatusCodes["405"] =  "Method Not Allowed";
    StatusCodes["500"] =  "Internal Server Error";
    StatusCodes["501"] =  "Not Implemented";
    StatusCodes["502"] =  "Bad Gateway";
    StatusCodes["503"] =  "Service Unavailable";
    StatusCodes["504"] =  "Gateway Timeout";
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