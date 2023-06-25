/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 16:31:00 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/25 17:01:48 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE.hpp"

DELETE::DELETE()
{
    std::cout << "DELETE under construction" << std::endl;
    status_codes();
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
        return (errored_response(response["Status-code"][0], StatusCodes[response["Status-code"][0]]));
    else if (response["Path"].size() < 2 && fill_status_code(response , "400", "bad file path format"))
        return (errored_response(response["Status-code"][0], StatusCodes[response["Status-code"][0]]));
    else
        return (fill_ok_response(response, server_info)); 
}


std::string DELETE::fill_ok_response(response_packet &response, std::map<std::string, std::string> &server_info)
{
    //all the heavy lifting is here inshalla
    std::string path = construct_path(response, server_info);
    std::cout << "path to delete = " << path << std::endl;
    if (!sanitized_path(path))
        return (errored_response("403", StatusCodes["403"]));
    if (access(path.c_str(), F_OK) != 0)
    {
        std::cout << "file not exist" << std::endl;
        return (errored_response("404", StatusCodes["404"]));
    }
    if (std::remove(path.c_str()) != 0)
        return (errored_response("404", StatusCodes["404"]));
    return (successful_delete_packet());
}

std::string DELETE::successful_delete_packet()
{
    std::string success_delete = "HTTP/1.1 204 No Content\n";
    success_delete += "Date: " + get_timebuffer() +"\n";
    success_delete += "Server: Webserv 1.0\n";
    return (success_delete);
}
bool DELETE::sanitized_path(std::string path)
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

std::string    DELETE::construct_path(response_packet &response , std::map<std::string, std::string> &server_info)
{

    std::string path = response["Path"][1];
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
std::string DELETE::errored_response(std::string error_code, std::string error_message)
{
   std::string response_packet = "HTTP/1.1 " + error_code 
        + " " + error_message + "\n";
    response_packet += "Server: webserve/1.0\n";
    response_packet += "Date: ";
    response_packet += get_timebuffer();
    response_packet += "Content-Type: text/html text/javascript test/css; charset=utf-8\n";
    response_packet += "Content-Length: 1050\n\n";
    response_packet += "<!DOCTYPE html>\n";
    response_packet += "<html>\n";
    response_packet += "<head>\n";
    response_packet += "    <title>";
    response_packet += error_code ;
    response_packet += " " + error_message + "</title>\n";
    response_packet += "</head>\n";
    response_packet += "<body>\n";
    response_packet += "    <h1>error under construction " + error_code + " " + error_message + "</h1>\n";
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


void    DELETE::status_codes()
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

std::string DELETE::get_timebuffer() {
    std::time_t current_time = std::time(nullptr);
    std::tm* time_info = std::gmtime(&current_time);

    char time_buffer[80];
    std::strftime(time_buffer, sizeof(time_buffer), "%a, %d %b %Y %H:%M:%S GMT\n", time_info);

    return std::string(time_buffer);
}