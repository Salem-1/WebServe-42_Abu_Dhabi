#include "Post.hpp"

Post::Post(packet_map &request_map, t_request &full_request, std::map<std::string, std::string> &server_info)
{
	this->_request_map = request_map;
	this->_request = full_request;
	this->_server_info = server_info;
}

Post::~Post()
{

}

void	Post::print_post_header()
{
	std::cout << YELLOW << "POST request header is: " << std::endl;
	std::cout << this->_request.header << std::endl;
	std::cout << RESET;
}

void	Post::print_post_body()
{
	std::cout << YELLOW << "POST request body is: " << std::endl;
	std::cout << this->_request.body << std::endl;
	std::cout << RESET;
}

void Post::print_received_request_map()
{
	std::cout << YELLOW << "POST request header is: " << std::endl;
	visualize_string_map(this->_request_map);
	std::cout << RESET;
}

void Post::visualize_string_map(packet_map &map)
{
    for (packet_map::iterator it = map.begin(); it != map.end(); it++)
    {
        std::cout << it->first << ": ";
        for (size_t i = 0; i < it->second.size(); i++)
        {
            std::cout << it->second[i];
            if (i != it->second.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}
