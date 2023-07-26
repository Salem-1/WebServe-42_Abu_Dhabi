#include "Post.hpp"

Post::Post(packet_map &request, t_body &body, std::map<std::string, std::string> &server_info)
{
	(void)request;
	(void)server_info;
	std::cout << YELLOW << "POST request body is: " << body.body << std::endl;
	std::cout << "POST request under construction by yassein" << std::endl << RESET;
}

Post::~Post()
{

}
