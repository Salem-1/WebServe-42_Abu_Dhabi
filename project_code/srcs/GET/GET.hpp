# ifndef GET_HPP
# define GET_HPP
# include "../../includes/libs.hpp"


class GET
{
    public:
        typedef std::map<std::string, std::vector<std::string> > packet_map;
        typedef std::map<std::string, std::string>               response_packet;
    public:
        GET();
        ~GET();
        GET &operator= (const GET &obj2);
        GET(const GET &obj2);
        void                        handle(packet_map packet);
        void                        fill_get_request(std::string packet);
        void                        visualize_request_packet();
        void                        fill_response();  
        void                        fill_valid_headers();
        int                         packet_counter;
        int                         i;
        packet_map                  request;
        response_packet             response;
        std::vector<std::string>    valid_headers;
};  
#endif