#include "ServerFill.hpp"

ServerFill::ServerFill(tokenized_conf &conf_tokens): _conf_tokens(conf_tokens)
{
    
}


ServerFill::~ServerFill()
{

}


bool    ServerFill::parseEssentials()
{
    std::vector<std::string> essentials;
    for (tokenized_conf::iterator it = _conf_tokens.begin();
        it != _conf_tokens.end(); ++it)
    {
        essentials.clear();
        replaceNlAndTabs(it->first);
        essentialsBasicCheck(it->first, essentials);
        fillEssentials(essentials);
    }
    return (true);
}

void    ServerFill::replaceNlAndTabs(std::string &str)
{
    std::replace(str.begin(), str.end(), '\n' , ' ');
    std::replace(str.begin(), str.end(), '\v' , ' ');
    std::replace(str.begin(), str.end(), '\t' , ' ');

}

void        ServerFill::fillEssentials(std::vector<std::string> &essentials)
{
    std::set<std::string> essentials_arg;
    std::vector<std::string> single_essential;

    essentials_arg.insert("listen");
    essentials_arg.insert("server_name");
    essentials_arg.insert("index");
    essentials_arg.insert("root");
    essentials_arg.insert("client_max_body_size");
    // visualize_string_vector(essentials, "essentials");
    for (std::vector<std::string>::iterator it = essentials.begin(); it != essentials.end(); ++it)
    {
        single_essential = split(*it, " ");
        if (single_essential.size() < 2)
            throw(std::runtime_error("Bad config file: wrong number of essential argument 💩"));
        if(single_essential[0] == "listen")
            fillPorts(single_essential, essentials_arg);
        else if(single_essential[0] == "server_name")
        {
            if(essentials_arg.find("server_name") == essentials_arg.end())
                throw(std::runtime_error("Bad config file: repeated server_name param t 💩"));

            essentials_arg.erase("server_name");
        }
        else if(single_essential[0] == "index")
        {
            if(essentials_arg.find("index") == essentials_arg.end())
                throw(std::runtime_error("Bad config file: repeated index param  💩"));

            essentials_arg.erase("index");
        }
        else if(single_essential[0] == "client_max_body_size")
        {
            if(essentials_arg.find("client_max_body_size") == essentials_arg.end())
                throw(std::runtime_error("Bad config file: repeated client_max_body_size param  💩"));

            essentials_arg.erase("client_max_body_size");
        }
        else if(single_essential[0] == "root")
        {
            if(essentials_arg.find("root") == essentials_arg.end())
                throw(std::runtime_error("Bad config file: repeated root param  💩"));

            essentials_arg.erase("root");
        }
        else
        {
            throw(std::runtime_error("Bad config file: bad essential argument 💩"));
        }
    }
    if (inSet(essentials_arg, "root") || inSet(essentials_arg, "listen") || inSet(essentials_arg, "server_name"))
            throw(std::runtime_error("Bad config file: bad essential argument 💩"));
}
void    ServerFill::fillPorts(std::vector<std::string> &listen_vec, std::set<std::string> &essentials_arg)
{
    if(essentials_arg.find("listen") == essentials_arg.end())
        throw(std::runtime_error("Bad config file: repeated listen param 💩"));
    (void)listen_vec;
    essentials_arg.erase("listen");
}

void    ServerFill::essentialsBasicCheck(std::string &row_essentials, std::vector<std::string> &essentials_vec)
{
        if(row_essentials.size() < 20)
            throw(std::runtime_error("Bad config file: Empty essentials_vec 💩"));
        essentials_vec = split(row_essentials, ";");
        if (essentials_vec.size() < 3 || essentials_vec.size() > 6)
            throw(std::runtime_error("Bad config file: wrong essentials parameters 💩"));
        
}



// void    fillListen(std::set<std::string> essentials_arg, std::vector<std::string> single_essential)
// {

// }