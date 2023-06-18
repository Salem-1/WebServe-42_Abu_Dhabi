#include "Parser.hpp"


Parser::Parser(): read_again(0), bytes_read(0), read_sock(0)
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

void    Parser::parse(char *new_buffer)
{
    if (!bytes_read)  
    {
        read_again = 0;
        return ;
    }
    std::string str(new_buffer);
    packet += str;
    if ((packet.find("\r\n\r\n") != std::string::npos || packet.find("\n\n") != std::string::npos ) && packet.length() > 10)
    {
        std::cout << "row packet is\n-----------\n" << packet << "\n --------" << std::endl;
        fill_header_request(packet);
        classify_packet();
        packet = "";
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
        read_again = 0;
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
            if (header == "HOST")
            {
                tmp_vec.push_back("400");
                request["Status-code"] = tmp_vec;
                return ;
            }
        }
    }
}

void    Parser::classify_packet()
{
    if (request["Status-code"][0] != "200")
    {
        std::cout << "bad header in parsing" << std::endl;
        return ;
    }
    request.erase("Status-code");
    if (packet.substr(0, packet.find(" ")) == "GET")
        check_headers();
    else if (packet.substr(0, packet.find(" ")) == "POST")
        std::cout << "POST method under construction" << std::endl;
    else if (packet.substr(0, packet.find(" ")) == "DEETE")
        std::cout << "DELETE method under construction" << std::endl;
    else
    {
        //501 method not implemented
        std::cout << "filled error response packet" << std::endl;
    }
    reponse_packet = filled_response;
}


