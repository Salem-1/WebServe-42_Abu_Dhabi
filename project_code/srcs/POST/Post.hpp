#ifndef POST_HPP
#define POST_HPP

#include "../../includes/libs.hpp"

class Post
{
private:
	packet_map	_request_map;
	t_request	_request;
	stringmap	_server_info;
	std::string	_response;

public:
	Post(packet_map &request_map, t_request &full_request, stringmap &server_info);
	~Post();
	void		printPostHeader();
	void		printPostBody();
	void		printReceivedRequestMap();
	void		sendToBackend();
	std::string	get_response() const;
	static void	visualizeStringMap(packet_map &map);
};

#endif 