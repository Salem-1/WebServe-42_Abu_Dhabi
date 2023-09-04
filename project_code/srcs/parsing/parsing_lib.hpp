
#ifndef PARSING_LIB
# define PARSING_LIB
# include "../../includes/libs.hpp"

std::vector<std::string>	split(std::string str, std::string delimeter);
std::set<std::string>		split_to_set(std::string str, std::string delimeter);

int							fillStatuCode(response_packet &response, std::string status_code, std::string message);
void						fillPath(packet_map &request,response_packet &response, std::string method);
void                        print_error(std::string msg);
void                        runAtAllCost();
#endif