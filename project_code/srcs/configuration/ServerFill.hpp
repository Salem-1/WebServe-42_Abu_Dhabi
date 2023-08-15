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
        
        tokenized_conf          _conf_tokens;
        Config                  servers;
        std::vector<std::string> multiple_ports;
};


#endif