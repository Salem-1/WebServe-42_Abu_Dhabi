/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:41:21 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/04 13:46:53 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"


Parser::Parser(): read_again(0), bytes_read(0), read_sock(0), packet_counter(0)
{
    fill_valid_headers();
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

void    Parser::flush_parsing()
{
        read_again = 0;
        packet = "";
        reponse_packet = ""; 
        packet_counter++;
        request.clear();
        response.clear();
        filled_response = "";
}

void    Parser::parse(char *new_buffer)
{
    flush_parsing();
    if (!bytes_read)  
    {
        read_again = 0;
        return ;
    }
    std::string str(new_buffer);
    packet += str;
    std::cout << "\n\ninside parser buffer = \n<" << new_buffer << ">" << std::endl;
    std::cout << "\n\ninside parser packet = \n<" << packet<< ">" << std::endl;
    if ((packet.find("\r\n\r\n") != std::string::npos || packet.find("\n\n") != std::string::npos ) && packet.length() > 10)
    {
        std::cout << "\nrow packet is\n-----------\n" << packet << "\n --------" << std::endl;
        fill_header_request(packet);
        check_headers();
        packet = "";
        read_again = 0;
    }
    else if (packet.length() > HEADER_MAX_LENGTH)
    {
        std::cout << "Reject packet at parser" << std::endl;
        //rejecting packet and close socket
        read_again = 0;
        bytes_read = 0;
        return ;
    }
    else
    {
        std::cout << "\nin-complete packet let's read again\n";
        read_again = 1;
    }
}

void    Parser::set_byteread_and_readsock(int bytes, int sock)
{
    this->bytes_read = bytes;
    this->read_sock = sock;
}

void    Parser::fill_header_request(std::string packet)
{
    std::vector<std::string> tmp_vec;
    std::string              header;
    std::vector<std::string> packet_lines = split(packet, "\r\n");

    tmp_vec.push_back("200");
    request["Status-code"] = tmp_vec;

    for (std::vector<std::string>::iterator it = packet_lines.begin(); it != packet_lines.end(); it++)
    {
        tmp_vec = split(*it, " ");
        if (tmp_vec.size() < 1)
        {
            tmp_vec.push_back("400");
            tmp_vec.push_back("Bad request");
            request["Status-code"] = tmp_vec;
            return ;
        }
        header = tmp_vec[0];
        tmp_vec.erase(tmp_vec.begin());
        //check for repetetion  in headers
        if (request.find(header) == request.end())
            request[header] = tmp_vec;
        else
        {
            if (header == "Host:")
            {
                tmp_vec.push_back("400");
                tmp_vec.push_back("Bad request");
                request["Status-code"] = tmp_vec;
                return ;
            }
        }
    }
}




