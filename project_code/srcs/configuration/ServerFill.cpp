#include "ServerFill.hpp"

ServerFill::ServerFill(tokenized_conf &conf_tokens): _conf_tokens(conf_tokens), i(0)
{
    // parseTokens();
}


ServerFill::~ServerFill()
{

}


bool    ServerFill::parseTokens()
{
    std::vector<std::string> essentials_vec;
    i = 0;
    servers.servers.clear();
    for (tokenized_conf::iterator it = _conf_tokens.begin();
        it != _conf_tokens.end(); ++it)
    {
        parseEssentials(it->first, essentials_vec);
        parseLocations(it->second);
        fillServerPorts();
    }
    checkRepeatedServers();
    return (true);
}

void    ServerFill::fillServerPorts()
{
    if (multiple_ports.size() > 0)
    {
        servers.servers[i]["port"] = multiple_ports[0];
        i++;
        for (std::vector<std::string>::iterator it = ++multiple_ports.begin();
            it != multiple_ports.end(); ++it)
        {

            servers.servers.push_back(stringmap());
            servers.servers[i] = servers.servers[i - 1];
            servers.servers[i]["port"] = *it;
            i++;
        }
    }
}

void     ServerFill::parseEssentials(std::string essential_str, std::vector<std::string> essentials_vec)
{
        flushEssentialsVars(essentials_vec);
        replaceNlAndTabs(essential_str);
        essentialsBasicCheck(essential_str, essentials_vec);
        fillEssentials(essentials_vec);
}

void    ServerFill::parseLocations(std::vector<std::string> locations)
{
    for (std::vector<std::string>::reverse_iterator it = locations.rbegin();
        it != locations.rend(); it++)
    {
        replaceNlAndTabs(*it);
        locationBasicCheck(*it);
        fillLocations(*it);
    }
    fillRootIndex();
}

void    ServerFill::fillRootIndex()
{
    if (inMap(servers.servers[i], "/"))
        return ;
    else if (inMap(servers.servers[i], "/ index"))
        servers.servers[i]["/"] = servers.servers[i]["root"] + "/" + servers.servers[i]["/ index"];
    if (inMap(servers.servers[i], "index"))
        servers.servers[i]["/"] = servers.servers[i]["root"] + "/" + servers.servers[i]["index"];
}

void    ServerFill::fillLocations(std::string location)
{
    std::set<std::string>       no_repeate_arg;
    std::vector<std::string>    location_options = split(location, ";");
    std::string                 path;
    locations_args              args;

    fillNoRepeateArg(no_repeate_arg);
    if (location_options.size() < 2)
        throw(std::runtime_error("Bad config file: incomplete location block"));
    fillLocationPath(location_options, path);
    fillArgs(args, path, location_options, no_repeate_arg);
    if (args.path  == "/cgi-bin")
    {
        fillCgiBinLocation(args);
        return ;
    }
    else if (args.path[0] == '.')
    {
        fillCGIExecutable(args);
        return ;
    }

    for (std::vector<std::string>::iterator it = ++location_options.begin();
        it != location_options.end(); ++it)
    {
        args.tmp_directive = split(*it, " ");
        if (args.tmp_directive.empty())
            continue;   
        if (args.tmp_directive.size() < 2)
            throw(std::runtime_error("Bad config file: single bad  directive args"));
        fillRestLocationDirectives(args);
    }
}

void    ServerFill::fillCGIExecutable(locations_args & args)
{
        for (std::vector<std::string>::iterator it = ++args.location_options.begin();
        it != args.location_options.end(); ++it)
    {
        args.tmp_directive = split(*it, " ");
        if (args.tmp_directive.empty())
            continue;   
        if (args.tmp_directive.size() != 2)
            throw(std::runtime_error("Bad config file: single bad  directive args"));
        if (args.tmp_directive[0] != "index")
            throw(std::runtime_error("Bad config file: CGI executable directive"));
         servers.servers[i][args.path] = servers.servers[i]["root"] + args.tmp_directive[1];
        // std::cout << servers.servers[i][args.path] << std::endl;
    }
}
void    ServerFill::fillCgiBinLocation(locations_args & args)
{
    args.cgi_essential.insert("root");
    args.cgi_essential.insert("error_page");
    for (std::vector<std::string>::iterator it = ++args.location_options.begin();
        it != args.location_options.end(); ++it)
    {
        args.tmp_directive = split(*it, " ");
        if (args.tmp_directive.empty())
            continue;
        if (args.tmp_directive.size() < 2 || !inSet(args.cgi_essential, args.tmp_directive[0]))
            throw(std::runtime_error("Bad config file: single bad  directive args CGI"));
        fillCGIRootLocation(args);
        fillCGIErrorLocation(args);
    }
    if (inSet(args.cgi_essential, "root"))
            throw(std::runtime_error("Bad config file: CGI with no root"));

}

void    ServerFill::fillCGIErrorLocation(locations_args &args)
{
    if(args.tmp_directive[0] != "error_page")
        return ;
    if (args.tmp_directive.size() != 3)
        throw(std::runtime_error("Bad configuration file: wrong number of error_page  CGI arguments"));
    if (!isAllDigit(args.tmp_directive[1]) || args.tmp_directive[1].size() != 3)
        throw(std::runtime_error("Bad configuration file: provided bad error code  error_page arguments CGI"));
    servers.servers[i][args.path + " " + args.tmp_directive[1]] = args.tmp_directive[2];
}
void    ServerFill::fillCGIRootLocation(locations_args &args)
{

    if(args.tmp_directive[0] != "root")
        return ;
    if (!inSet(args.cgi_essential, "root"))
        throw(std::runtime_error("Bad configuration file: bad  root directive CGI"));
    if (args.tmp_directive.size() != 2)
        throw(std::runtime_error("Bad configuration file: wrong number of root directive arguments"));
    args.cgi_essential.erase("root");
    servers.servers[i][args.path] = servers.servers[i]["root"] + args.tmp_directive[1];
}

void    ServerFill::fillRestLocationDirectives(locations_args & args)
{

        fillRootLocation(args);
        fillIndexLocation(args);
        fillAutoIndexLocation(args);
        fillmethodsLocation(args);
        fillErrorPageLocation(args);
        fillRedirectioneLocation(args);
        fillBodySizeLocation(args);
        ExecludeBadDirectives(args);

}

void    ServerFill::ExecludeBadDirectives(locations_args &args)
{
    std::string msg = "Bad config file: Ambigous directive inside location " + args.tmp_directive[0]; 
    if (!inSet(args.no_repeate_arg ,args.tmp_directive[0]))
        throw(std::runtime_error(msg));
}


void    ServerFill::fillBodySizeLocation(locations_args &args)
{
    if(args.tmp_directive[0] != "client_max_body_size")
        return ;
    if (args.tmp_directive.size() != 2)
        throw(std::runtime_error("Bad configuration file: wrong number of redirection client max body size"));
    if (!isAllDigit(args.tmp_directive[1]) || args.tmp_directive[1].size() > 10)
        throw(std::runtime_error("Bad configuration file: provided bad body size "));
    servers.servers[i][args.path + " Max-Body"] = args.tmp_directive[1];
}

void    ServerFill::fillRedirectioneLocation(locations_args &args)
{
    if(args.tmp_directive[0] != "redirection")
        return ;
    if (args.tmp_directive.size() != 3)
        throw(std::runtime_error("Bad configuration file: wrong number of redirection arguments"));
    if (!isAllDigit(args.tmp_directive[1]) || args.tmp_directive[1].size() != 3)
        throw(std::runtime_error("Bad configuration file: provided bad redirection code  redirection arguments"));
    servers.servers[i][args.path + " " + args.tmp_directive[1]] = args.tmp_directive[2];
}

void    ServerFill::fillErrorPageLocation(locations_args &args)
{
    if(args.tmp_directive[0] != "error_page")
        return ;
    if (args.tmp_directive.size() != 3)
        throw(std::runtime_error("Bad configuration file: wrong number of error_page arguments"));
    if (!isAllDigit(args.tmp_directive[1]) || args.tmp_directive[1].size() != 3)
        throw(std::runtime_error("Bad configuration file: provided bad error code  error_page arguments"));
    servers.servers[i][args.path + " " + args.tmp_directive[1]] = args.tmp_directive[2];
}
void    ServerFill::fillmethodsLocation(locations_args &args)
{
    if(args.tmp_directive[0] != "methods")
        return ;
    if (args.tmp_directive.size() < 2)
        throw(std::runtime_error("Bad configuration file: wrong number of methods arguments"));
    std::set<std::string> allowed_methods;
    allowed_methods.insert("GET");
    allowed_methods.insert("POST");
    allowed_methods.insert("DELETE");
    allowed_methods.insert("PUT");
    servers.servers[i][args.path + " methods"].clear();
    for (std::vector<std::string>::iterator it = ++args.tmp_directive.begin();
            it != args.tmp_directive.end(); ++it)
    {
        if (!inSet(allowed_methods, *it))
            throw(std::runtime_error("Bad configuration file: bad of methods arguments"));
        servers.servers[i][args.path + " methods"] += *it;
        if (it + 1 != args.tmp_directive.end())
            servers.servers[i][args.path + " methods"] += " ";
        allowed_methods.erase(*it);
    }
}

void    ServerFill::fillAutoIndexLocation(locations_args &args)
{
    if(args.tmp_directive[0] != "autoindex")
        return ;
    if (args.tmp_directive.size() != 2)
        throw(std::runtime_error("Bad configuration file: wrong number of autindex directive arguments"));
    if (args.tmp_directive[1] == "on" || args.tmp_directive[1] == "off")
        servers.servers[i][args.path + " autoindex"] = args.tmp_directive[1];
    else
        throw(std::runtime_error("Bad configuration file: wrong  autindex directive argument"));
}

void    ServerFill::fillIndexLocation(locations_args &args)
{
    if(args.tmp_directive[0] != "index")
        return ;
    if (args.tmp_directive.size() != 2)
        throw(std::runtime_error("Bad configuration file: wrong number of index directive arguments"));
    servers.servers[i][args.path + " index"] = args.tmp_directive[1];
}

void    ServerFill::fillRootLocation(locations_args &args)
{

    if(args.tmp_directive[0] != "root")
        return ;
    if (args.tmp_directive.size() != 2)
        throw(std::runtime_error("Bad configuration file: wrong number of root directive arguments"));
    servers.servers[i][args.path] = servers.servers[i]["root"] + args.tmp_directive[1];
}

void    ServerFill::fillArgs(locations_args &args, std::string &path, 
            std::vector<std::string> &location_options, std::set<std::string>  &no_repeate_arg)
{
    args.location_options = location_options;
    args.path = path;
    args.no_repeate_arg = no_repeate_arg;
}
void ServerFill::fillLocationPath(std::vector<std::string> &location_options, std::string &path)
{
    std::vector<std::string> tmp_location_path = split(location_options[0], " ");
    if (tmp_location_path.size() != 2 || tmp_location_path[0] != "location")
        throw(std::runtime_error("Bad config file: bad location block"));
    path = tmp_location_path[1];
}

void    ServerFill::locationBasicCheck(std::string location)
{
    if (location.length() < 13)
        throw(std::runtime_error("Bad configuration file: bad location"));
}


void    ServerFill::flushEssentialsVars(std::vector<std::string> essentials)
{
        essentials.clear();
        multiple_index.clear();
        multiple_ports.clear();

}

void    ServerFill::checkRepeatedServers(void)
{
    conf::iterator repeated_it = servers.servers.end();

    for (conf::iterator it = servers.servers.begin();
        it != servers.servers.end(); ++it)
    {
        if (findRepeatedPort(it, repeated_it))
            checkDuplicateServerNames(it, repeated_it);
    }
}

bool    ServerFill::findRepeatedPort(conf::iterator it, conf::iterator &repeated_it)
{
    for (conf::iterator pit = it + 1
        ; pit != servers.servers.end(); ++pit)
    {
        if ((*it)["port"] == (*pit)["port"])
        {
            repeated_it = pit;
            return (true);
        }    
    }
    return (false);
}

void    ServerFill::checkDuplicateServerNames(conf::iterator &it, conf::iterator &repeated_it)
{
    std::vector<std::string> compared = split((*it)["server_name"], " ");
    std::vector<std::string> to = split((*repeated_it)["server_name"], " ");
    for (std::vector<std::string>::iterator compared_it = compared.begin(); 
        compared_it != compared.end(); compared_it++)
    {
        if (inVector(to, *compared_it))
            throw(std::runtime_error("Conf error: repeated hostname with the same port"));
    }
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
    std::vector<std::string> single_essential;
    servers.servers.push_back(stringmap());
    fillEssentialArg(essentials_arg);
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
        else if(single_essential[0] == "DELETE_path")
            fillDELETE_path(single_essential, essentials_arg, servers.servers[i]);
        else
            throw(std::runtime_error("Bad config file: bad essential argument 💩"));
    }
    if (inSet(essentials_arg, "root") || inSet(essentials_arg, "listen") || inSet(essentials_arg, "server_name"))
            throw(std::runtime_error("Bad config file: bad essential argument 💩"));
    if (inSet(essentials_arg, "client_max_body_size"))
        servers.servers[0]["Max-Body"] = MAX_BODY_SIZE;
    if (inSet(essentials_arg, "DELETE_path"))
        servers.servers[0]["DELETE"] = "POST";
    // std::cout << "inside the function we have servers = " << servers.servers.size();
}
void    ServerFill::fillDELETE_path(std::vector<std::string> &DELETE_path_vec,  std::set<std::string> &essentials_arg, 
        stringmap &server)
{
    if(!inSet(essentials_arg, "DELETE_path") || DELETE_path_vec.size() != 2)
        throw(std::runtime_error("Bad config file: repeated DELETE_path essential  💩"));
    server["DELETE"] = DELETE_path_vec[1];
    essentials_arg.erase("DELETE_path");

}
void    ServerFill::fillBodySize(std::vector<std::string> &bodySize_vec,  std::set<std::string> &essentials_arg, 
        stringmap &server)
{
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
        server["index"] += *it + " ";
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

// void    fillListen(std::set<std::string> essentials_arg, std::vector<std::string> single_essential)
// {

// }

void    ServerFill::fillEssentialArg(std::set<std::string>  &essentials_arg)
{
    essentials_arg.insert("listen"); 
    essentials_arg.insert("server_name");
    essentials_arg.insert("index");
    essentials_arg.insert("root");
    essentials_arg.insert("DELETE_path");
    essentials_arg.insert("client_max_body_size");
};
void    ServerFill::fillNoRepeateArg(std::set<std::string>  &no_repeate_arg)
{
    no_repeate_arg.insert("root");
    no_repeate_arg.insert("index");
    no_repeate_arg.insert("autoindex");
    no_repeate_arg.insert("redirection");
    no_repeate_arg.insert("methods");
    no_repeate_arg.insert("error_page");
    no_repeate_arg.insert("client_max_body_size");
    no_repeate_arg.insert("cgi-bin");
};