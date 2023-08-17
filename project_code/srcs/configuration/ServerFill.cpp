#include "ServerFill.hpp"

ServerFill::ServerFill(tokenized_conf &conf_tokens): _conf_tokens(conf_tokens), i(0), port_counter(0)
{
    // parseEssentials();
}


ServerFill::~ServerFill()
{

}


bool    ServerFill::parseEssentials()
{
    i = 0;
    std::vector<std::string> essentials;
    for (tokenized_conf::iterator it = _conf_tokens.begin();
        it != _conf_tokens.end(); ++it)
    {

        essentials.clear();
        replaceNlAndTabs(it->first);
        essentialsBasicCheck(it->first, essentials);
        fillEssentials(essentials);
        fillMultiplePorts();
        //fill_locations();
        flushSingleServer();
    }
    return (true);
}
void    ServerFill::flushSingleServer()
{
    multiple_ports.clear();
    multiple_index.clear();
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
    
    servers.servers.push_back(stringmap());
    fill_essential_arg(essentials_arg);
    assignEssentials(essentials_arg, essentials);    
    if (inSet(essentials_arg, "root") || inSet(essentials_arg, "listen") || inSet(essentials_arg, "server_name"))
            throw(std::runtime_error("Bad config file: bad essential argument 💩"));
    if (inSet(essentials_arg, "client_max_body_size"))
        servers.servers[0]["Max-Body"] = "1000";
}

void   ServerFill::assignEssentials(std::set<std::string> &essentials_arg, std::vector<std::string> &essentials)
{
    std::vector<std::string> single_essential;
    for (std::vector<std::string>::iterator it = essentials.begin(); it != essentials.end(); ++it)
    {
        single_essential = split(*it, " ");
        if (single_essential.size() < 2)
            throw(std::runtime_error("Bad config file: wrong number of essential argument 💩"));
        if(single_essential[0] == "listen")
            fillPorts(single_essential, essentials_arg);
        else if(single_essential[0] == "server_name")
            fillServerNames(single_essential, essentials_arg, servers.servers[i]);
        else if(single_essential[0] == "root")
            fillRoot(single_essential, essentials_arg, servers.servers[i]);
        else if(single_essential[0] == "index")
            fillIndex(single_essential, essentials_arg, servers.servers[i]);
        else if(single_essential[0] == "client_max_body_size")
            fillBodySize(single_essential, essentials_arg, servers.servers[i]);
        else
            throw(std::runtime_error("Bad config file: bad essential argument 💩"));
    }
}
void    ServerFill::fillMultiplePorts(void)
{
    int port_num = 0;
    servers.servers[i]["port"] = multiple_ports[port_num];
    std::cout << "server port = " << servers.servers[i]["port"] << std::endl; 
    port_counter += multiple_ports.size(); 
    while (++i < port_counter)
    {

        servers.servers.push_back(stringmap());
        servers.servers[i] = servers.servers[i - 1];
        servers.servers[i]["port"] = multiple_ports[port_num];
        port_num++;
    }
}


void    ServerFill::fillBodySize(std::vector<std::string> &bodySize_vec,  std::set<std::string> &essentials_arg, 
        stringmap &server)
{
    (void)bodySize_vec;
    if(essentials_arg.find("client_max_body_size") == essentials_arg.end() 
        || bodySize_vec.size() != 2 || !isAllDigit(bodySize_vec[1]))
        throw(std::runtime_error("Bad config file: repeated client_max_body_size param  💩"));
    server["Max-Body"] = bodySize_vec[1];
    essentials_arg.erase("client_max_body_size");
}
void    ServerFill::fillIndex(std::vector<std::string> &index_vec,  std::set<std::string> &essentials_arg, 
        stringmap &server)
{
    if(essentials_arg.find("index") == essentials_arg.end() || index_vec.size() < 2)
        throw(std::runtime_error("Bad config file: bad index param  💩"));
        multiple_index.clear();
        server["index"] =  "";
    for (std::vector<std::string>::iterator it = index_vec.begin();
        it != index_vec.end(); ++it)
    {
        multiple_index.push_back(*it);
        // server["index"] += *it + " ";
    }
    // server["index"].erase(server["index"].find_last_of(' '), 1);
    server["index"] = index_vec[1];
    essentials_arg.erase("index");
}
void    ServerFill::fillRoot(std::vector<std::string> &root_vec,  std::set<std::string> &essentials_arg, 
        stringmap &server)
{
    if(essentials_arg.find("root") == essentials_arg.end() || root_vec.size() != 2)
        throw(std::runtime_error("Bad config file: bad root param  💩"));
    if (root_vec[1][0] != '/')
        server["root"] = servers.getPwd() + "/" + root_vec[1];
    else
        server["root"] = servers.getPwd()  + root_vec[1];
    essentials_arg.erase("root");
}

void    ServerFill::fillServerNames(std::vector<std::string> &hosts_vec,  std::set<std::string> &essentials_arg, 
        stringmap &server)
{
    (void)hosts_vec;
    if(essentials_arg.find("server_name") == essentials_arg.end())
        throw(std::runtime_error("Bad config file: repeated server_name param t 💩"));
    std::string host_names = "";
    for (std::vector<std::string>::iterator it = ++hosts_vec.begin();
            it != hosts_vec.end(); ++it)
    {
        host_names += *it + " ";
    }
    host_names.erase(host_names.find_last_of(' '), 1);
    server["server_name"] = host_names;
    essentials_arg.erase("server_name");
}

void    ServerFill::fillPorts(std::vector<std::string> &listen_vec, std::set<std::string> &essentials_arg)
{
    int port_check = 0;
    if (listen_vec.size() != 2)
        throw(std::runtime_error("listen has more thane one port"));
    if(essentials_arg.find("listen") == essentials_arg.end())
        essentials_arg.insert("listen");
    if (listen_vec[1].length() > 5)
            throw(std::runtime_error("port num with a more than 5 chars"));
    std::istringstream check_me(listen_vec[1]);
    check_me >> port_check;
    if (!isAllDigit(listen_vec[1]) || (port_check == 0 && listen_vec[1] != "0") || port_check < 0 || port_check > 65535)
        throw(std::runtime_error("Non numeric or overflow port number"));
    multiple_ports.push_back(listen_vec[1]);
    
    essentials_arg.erase("listen");
}

void    ServerFill::essentialsBasicCheck(std::string &row_essentials, std::vector<std::string> &essentials_vec)
{
        if(row_essentials.size() < 20)
            throw(std::runtime_error("Bad config file: Empty essentials_vec 💩"));
        essentials_vec = split(row_essentials, ";");
        
}

int ServerFill::isAllDigit(std::string str)
{
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (!std::isdigit(str[i]))
            return (false);
    }
    return (true);
}

void    ServerFill::fill_essential_arg(std::set<std::string>  &essentials_arg)
{
    essentials_arg.insert("listen");
    essentials_arg.insert("server_name");
    essentials_arg.insert("index");
    essentials_arg.insert("root");
    essentials_arg.insert("client_max_body_size");
};