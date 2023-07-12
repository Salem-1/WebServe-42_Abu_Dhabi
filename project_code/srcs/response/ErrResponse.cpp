/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:21:17 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/12 18:42:12 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrResponse.hpp"

ErrResponse::ErrResponse()
{
    status_codes();   
};
ErrResponse::~ErrResponse()
{
    
};

std::string ErrResponse::code(
        std::map<std::string, std::string> &server_info, std::string err)
{
    if (server_info.find(err) == server_info.end())
    {
        std::cout << "didn't foudn err " << server_info[err] << std::endl;
        return (errored_response(err));
    }
    std::ifstream infile(server_info[err].c_str());
    std::cout << server_info[err].c_str() << std::endl;
    if (infile.fail())
    {
        std::cout << "failed opening the file \n";
        return (errored_response(err));
    }
    std::cout << "success open the file \n";
    std::stringstream content_stream;
    content_stream  << infile.rdbuf();
    std::string full_file_to_string = content_stream.str();
    return (construct_custom_err_packet(err, full_file_to_string));
}

std::string ErrResponse::construct_custom_err_packet(std::string err, std::string & full_file_to_string)
{
      response_packet = "HTTP/1.1 " + err 
        + " " + StatusCodes[err] + "\n";
    response_packet += "Server: webserve/1.0\n";
    response_packet += "Date: ";
    response_packet += get_timebuffer();
    response_packet += "Content-Type: text/html\n";
	std::stringstream ss;
	ss << full_file_to_string.length();
    response_packet += "Content-Length: " + ss.str() + "\n\n";
    response_packet += full_file_to_string;
    return (response_packet);
}
std::string ErrResponse::errored_response(std::string err)
{
    response_packet = "HTTP/1.1 " + err
        + " " + StatusCodes[err] + "\n";
    response_packet += "Server: webserve/1.0\n";
    response_packet += "Date: ";
    response_packet += get_timebuffer();
    response_packet += "Content-Type: text/html text/javascript test/css; charset=utf-8\n";
    response_packet += "Content-Length: 1050\n\n";
    response_packet += "<!DOCTYPE html>\n";
    response_packet += "<html>\n";
    response_packet += "<head>\n";
    response_packet += "    <title>";
    response_packet += err ;
    response_packet += " " + StatusCodes[err]+ " </title>\n";
    response_packet += "</head>\n";
    response_packet += "<body>\n";
    response_packet += "    <h1>error under construction " + err + " " + StatusCodes[err] + "</h1>\n";
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

void    ErrResponse::status_codes()
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

std::string ErrResponse::get_timebuffer() {
    std::time_t current_time = std::time(NULL);
    std::tm* time_info = std::gmtime(&current_time);

    char time_buffer[80];
    std::strftime(time_buffer, sizeof(time_buffer), "%a, %d %b %Y %H:%M:%S GMT\n", time_info);

    return std::string(time_buffer);
}
