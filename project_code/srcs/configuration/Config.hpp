

#ifndef CONFIG_HPP
# define CONFIG_HPP
# include "../../includes/libs.hpp"

class Config
{
    public:
        typedef std::vector<std::map<std::string, std::string> > conf; 
    public:
        Config();
        ~Config();
        void    fill_ports();
        conf                        servers;
        std::set<std::string>       ports;
        std::vector<int>            sockets;
};
#endif