#include "Config.hpp"

Config::Config()
{
    std::cout << "filling sample conf file " << std::endl;
    servers.push_back(std::map<std::string, std::string>());
    servers[0]["server_name"] = "127.0.0.1";
    servers[0]["Port"] = "3490";
    servers[0]["root"] = "/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/intra/YoupiBanane";
    servers[0] ["index"] = "youpi.bad_extension";
    servers[0] ["/"] = servers[0]["root"] + "/" + servers[0] ["index"];
    servers[0] ["/images"] = servers[0]["root"] + "/images/";
    servers[0] ["/styles"] = servers[0]["root"] + "/styles/";
    servers[0] ["/js"] = servers[0]["root"] + "/js/";
    
}

Config::~Config()
{

}

