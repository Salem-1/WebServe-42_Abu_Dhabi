#include "Respond.hpp"


bool	Respond::bodyTooBig(response_packet &response, stringmap &server_info, std::string &body)
{
	size_t max_body = 0;
	std::string query = response["dir"][0] + " Max-Body";
	if (server_info.find(query) == server_info.end())
		query = "Max-Body";
	std::istringstream max(server_info[query]);
	max >> max_body;
	if (max_body < body.length())
		return (true);
	return(false);		
}

void    Respond::visualizeResponse()
{
    std::cout << BOLDRED << "\nVisualizing reponse API\n" << std::endl;
    std::cout << "{" << std::endl << RESET;
    for (response_packet::iterator it = response.begin(); it != response.end(); it++)
    {
        if ((it->first).length() < 10000)
            std::cout << BOLDBLUE << "  \"" << it->first << "\": [" << RESET;
        else
            std::cout << "\"" << "large packet not gonna visualize" << "\", ";

         for (std::vector<std::string>::iterator vit = it->second.begin(); vit != it->second.end(); ++vit)
        {
            if ((*vit).length() < 10000)
                std::cout << "\"" << *vit << "\", ";
            else
                std::cout << "\"" << "large packet not gonna visualize" << "\", ";
        }
        std::cout << BOLDBLUE << "]" << std::endl << RESET;
    }
    std::cout << BOLDRED << "}" << std::endl << RESET;
}
