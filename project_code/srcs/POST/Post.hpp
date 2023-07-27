#ifndef POST_HPP
#define POST_HPP

#include "../../includes/libs.hpp"
#include <iostream>

class Post
{
private:
	packet_map							_request_map;
	t_request							_request;
	std::map<std::string, std::string>	_server_info;

public:
	Post(packet_map &request_map, t_request &full_request, std::map<std::string, std::string> &server_info);
	~Post();
	void	printPostHeader();
	void	printPostBody();
	void	printReceivedRequestMap();
	static void	visualizeStringMap(packet_map &map);
};

#endif 