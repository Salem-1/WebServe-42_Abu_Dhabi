
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
    
    if (fullheader == false)
	{
		if (((packet.find("\r\n\r\n") != std::string::npos || packet.find("\n\n") != std::string::npos ) && packet.length() > 10)
			|| earlyBadRequest(packet))
		{
			std::cout << "\nraw packet is\n-----------\n" << packet << "\n --------" << std::endl;
			body_start_pos = packet.find("\r\n\r\n") + 4;
			if (body_start_pos == std::string::npos)
				body_start_pos = packet.find("\n\n") + 2;
			if (body_start_pos)
			{
				full_request.header = packet.substr(0, body_start_pos);
				fillHeaderRequest(full_request.header);
				full_request.request_is_valid = checkHeaders();
				fullheader = true;
			}
		}
	}
	if (Parser::fullheader == true && Parser::fullbody == false)
	{
		if (Parser::request.find("content-length:") != request.end() && full_request.request_is_valid)
		{
			std::cout << YELLOW <<"body content length is " << request["content-length:"][0] << std::endl << RESET;
			std::istringstream iss(request["content-length:"][0]);
			iss >> full_request.body_content_length;
		}
		else if (Parser::request.find("Transfer-Encoding:") != request.end())
			Parser::ischunked = Parser::request.find("Transfer-Encoding:")->first == "chunked";
		if ((!full_request.body_content_length && !Parser::ischunked ) 
							|| full_request.body_content_length > MAX_BODY_SIZE)
		{
			if (full_request.body_content_length > MAX_BODY_SIZE)
				std::cout << "body is too large\n";
			packet = "";
			read_again = 0;
		}
		//   else
		// {
		// 	std::cout << "in-complete packet let's read again\n";
		// 	read_again = 1;
		// }
		Parser::fillBodyRequest(new_buffer); // what happens if length is not full
		read_again = 0;
    }
    if (full_request.header.length() > HEADER_MAX_LENGTH)
    {
        std::cout << "Reject packet at parser" << std::endl;
        //rejecting packet and close socket
        read_again = 0;
        bytes_read = 0;
        return ;
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

int	Parser::chunkLength(std::string buffer)
{
	std::istringstream	strlen(buffer);
	int 				len = 0;
	strlen >> len;		 
	for (size_t i = 0; std::isdigit(buffer[i]);  ++i) { }
	if (buffer.substr(i) == "\r\n" && len <= MAX_BODY_SIZE)
		return len;
	return -1;
}

void	Parser::fillBodyRequest(std::string buffer)
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
		if (Parser::ischunked)
		{
			if (ischunkbody == false)
			{

				int len = chunkLength(buffer);
				if (len <= 0)
					Parser::read_again = 0; // return err response
				chunklen = chunkLength(buffer);
				full_request.body_content_length += chunklen;
				ischunkbody = true;
			}
			else
			{
				// if (buffer.len != chunklen + 2)
				// 	Parser.read_again = 0; // return err response
				Parser::full_request.body += buffer.substr(0, chunklen);
				ischunkbody = false;
			}
		}
		return ;
	}
	else
	{
		Parser::full_request.body = Parser::packet.substr(Parser::body_start_pos);
		Parser::packet = "";
		Parser::read_again = 0;
	}
}
