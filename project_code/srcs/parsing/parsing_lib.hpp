
#ifndef PARSING_LIB
# define PARSING_LIB
# include "../../includes/libs.hpp"

// typedef std::map<std::string, std::vector<std::string> >    packet_map;
// typedef std::map<std::string, std::vector<std::string> >    response_packet;
// typedef std::vector<stringmap >    conf; 
std::vector<std::string>	split(std::string str, std::string delimeter);
int							fillStatuCode(response_packet &response, std::string status_code, std::string message);
void						fillPath(packet_map &request,response_packet &response, std::string method);
#endif