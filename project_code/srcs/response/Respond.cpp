/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Respond.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:35:48 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/01 23:53:21 by ymohamed         ###   ########.fr       */
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
	// size_t max_body;
	// if (server_info.find("MaxBodySize") != server_info.end())
	// 	max_body = atoi(server_info["MaxBodySize"].c_str());
	// else
	// 	max_body = MAX_BODY_SIZE;
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
	std::string cgi_path = isCGI(request);
	std::cout << BOLDGREEN << "cgi path = " << cgi_path << std::endl << RESET;
	if (cgi_path != "")
		response_string = responseCGI(request, server_info, cgi_path);
    if (request.find("GET") != request.end())
        response_string = normalGETResponse(request, server_info);
    else if (request.find("POST") != request.end())
    {
        Post apost(request, full_request, server_info);
		// apost.printPostHeader();
		// apost.printPostBody();
		apost.sendToBackend();
		response_string = apost.get_response();
		// apost.printReceivedRequestMapsour();
        // response_string = apost.get_response();
		std::cout << BOLDYELLOW << "responding to post: " << response_string << std::endl << RESET;
    }
    else if (request.find("DELETE") != request.end())
    {
        DELETE DELETE_response;
        response_string = DELETE_response.deleteResponseFiller(request, response, server_info);
    }
    else
	{
		ErrResponse err;
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

// this ugly syntax :&a[response_bytes_sent]
// because I am not able to save const char *a as private asset as the conversion change for each packet
//while it's constant, I will try to change it later inshalla, for now take the code below for granted
//till make the new sending scaling work for now
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
    if (response_bytes_sent == packet_len)
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
        std::cout << "1" << std::endl;
        if (servers[i]["Port"] == port)
        {
            std::cout << "2" << std::endl;
            nominated_servers.push_back(i);
        }
    }
    
    for (unsigned long i = 0; i < nominated_servers.size(); i++)
    {
        server_names =  split(servers[nominated_servers[i]]["server_name"], " ");
        for (unsigned long j = 0; j < server_names.size(); j++)
        {   
            std::vector<std::string> tmp_vec;
            if (request.find("Host:") == request.end() || request["Host:"].size() != 1)  
            {
                tmp_vec.push_back("400");
                tmp_vec.push_back("No host");
                response["Status-code"] = tmp_vec;
                
                return (servers[0]);
            }
            
            std::cout << "our hostname is " << request["Host:"][0] << std::endl;
            std::cout << "server hostname " << server_names[j] << std::endl;
            std::cout << "trimmed host = " << request["Host:"][0].substr(0, server_names[j].length()) << std::endl;
            if (server_names[j] == request["Host:"][0].substr(0, server_names[j].length()))
            {
                std::cout << "our server is " << nominated_servers[j] << std::endl;
                return (servers[nominated_servers[i]]);
            }
        }
    }
    return (servers[0]);
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