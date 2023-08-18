#ifndef SERVERFILL_HPP
# define SERVERFILL_HPP

# include "../../includes/libs.hpp"
# include "Config.hpp"
class ServerFill
{
    public:
        ServerFill(tokenized_conf &conf_tokens);
        ~ServerFill();
        bool                    parseEssentials();
        // std::vector<std::string> getMultiplePorts() const;
        // std::vector<std::string> getMultipleindex() const;
        // Config                      getServers();
        tokenized_conf              _conf_tokens;
        Config                      servers;
        void                        essentialsBasicCheck(
                                        std::string &row_essentials, std::vector<std::string> &essentials_vec);
        void                        fillEssentials(std::vector<std::string> &essentials);
        void                        replaceNlAndTabs(std::string &str);
        void                        fillPorts(std::vector<std::string> &listen_vec, 
                                        std::set<std::string> &essentials_arg);
        int                         isAllDigit(std::string str);
        void                        fillServerNames(std::vector<std::string> &hosts_vec,  
                                        std::set<std::string> &essentials_arg, stringmap &server);
        void                        fill_essential_arg(std::set<std::string> &essential_arg);
        void                        fillRoot(std::vector<std::string> &root_vec,  std::set<std::string> &essentials_arg, 
                                        stringmap &server);
        void                        fillIndex(std::vector<std::string> &index_vec,  std::set<std::string> &essentials_arg, 
                                        stringmap &server);
        void                        fillBodySize(std::vector<std::string> &bodySize_vec,  std::set<std::string> &essentials_arg, 
                                        stringmap &server);
        void                        fillMultiplePorts(void);
        void                        assignEssentials(
                                        std::set<std::string> &essential_arg, std::vector<std::string> &essentials);
        void                        flushSingleServer(void);
        void                        checkRepeatedServers(void);
        bool                        findRepeatedPort(conf::iterator it, conf::iterator &repeated_it);
        void                        checkDuplicateServerNames(conf::iterator &it, conf::iterator &repeated_it);
        void                        flushEssentialsVars(std::vector<std::string> essentials);
        std::vector<std::string>    multiple_ports;
        std::vector<std::string>    multiple_index;
    private:
        size_t                   i;
        size_t                   port_counter;
    
};


#endif