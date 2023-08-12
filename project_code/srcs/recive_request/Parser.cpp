/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:41:21 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/12 16:45:34 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"


Parser::Parser(): read_again(0), bytes_read(0), read_sock(0), packet_counter(0), is_post(false)
{
	Parser::full_request.body_content_length = 0;
	Parser::full_request.request_is_valid = 1;
	Parser::full_request.header = "";
	Parser::full_request.body = "";
	Parser::body_start_pos = 0;
	fullbody = false;
	fullheader = false;
	ischunked = false;
	ischunkbody = false;
	chunklen = 0;
    fillValidHeaders();
}


Parser::Parser(const Parser &obj2)
{
    *this = obj2;
}

Parser &Parser::operator= (const Parser &obj2)
{
    if (this != &obj2)
    {

    }
    return (*this);
}
Parser::~Parser()
{

}

void    Parser::flushParsing()
{
        read_again = 0;
        reponse_packet = ""; 
        packet_counter++;
        request.clear();
        response.clear();
        filled_response = "";
}


void    Parser::parse(char *new_buffer)
{
	if (fullheader == false)
    	flushParsing();
    if (!bytes_read)  
    {
        read_again = 0;
        return ;
    }
    packet += new_buffer;
    
    vis_str(new_buffer, "new_buffer inside parser");
    vis_str(packet, "packet inside parser");
    
    if (fullheader == false)
	{
		if (((packet.find("\r\n\r\n") != std::string::npos || packet.find("\n\n") != std::string::npos ))
			|| earlyBadRequest(packet))
		{
			std::cout << "\nraw packet is\n-----------\n" << packet << "\n --------" << std::endl;
			body_start_pos = packet.find("\r\n\r\n") + 4;
			if (body_start_pos == std::string::npos + 4)
				body_start_pos = packet.find("\n\n") + 2;
			if (body_start_pos != std::string::npos + 2)
			{
				full_request.header = packet.substr(0, body_start_pos);
				fillHeaderRequest(full_request.header);
				fullheader = true;
			}
			full_request.request_is_valid = checkHeaders();
			if (Parser::request.find("GET") != request.end() 
				||  Parser::request.find("DELETE") != request.end())
			{
				read_again = 0;
				return ;
			}
		}
	}
	if (Parser::fullheader == true && Parser::fullbody == false && full_request.request_is_valid)
	{
		packet_map::iterator it = Parser::request.find("content-length:");
		if (it != request.end() && full_request.request_is_valid)
		{
			std::istringstream iss(it->second[0]);
			iss >> full_request.body_content_length;
		}
		else if (Parser::request.find("Transfer-Encoding:") != request.end())
			Parser::ischunked = Parser::request.find("Transfer-Encoding:")->second[0] == "chunked";
		if ((!full_request.body_content_length && !Parser::ischunked ) 
							|| full_request.body_content_length > MAX_BODY_SIZE)
		{
			if (full_request.body_content_length > MAX_BODY_SIZE)
				throw(std::runtime_error("400"));
			read_again = 0;
			fullbody = 1;
		}
		  else
		{
			std::cout << "in-complete packet let's read again\n";
			read_again = 1;
			Parser::fillBodyRequest();
		}
    }
    if (full_request.header.length() > HEADER_MAX_LENGTH || full_request.request_is_valid == 0)
    {
        std::cout << "Reject packet at parser" << std::endl;
        //rejecting packet and close socket
        read_again = 0;
        bytes_read = 0;
        return ;
    }
	if ((fullheader && fullbody) || full_request.request_is_valid == 0)
		read_again = 0;
	else
		read_again = 1;
}

bool Parser::earlyBadRequest(std::string packet)
{
    if (packet.length() >= 1)
    {
		char a = packet[0];
        if (a == 'G' || a == 'P' || a == 'D' || a == 'H')
            return false;
    }
    throw(std::runtime_error("400"));;
}

void    Parser::setBytereadAndReadsock(int bytes, int sock)
{
    this->bytes_read = bytes;
    this->read_sock = sock;
}

void    Parser::fillHeaderRequest(std::string packet)
{
    std::vector<std::string> tmp_vec;
    std::string              header;
    std::vector<std::string> packet_lines = split(packet, "\r\n");

    for (std::vector<std::string>::iterator it = packet_lines.begin(); it != packet_lines.end(); it++)
    {
        tmp_vec = split(*it, " ");
        if (tmp_vec.size() < 1)
			throw(std::runtime_error("400"));
        header = tmp_vec[0];
        tmp_vec.erase(tmp_vec.begin());
        //check for repetetion  in headers
        if (request.find(header) == request.end())
            request[header] = tmp_vec;
        else
			throw(std::runtime_error("400"));
    }
}

int	Parser::chunkLength(std::string buffer)
{
	std::istringstream	strlen(buffer);
	int 				len = 0;
	size_t				i = 0;
	strlen >> len;		 
	for (i = 0; std::isdigit(buffer[i]);  ++i) { }
	if (buffer.substr(i) == "\r\n" && len <= MAX_BODY_SIZE)
		return len;
	return -1;
}

std::string Parser::parseChunks(const std::string &s, const std::string &delimiter)
{
	std::string result;
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	size_t value;

	for (size_t i = 0; (pos_end = s.find(delimiter, pos_start)) != std::string::npos; ++i)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		if (i % 2 == 0)
		{
			if (token.length() > 8 || token.length() < 1)
				throw(std::runtime_error("400"));
			// what if extar chars are present
  			sscanf(token.c_str(), "%lx", &value);
  			std::cout << value << std::endl;
		}
		else
		{
			if (token.length() != value)
				throw(std::runtime_error("400"));
			result += token.substr(0, value);
			// result += token.substr(0, value) + "/r/n";
		}
		pos_start = pos_end + delim_len;
	}

	return result;
}

void	Parser::fillBodyRequest()
{
	if (Parser::ischunked)
	{
		int pos = packet.rfind("\r\n0\r\n\r\n");
		(void) pos;
		if (packet.rfind("\r\n0\r\n\r\n") == std::string::npos)
			read_again = 1;
		else
		{
			full_request.body_content_length += chunklen;
			full_request.body = parseChunks(packet.substr(Parser::body_start_pos), "\r\n");
			read_again = 0;
			fullbody = 1;
		}
		return ;
		// if (ischunkbody == false)
		// {
		// 	int len = chunkLength(buffer);
		// 	if (len <= 0)
		// 	{
		// 		ischunkbody = false;
		// 		fullbody = 1;
		// 		Parser::read_again = 0; // return err response
		// 	}
		// 	chunklen = chunkLength(buffer);
		// 	full_request.body_content_length += chunklen;
		// 	ischunkbody = true;
		// }
		// else
		// {
		// 	if (buffer.length() != chunklen + 2)
		// 		Parser::read_again = 0; // return err response
		// 	Parser::full_request.body += buffer.substr(0, chunklen);
		// 	ischunkbody = false;
		// }
		// return ;
	}
	Parser::full_request.body = Parser::packet.substr(Parser::body_start_pos);
	if (Parser::full_request.body.length() < Parser::full_request.body_content_length)
	{
		Parser::read_again = 1;
	}
	else
	{
		Parser::read_again = 0;
		fullbody = 1;
	}
}
