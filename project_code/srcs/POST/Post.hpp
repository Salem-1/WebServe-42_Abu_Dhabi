#ifndef POST_HPP
#define POST_HPP

#include "../../includes/libs.hpp"
#include <iostream>

class Post
{
private:
	/* data */
public:
	Post(packet_map &request, std::map<std::string, std::string> &server_info);
	~Post();
};

// Post::Post(/* args */)
// {
// }

// Post::~Post()
// {
// }

#endif 