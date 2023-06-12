# ifndef GET_HPP
# define GET_HPP
# include "../../includes/libs.hpp"


class GET
{
    public:
        GET();
        ~GET();
        GET &operator= (const GET &obj2);
        GET(const GET &obj2);
        // void    handle(std::string buffer, int len);
        // int     check_method(std::string buffer, int len);
        int     packet_counter;
        int     i;
        std::map<std::string, std::string> request;
};  
#endif