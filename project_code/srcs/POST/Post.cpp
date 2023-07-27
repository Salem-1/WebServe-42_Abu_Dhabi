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

void	Post::printPostHeader()
{
	std::cout << YELLOW << "\nPOST request header is: " << std::endl;
	std::cout << this->_request.header << std::endl;
	std::cout << RESET;
}

void	Post::printPostBody()
{
	std::cout << YELLOW << "\nPOST request body is: " << std::endl;
	std::cout << this->_request.body << std::endl;
	std::cout << RESET;
}

void Post::printReceivedRequestMap()
{
	std::cout << YELLOW << "\nPOST request header map is: " << std::endl;
	visualizeStringMap(this->_request_map);
	std::cout << RESET;
}

void Post::visualizeStringMap(packet_map &map)
{
    for (packet_map::iterator it = map.begin(); it != map.end(); it++)
    {
        std::cout << it->first << ": ";
        for (size_t i = 0; i < it->second.size(); i++)
        {
            std::cout << it->second[i];
            if (i != it->second.size() - 1)
            {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}
