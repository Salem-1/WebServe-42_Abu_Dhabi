#ifndef PUT_HPP
#define PUT_HPP

#include "../../includes/libs.hpp"
# include "../response/ErrResponse.hpp"

class PUT
{
protected:
	packet_map		_request_map;
	t_request		_request;
	stringmap		_server_info;
	response_packet _response_pack;
	ErrResponse		err;
public:
	PUT(packet_map &request_map, t_request &full_request, stringmap &server_info, response_packet &response);
	~PUT();
	void		printPUTHeader();
	void		printPUTBody();
	void		handlePUT();
	std::string	constructPath(stringmap &server_info);
	bool        sanitizedPath(std::string path);
	bool    	putBody(std::string path);
	std::string		fillOkResponse();
	std::string	_response;
};

#endif 