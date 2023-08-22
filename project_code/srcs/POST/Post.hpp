#ifndef POST_HPP
# define POST_HPP

# include "../../includes/libs.hpp"
# include "../PUT/PUT.hpp"
class Post: public PUT
{
private:
	void			sendToBackend();
	void			handleUpload();
	bool    	postBody(std::string path);

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
};

#endif 