

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

void	Parser::purgeParsing()
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
	is_post = false;
	flushParsing();
    fillValidHeaders();

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
	// std::string testing(new_buffer, bytes_read);
    packet.append(new_buffer, bytes_read);
    
    // vis_str(new_buffer, "new_buffer inside parser");
    // vis_str(packet, "packet inside parser");
	    
    if (fullheader == false)
	{
		if (((packet.find("\r\n\r\n") != std::string::npos || packet.find("\n\n") != std::string::npos ))
			|| earlyBadRequest(packet))
		{
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
			if (full_request.header.find("HTTP/1.1") == std::string::npos)
				throw(std::runtime_error("505"));
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
		if (Parser::ischunked == false)
		{
			packet_map::iterator it = Parser::request.find("content-length:");
			if (it != request.end() && full_request.request_is_valid)
			{
				std::istringstream iss(it->second[0]);
				iss >> full_request.body_content_length;

				if ((full_request.body_content_length == 0 && it->second[0] != "0") 
					||full_request.body_content_length > MAX_BODY_SIZE)
					throw(std::runtime_error("400"));
			}
			else if (Parser::request.find("Transfer-Encoding:") != request.end())
				Parser::ischunked = Parser::request.find("Transfer-Encoding:")->second[0] == "chunked";
			else
			{
				read_again = 0;
				fullbody = 1;
				return ;
			}
		}
		Parser::fillBodyRequest();
    }
    if (full_request.header.length() > HEADER_MAX_LENGTH || full_request.request_is_valid == 0)
    {
        std::cout << "Reject packet at parser" << std::endl;
		throw(std::runtime_error("400")); //rejecting packet and close socket
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

void    Parser::fillHeaderRequest(std::string &packet)
{
    std::vector<std::string> tmp_vec;
    std::string              header;
    std::vector<std::string> packet_lines = split(packet, "\r\n");

    for (std::vector<std::string>::iterator it = packet_lines.begin(); it != packet_lines.end(); it++)
    {
        tmp_vec = split(*it, " ");
        if (tmp_vec.size() < 2)
			throw(std::runtime_error("400"));
        header = tmp_vec[0];
        tmp_vec.erase(tmp_vec.begin());
        if (request.find(header) == request.end())
            request[header] = tmp_vec;
        else
			throw(std::runtime_error("400"));
    }
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
		}
		pos_start = pos_end + delim_len;
	}

	return result;
}

void	Parser::fillBodyRequest()
{
	if (Parser::ischunked)
	{
		if (packet.rfind("\r\n0\r\n\r\n") == std::string::npos)
			read_again = 1;
		else
		{
			full_request.body = parseChunks(packet.substr(Parser::body_start_pos), "\r\n");
			full_request.body_content_length = full_request.body.length();
			// std::cout << BOLDYELLOW << full_request.body << std::endl << RESET;
			read_again = 0;
			fullbody = 1;
		}
		return ;
	}
	Parser::read_again = 1;
	if (packet.length() - Parser::body_start_pos >= Parser::full_request.body_content_length)
	{
		Parser::full_request.body = Parser::packet.substr(Parser::body_start_pos, Parser::full_request.body_content_length);
		fullbody = 1;
		Parser::read_again = 0;
	}
}
