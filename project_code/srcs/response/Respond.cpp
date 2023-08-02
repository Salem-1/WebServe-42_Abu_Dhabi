/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Respond.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:35:48 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/02 08:22:05 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Respond.hpp"

Respond::Respond()
{};

Respond::Respond(int    client_socket) :client_socket(client_socket),  sending(false), response_bytes_sent(0)
{
}

Respond::~Respond()
{
}

void    Respond::respond(packet_map &request, t_request &full_request,  conf &servers, std::string port)
{
    
    //here should extract the port and hostname to give to the corresponding method
    stringmap server_info = getServerInfo(request, servers, port);
    visualize_string_map(server_info);
    // exit(0);
    fillResponse(request, full_request, server_info);
    sending = true;
    
    std::cout << "We have ready response" << std::endl;
}

void    Respond::flushResponse()
{
    response.clear();
    response_string = "";
    response_bytes_sent = 0;
    sending = false;

}

void    Respond::fillResponse(packet_map &request, t_request &full_request, stringmap &server_info)
{
    if ((response.find("Content-Length:") != response.end() && response.find("Transfer-Encoding:") != response.end())
        || checkPoisonedURL(request))
    {
        fillStatuCode("400", "request method not supported");
        return ;
    }
    response["Status-code"].push_back("200");
	// std::string cgi_path = isCGI(request);
	// std::cout << BOLDGREEN << "cgi path = " << cgi_path << std::endl << RESET;
    
	std::vector<std::string> supported_methods;
    // if (cgi_path != "")
	// 	response_string = responseCGI(request, server_info, cgi_path);
    //@AHMED MAHDI, check if you need to make this if -> else if, as this can return normal GET request even after filling the reponse with CGI
    ErrResponse err;
    std::string msg;
    if (request.find("GET") != request.end())
    {
        fillSupportedMethods(supported_methods, server_info, "GET" ,  request);
        if(!(isSupportedMethod("GET", supported_methods)))
        {
            msg =  ":( GET is not supported method for "  + response["dir"][0];
            print_to_file("/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/logs/dirs.txt", msg);
            response_string = err.code(server_info, "405");
            return ;
        }   
            msg =  ":) GET is Allowed method for "  + response["dir"][0];
            print_to_file("/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/logs/dirs.txt", msg);
        //@ Ahmed MAHDI also can you put the CGI check here    
        response_string = normalGETResponse(request, server_info);
        std::cout << "redirection packet = " << response_string;

    }
    else if (request.find("POST") != request.end())
    {
        fillSupportedMethods(supported_methods, server_info, "POST" ,  request);
        if(!(isSupportedMethod("POST", supported_methods)))
                 {
            msg =  ":( POST is not supported method for "  + response["dir"][0];
            print_to_file("/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/logs/dirs.txt", msg);
            response_string = err.code(server_info, "405");
            return ;
        } 
            msg =  ":) POST is Allowed method for "  + response["dir"][0];
            print_to_file("/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/logs/dirs.txt", msg);
        Post apost(request, full_request, server_info);
		apost.printPostHeader();
		apost.printPostBody();
		apost.printReceivedRequestMap();
        // response_string = apost.get_response();
    }
    else if (request.find("DELETE") != request.end())
    {
        fillSupportedMethods(supported_methods, server_info, "DELETE" ,  request);
        if(!(isSupportedMethod("DELETE", supported_methods)))
                 {
            msg =  ":( DELETE is not supported method for "  + response["dir"][0];
            response_string = err.code(server_info, "405");
            return ;
        } 
            msg =  ":) DELETE is Allowed method for "  + response["dir"][0];
            response_string = err.code(server_info, "405");
        DELETE DELETE_response;
        response_string = DELETE_response.deleteResponseFiller(request, response, server_info);
    }
    else
	{
		response_string = err.code(server_info, "501");
	}
};

std::string     Respond::normalGETResponse(packet_map &request, stringmap &server_info)
{
    GET GET_fill(request, response);
    GET_fill.prepareGetResponse(server_info);
    return (GET_response(GET_fill.response).fillGetResponse(server_info));
}

int Respond::checkPoisonedURL(packet_map &request)
{
    if (request.find("GET") != request.end())
    {
		if (request["GET"].size() != 2)
			return (0);
        if (request["GET"][0].find(" ") != std::string::npos)
            return (1);
    }
    if (request.find("POST") != request.end())
    {
        if (request["POST"][0].find(" ") != std::string::npos)
             return (1);
    }
    if (request.find("DELETE") != request.end())
    {
        if (request["DELETE"][0].find(" ") != std::string::npos)
          return (1);
    }
    return (0);
}


void    Respond::visualizeResponse()
{
    std::cout << BOLDRED << "\nVisualizing reponse API\n" << std::endl;
    std::cout << "{" << std::endl << RESET;
    for (response_packet::iterator it = response.begin(); it != response.end(); it++)
    {
        if ((it->first).length() < 10000)
            std::cout << BOLDBLUE << "  \"" << it->first << "\": [" << RESET;
        else
            std::cout << "\"" << "large packet not gonna visualize" << "\", ";

         for (std::vector<std::string>::iterator vit = it->second.begin(); vit != it->second.end(); ++vit)
        {
            if ((*vit).length() < 10000)
                std::cout << "\"" << *vit << "\", ";
            else
                std::cout << "\"" << "large packet not gonna visualize" << "\", ";
        }
        std::cout << BOLDBLUE << "]" << std::endl << RESET;
    }
    std::cout << BOLDRED << "}" << std::endl << RESET;
}

void    Respond::sendAll(connection_state &state)
{
    size_t  packet_len = response_string.length(); 
    std::cout << "inside send all" << std::endl;
    const char *a = response_string.c_str();
    int send_ret = 0;
    // sending = true ;
    visualizeResponse();
    vis_str(response_string, "inside send all");
    if (packet_len - response_bytes_sent > BUFFER_SIZE)
        send_ret += send(client_socket, &a[response_bytes_sent], BUFFER_SIZE, 0);
    else
        send_ret += send(client_socket, &a[response_bytes_sent], packet_len - response_bytes_sent, 0);  
    std::cout << send_ret << " bytes sent \n";
    response_bytes_sent += send_ret;
    if (send_ret <= 0)
    {
        perror("send failed");
        flushResponse();
        state = KILL_CONNECTION;
    }
    if (response_bytes_sent >= packet_len)
    {
        state = KILL_CONNECTION;
        flushResponse();
    }
}

int Respond::fillStatuCode(std::string status_code, std::string message)
{
    response["Status-code"].clear();
    response["Status-code"].push_back(status_code);
    response["Status-code"].push_back(message);
    return (1);
}

stringmap  Respond::getServerInfo(packet_map &request,conf &servers, std::string port)
{
    std::vector<int>            nominated_servers;
    std::vector<std::string>    server_names;
    std::cout << BOLDGREEN << "port  = " << port << std::endl;
    unsigned long n = servers.size();
    std::cout << "we have " << n << "servers\n" << RESET;
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        std::cout << "config port = " << servers[i]["Port"] << std::endl ;
        if (servers[i]["Port"] == port)
            nominated_servers.push_back(i);
    }
    
    for (unsigned long i = 0; i < nominated_servers.size(); i++)
    {
        server_names =  split(servers[nominated_servers[i]]["server_name"], " ");
        for (unsigned long j = 0; j < server_names.size(); j++)
        {   
            if (request.find("Host:") == request.end() || request["Host:"].size() != 1)  
            {
                fillStatuCode("400", "No host");   
                return (servers[nominated_servers[0]]);
            } 
            std::cout << "our hostname is " << request["Host:"][0] << std::endl;
            std::cout << "server hostname " << server_names[j] << std::endl;
            std::string requested_host;
            if (request["Host:"][0].find(":") == std::string::npos || request["Host:"][0].find(":") == request["Host:"][0].length())
                requested_host  = request["Host:"][0];
            else
                requested_host  = request["Host:"][0].substr(0, request["Host:"][0].find(":"));
            std::cout << "requested trimmed host = " << requested_host << std::endl;
            std::cout << "requested port  = " << port  << std::endl;
            std::pair<std::string, std::string> host_port(requested_host, port); 
            if (server_names[j] == requested_host)
            {
                std::cout << "our server is " << nominated_servers[j] << std::endl;
                return (servers[nominated_servers[i]]);
            }
        }
    }
    return (servers[nominated_servers[0]]);
}

std::string	Respond::isCGI(packet_map &request)
{
	packet_map::iterator it = request.find("GET");
	if (it == request.end())
		it = request.find("POST");
	if (it == request.end())
		return ("");
	if (it->second[0].find("cgi-bin") != std::string::npos)
		return (it->second[0]);
	return ("");
}

std::string Respond::responseCGI(packet_map &request, stringmap &server_info, std::string &cgi_path)
{
	(void) request;
    std::cout << "inside response CGI" << std::endl;
	if (cgi_path.find("?") != std::string::npos)
	{
		std::string query = cgi_path.substr(cgi_path.find("?") + 1, cgi_path.length() - 1);
		cgi_path = cgi_path.substr(0, cgi_path.find("?"));
		// server_info["query"] = query;
	}
	std::string full_cgi_path = server_info["root"] + cgi_path;
	std::cout << BOLDGREEN << "full path = " << full_cgi_path << std::endl << RESET;
	return "";
}


bool    Respond::isSupportedMethod(std::string method, std::vector<std::string> &supported_methods)
{
    if(supported_methods.empty())
        return (true);
    for (std::vector<std::string>::iterator it = supported_methods.begin();
            it != supported_methods.end(); ++it)
    {
        if (*it == method)
            return (true);
    }

    return (false);
}

void   Respond::fillSupportedMethods(
                    std::vector<std::string> &supported_methods, stringmap &server_info
                    , std::string method, packet_map& request)
{
    supported_methods.clear();
    fillPath(request, response, method);
    if (response["Status-code"][0] != "200")
        return ;
    for (size_t i = 0; i < response["Path"].size() ; ++i)
        std::cout << "response['Path'][" << i << "] = " << response["Path"][i] << std::endl;
    std::string path = response["Path"][1];

    std::cout << "Given path = " << path << std::endl;
    std::cout << MAGENTA << "construcing path = " << path << std::endl << RESET ;
    
    std::string dir;
    if (path == "/")
        dir = path;
    else if (std::count(path.begin(), path.end(), '/') > 1)
        dir = path.substr(0, path.find("/", path.find("/") + 1));
    else if (path.find(".") != std::string::npos)
        dir = "/";
    else if (path.find("?") == std::string::npos)
        dir = path;
    else
        dir = path.substr(0, path.find("?" - 1));
    response["dir"].push_back(dir);
    std::string msg =   "dir = " +  dir + "             path = " + path;
    print_to_file("/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/logs/out.txt", dir);
    std::string allowed_methods = dir + " methods";
    if (server_info.find(allowed_methods) != server_info.end())
    {
        if (server_info[allowed_methods].empty())
            supported_methods.push_back("nothing");
        else
            supported_methods = split(server_info[allowed_methods], " ");
        return ;
    }
    supported_methods.push_back("GET");
    supported_methods.push_back("POST");
    supported_methods.push_back("DELETE");
    // supported_methods.push_back("PUT");
}