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
        GET();
        ~GET();
        GET &operator= (const GET &obj2);
        GET(const GET &obj2);
        void                     handle(packet_map packet);
        void                     fill_get_request(std::string packet);
        void                     visualize_request_packet();
        int                      packet_counter;
        int                      i;
        packet_map               request;
        response_packet          response;
        std::set<std::string>    valid_headers;
        std::string              filled_response;
    private:
        void                     fill_response();  
        void                     fill_valid_headers();
        int                      valid_packet_headers();
        void                     fill_request_line();
        void                     fill_path();
        void                     visualize_response();
        int                      fill_status_code(std::string status_code, std::string message);


};  
#endif