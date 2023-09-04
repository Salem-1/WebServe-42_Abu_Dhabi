#include "Respond.hpp"


bool	Respond::bodyTooBig(response_packet &response, stringmap &server_info, std::string &body)
{
	std::string query;
	size_t max_body = 0;
	if (response.find("dir") != response.end())
		query = response["dir"][0] + " Max-Body";
	else if (response.find("dir") == response.end())
		query = "/ Max-Body";
	if (server_info.find(query) == server_info.end())
		query = "Max-Body";
	std::istringstream max(server_info[query]);
	max >> max_body;
	if (max_body < body.length())
		return (true);
	return(false);		
}

