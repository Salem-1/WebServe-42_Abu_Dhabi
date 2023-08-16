#ifndef CONFIG_HPP
# define CONFIG_HPP
# include "../../includes/libs.hpp"

class Config
{
    public:
        Config();
        ~Config();
        void	fillPorts();
        conf					servers;
        std::set<std::string>	ports;
        std::vector<int>		sockets;
        std::string     getPwd() const;
        void            visualize_config();
    private:
         std::string            pwd;
};
#endif