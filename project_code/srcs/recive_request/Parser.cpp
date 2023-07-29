
#include "Parser.hpp"


Parser::Parser(): read_again(0), bytes_read(0), read_sock(0), packet_counter(0), is_post(false)
{
	Parser::full_request.body_content_length = 0;
	Parser::full_request.request_is_valid = 0;
	Parser::full_request.header = "";
	Parser::full_request.body = "";
	Parser::body_start_pos = 0;
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
    flushParsing();
    if (!bytes_read)  
    {
        read_again = 0;
        return ;
    }
    std::string str(new_buffer);
    packet += str;
    
    vis_str(new_buffer, "new_buffer inside parser");
    vis_str(packet, "packet inside parser");
    
    //this will parse the headers
    if (((packet.find("\r\n\r\n") != std::string::npos || packet.find("\n\n") != std::string::npos ) && packet.length() > 10)
        || earlyBadRequest(packet))
    {
        std::cout << "\nrow packet is\n-----------\n" << packet << "\n --------" << std::endl;
		body_start_pos = packet.find("\r\n\r\n") + 4;
		if (body_start_pos == std::string::npos)
			body_start_pos = packet.find("\n\n") + 2;
		if (body_start_pos)
			full_request.header = packet.substr(0, body_start_pos);
        fillHeaderRequest(full_request.header);
		full_request.request_is_valid = checkHeaders();
		if (Parser::request.find("content-length:") != request.end() && full_request.request_is_valid)
		{
			std::cout << YELLOW <<"body content length is " << request["content-length:"][0] << std::endl << RESET;
			std::istringstream iss(request["content-length:"][0]);
			iss >> full_request.body_content_length;
		}
		if (!full_request.body_content_length || full_request.body_content_length > MAX_BODY_SIZE)
		{
			if (full_request.body_content_length > MAX_BODY_SIZE)
				std::cout << "body is too large\n";
			packet = "";
			read_again = 0;
		}
		else
			Parser::fillBodyRequest();
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
        std::cout << "in-complete packet let's read again\n";
        read_again = 1;
    }
}
bool Parser::earlyBadRequest(std::string packet)
{
    if (packet.length() >= 1)
    {
        if (packet[0] != 'G' || packet[0] != 'P' 
            || packet[0] != 'D')
            return (true);
    }
    return (false);
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

void	Parser::fillBodyRequest()
{
	// body chuncked request is not handled yet
	if (Parser::packet.length() > MAX_BODY_SIZE + HEADER_MAX_LENGTH)
	{
		std::cout << "body is too large\n";
		Parser::full_request.body = "";
		Parser::full_request.body_content_length = 0;
		Parser::packet = "";
		Parser::read_again = 0;
		return ;
	}
	else if (Parser::packet.length() < Parser::full_request.body_content_length + Parser::body_start_pos)
	{
		Parser::read_again = 1;
		return ;
	}
	else
	{
		Parser::full_request.body = Parser::packet.substr(Parser::body_start_pos, Parser::full_request.body_content_length);
		Parser::packet = "";
		Parser::read_again = 0;
	}
}
