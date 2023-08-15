#ifndef POST_HPP
# define POST_HPP

# include "../../includes/libs.hpp"
# include "../PUT/PUT.hpp"
class Post: public PUT
{
private:
	packet_map		_request_map;
	t_request		_request;
	stringmap		_server_info;
	std::string		_response;
	response_packet _response_pack;
	void			sendToBackend();
	void			handleUpload();

public:
	Post(packet_map &request_map, t_request &full_request, stringmap &server_info
	,response_packet & response);
	~Post();
	void		printPostHeader();
	void		printPostBody();
	void		printReceivedRequestMap();
	void		handlePost();
	std::string	get_response() const;
	static void	visualizeStringMap(packet_map &map);
	bool    	postBody(std::string path);
};

#endif 