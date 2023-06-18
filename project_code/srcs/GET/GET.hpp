# ifndef GET_HPP
# define GET_HPP
# include "../../includes/libs.hpp"
# include "GET_response.hpp"

class GET
{
    public:
        typedef std::map<std::string, std::vector<std::string> >    packet_map;
        typedef std::map<std::string, std::vector<std::string> >    response_packet;
    public:
        GET(packet_map &request, response_packet &response);
        ~GET();
        void    fill_request_line(packet_map &request);
        void    fill_path(packet_map &request);
        int     fill_status_code(std::string status_code, std::string message);
        void    prepare_get_response(std::map<std::string, std::string> &server_info);
        // GET &operator= (const GET &obj2);
        // GET(const GET &obj2);
        packet_map      &request;
        response_packet &response;
};  
#endif