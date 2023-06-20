# ifndef GET_RESPONSE_HPP
# define GET_RESPONSE_HPP
# include "../../includes/libs.hpp"


class GET_response
{
    public:
            typedef std::map<std::string, std::vector<std::string> >    response_type;

    public:
        GET_response(response_type res);
        ~GET_response();
        std::string     fill_get_response(std::map<std::string, std::string> &server_info);
        std::string     errored_response();
        std::string     construct_path(std::map<std::string, std::string> &server_info);
        void            fill_ok_response(std::map<std::string, std::string> &server_info);
        std::string     get_timebuffer();
        void            status_codes();
        int             fill_status_code(std::string status_code, std::string message);
        std::string     response_packet;
        response_type   reponse_check;
        std::unordered_map<std::string, std::string> StatusCodes;
};
# endif