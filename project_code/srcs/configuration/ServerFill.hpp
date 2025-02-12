#ifndef SERVERFILL_HPP
# define SERVERFILL_HPP

# include "../../includes/libs.hpp"
# include "Config.hpp"
class ServerFill
{
    public:
        ServerFill(tokenized_conf &conf_tokens);
        ServerFill(ServerFill const &obj2);
        ServerFill &operator= (ServerFill const &obj2);
        ~ServerFill();
        bool                    parseTokens();
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
        void                        fillEssentialArg(std::set<std::string> &essential_arg);
        void                        fillNoRepeateArg(std::set<std::string> &essential_arg);
        void                        fillRoot(std::vector<std::string> &root_vec,  std::set<std::string> &essentials_arg, 
                                        stringmap &server);
        void                        fillIndex(std::vector<std::string> &index_vec,  std::set<std::string> &essentials_arg, 
                                        stringmap &server);
        void                        fillBodySize(std::vector<std::string> &bodySize_vec,  std::set<std::string> &essentials_arg, 
                                        stringmap &server);
        void                        fillMultiplePorts(void);
        void                        assignEssentials(
                                        std::set<std::string> &essential_arg, std::vector<std::string> &essentials);
        void    fillDELETE_path(std::vector<std::string> &bodySize_vec,  std::set<std::string> &essentials_arg, 
                    stringmap &server);
        void                        flushSingleServer(void);
        void                        checkRepeatedServers(void);
        bool                        findRepeatedPort(conf::iterator it, conf::iterator &repeated_it);
        void                        checkDuplicateServerNames(conf::iterator &it, conf::iterator &repeated_it);
        void                        flushEssentialsVars(std::vector<std::string> essentials);
        void                        parseEssentials(std::string essential_str, std::vector<std::string> essentials_vec);
        void                        parseLocations(std::vector<std::string> locations);
        void                        locationBasicCheck(std::string location);
        void                        fillLocations(std::string location);
        void                        fillLocationPath(std::vector<std::string> &location_options,
                                         std::string &path);
        void                        fillRestLocationDirectives(locations_args &args);
        void                        fillRootLocation(locations_args &args);
        void                        fillIndexLocation(locations_args &args);
        void                        fillAutoIndexLocation(locations_args &args);
        void                        fillErrorPageLocation(locations_args &args);
        void                        fillCgiBinLocation(locations_args &args);
        void                        fillRedirectioneLocation(locations_args &args);
        void                        fillServerPorts();
        void                        fillArgs(locations_args &args, std::string &path, 
                                        std::vector<std::string> &location_options, std::set<std::string>  &no_repeate_arg);
        void                        fillmethodsLocation(locations_args &args);
        void                        fillCGIRootLocation(locations_args &args);
        void                        fillCGIErrorLocation(locations_args &args);
        void                        fillBodySizeLocation(locations_args &args);
        void                        fillCGIExecutable(locations_args & args);
        void                        ExecludeBadDirectives(locations_args &args);
        void                        test_multiple_servers();
        void                        fillRootIndex();
        std::vector<std::string>    multiple_ports;
        std::vector<std::string>    multiple_index;
    private:
        size_t                   i;
        size_t                   port_counter;
    
};


#endif