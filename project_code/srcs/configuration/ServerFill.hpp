#ifndef SERVERFILL_HPP
# define SERVERFILL_HPP

# include "../../includes/libs.hpp"
# include "Config.hpp"
class ServerFill
{
    public:
        ServerFill(tokenized_conf &conf_tokens);
        ~ServerFill();
        bool                parseEssentials();
        void                essentialsBasicCheck(
                                std::string &row_essentials, std::vector<std::string> &essentials_vec);
        void                fillEssentials(std::vector<std::string> &essentials);
        void                replaceNlAndTabs(std::string &str);
        void                fillPorts(std::vector<std::string> &listen_vec, 
                                std::set<std::string> &essentials_arg);
        int                 allInt(std::string str);
        void                fillServerNames(std::vector<std::string> &hosts_vec,  
                                std::set<std::string> &essentials_arg, stringmap &server);
        void                fill_essential_arg(std::set<std::string> &essential_arg);
        tokenized_conf          _conf_tokens;
        Config                  servers;
        std::vector<std::string> multiple_ports;
};


#endif