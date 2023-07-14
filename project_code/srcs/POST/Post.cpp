#include "Post.hpp"

Post::Post(packet_map &request, std::map<std::string, std::string> &server_info)
{
	(void)request;
	(void)server_info;
	std::cout << "POST request under construction by yassein" << std::endl;
}

Post::~Post()
{

}
