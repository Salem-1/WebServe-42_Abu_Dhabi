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
        std::string     fill_get_response();
        std::string     errored_response();
        void            fill_ok_response();
        response_type   reponse_check;
        std::string     response_packet;
    
};
# endif