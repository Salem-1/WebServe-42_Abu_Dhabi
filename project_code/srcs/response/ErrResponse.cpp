/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:21:17 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/27 13:00:08 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrResponse.hpp"

ErrResponse::ErrResponse()
{
    statusCodes();   
};
ErrResponse::~ErrResponse()
{
    
};

std::string ErrResponse::code(
        stringmap &server_info, std::string err)
{
    //err = 404
    //server_info[err] = file_name
    std::string dir = server_info["constructed path dir"];
    std::cout << "inside dir " << dir << std::endl;
    // visualize_string_map(server_info);
    if (!inMap(server_info, dir + " error_page"))
    {
       if (!inMap(server_info, "/ error_page")) 
        {
            std::cout << "didn't foudn err " << server_info[err] << std::endl;
            // exit(0);
            return (erroredResponse(err));
        }
        else
            dir = "/";
    }
    std::string error_path = constructErroPath(server_info[dir + " error_page"], err);
    std::ifstream infile(error_path.c_str());
    std::cout << error_path.c_str() << std::endl;
    std::cout << "dir inside err response = " << dir  << std::endl;
    std::cout << "server_info[err]0 = " << error_path << std::endl;
    if (infile.fail())
    {
        std::cout << "failed opening the file \n";
        return (erroredResponse(err));
    }
    std::cout << "success open the file \n";
    std::stringstream content_stream;
    content_stream  << infile.rdbuf();
    std::string full_file_to_string = content_stream.str();
    return (constructCustomErrPacket(err, full_file_to_string));
}




std::string ErrResponse::constructErroPath(std::string server_eror_info, std::string err)
{
    std::vector<std::string > error_pages;
    std::vector<std::string> single_error_pgae;
    error_pages = split(server_eror_info, " , ");
 
    for (std::vector<std::string>::iterator it = error_pages.begin()
        ; it != error_pages.end(); ++it)
    {
        std::cout << *it << std::endl;
        single_error_pgae = split(*it, " ");
        std::cout << single_error_pgae[0] << std::endl;
        std::cout << single_error_pgae[1] << std::endl;
        if (single_error_pgae.size() != 2)
            return ("bad single page error lenght");
std::cout << "comparing <" << err << "> to <" << single_error_pgae[0] << ">" << std::endl;
        if (err == single_error_pgae[0])
            return (single_error_pgae[1]);
    }
        return ("bad error path");
    
}
std::string ErrResponse::constructCustomErrPacket(std::string err, std::string & full_file_to_string)
{
      response_str = "HTTP/1.1 " + err 
        + " " + StatusCodes[err] + "\r\n";
    response_str += "Server: Phantoms\r\n";
    response_str += "Date: ";
    response_str += getTimeBuffer();
    response_str += "Content-Type: text/html\r\n";
	std::stringstream ss;
	ss << full_file_to_string.length();
    response_str += "Content-Length: " + ss.str() + "\r\n\r\n";
    response_str += full_file_to_string;
    return (response_str);
}

std::string ErrResponse::erroredResponse(std::string err)
{
    if (!readFileToString("srcs/response/defualt_error.html", 
                                        full_file_to_string))
    {
        std::cout << "couldn't open file" << std::endl;
        return (regularErroredResponse(err));
    }
    std::string err_message =  err + " " + StatusCodes[err] ;
    full_file_to_string.replace(full_file_to_string.find("<h1 class=\"game-title\">error_message_here</h1>"), 47, "<h1 class=\"game-title\">" +err_message + "</h1>");
      response_str = "HTTP/1.1 " + err
        + " " + StatusCodes[err] + "\r\n";
    response_str += "Server: Phantoms\r\n";
    response_str += "Date: ";
    response_str += getTimeBuffer();
    response_str += "Content-Type: text/html text/javascript test/css; charset=utf-8\r\n";
    std::stringstream ss;
	ss << full_file_to_string.length();
    
    response_str += "Content-Length: " + ss.str() + "\r\n\r\n";
    response_str += full_file_to_string;
    return (response_str);
}
std::string ErrResponse::regularErroredResponse(std::string err)
{    
    response_str = "HTTP/1.1 " + err
        + " " + StatusCodes[err] + "\r\n";
    response_str += "Server: Phantoms\r\n";
    response_str += "Date: ";
    response_str += getTimeBuffer();
    response_str += "Content-Type: text/html text/javascript test/css; charset=utf-8\r\n";
    response_str += "Content-Length: 1050\r\n\r\n";
    response_str += "<!DOCTYPE html>\r\n";
    response_str += "<html>\r\n";
    response_str += "<head>\r\n";
    response_str += "    <title>";
    response_str += err ;
    response_str += " " + StatusCodes[err]+ " </title>\r\n";
    response_str += "</head>\r\n";
    response_str += "<body>\r\n";
    response_str += "    <h1>error under construction " + err + " " + StatusCodes[err] + "</h1>\r\n";
    response_str += "</body>\r\n";
    response_str += "</html>\r\n";
    response_str += "<p>\r\n";
    response_str += "   ---- &&& &&  &amp; &amp; <>\r\n\n";
    response_str += "    &nbsp;&nbsp;&nbsp;&nbsp;  && &amp;--&amp;-|&amp; (🍎)|- @, &amp;&amp; <br>\r\n";
    response_str += "    &nbsp;&nbsp;&nbsp;&nbsp;  &amp;--(-&amp;&amp;-||-&amp; -_-)_&amp;-_&amp;<br>\r\n";
    response_str += "    &nbsp; &amp;(🍎) &amp;--&amp;|(🍎)|-&amp;-- '% &amp; ()🍌🐒<br>\r\n";
    response_str += "    &nbsp;&amp;_-&amp;_&amp;&amp;_- |&amp; |&amp;&amp;-&amp;__%_-_&amp;&amp; <br>\r\n";
    response_str += "    &&&   && &amp; &amp;| &amp;| -&amp; &amp;% ()&amp; -&&<br>\r\n";
    response_str += "    &nbsp;(🍎)&_---(🍎)&amp;-&amp;-|&amp;&amp;-&amp;&amp;--%---(🍎)~<br>\r\n";
    response_str += "    &nbsp;&nbsp;&nbsp;&nbsp; &&     -|||<br>\r\n";
    response_str += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\r\n";
    response_str += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\r\n";
    response_str += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\r\n";
    response_str += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\r\n";
    response_str += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; , -=-~  .-^- _<br>\r\n";
    response_str += "</p>\n";

    return (response_str);
}



void    ErrResponse::statusCodes()
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
    StatusCodes["413"] =  "Request Entity Too Large";
    StatusCodes["500"] =  "Internal Server Error";
    StatusCodes["501"] =  "Not Implemented";
    StatusCodes["502"] =  "Bad Gateway";
    StatusCodes["503"] =  "Service Unavailable";
    StatusCodes["504"] =  "Gateway Timeout";
}

std::string	getTimeBuffer() {
	std::time_t current_time = std::time(NULL);
	std::tm* time_info = std::gmtime(&current_time);

	char time_buffer[80];
	std::strftime(time_buffer, sizeof(time_buffer), "%a, %d %b %Y %H:%M:%S GMT\r\n", time_info);

	return std::string(time_buffer);
}


bool    ErrResponse::readFileToString(std::string file_path, 
                                        std::string &full_file_to_string)
{
    std::ifstream infile(file_path.c_str());
    if (infile.fail())
        return (false);
    std::stringstream content_stream;
    content_stream  << infile.rdbuf();
    full_file_to_string = content_stream.str();
    std::cout << full_file_to_string << std::endl;
    return (true);
}