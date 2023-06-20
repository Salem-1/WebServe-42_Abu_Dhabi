#include "Config.hpp"

Config::Config()
{
    // std::cout << "filling sample conf file " << std::endl;
    // servers.push_back(std::map<std::string, std::string>());
    // servers[0]["server_name"] = "127.0.0.1";
    // servers[0]["Port"] = "3490";
    // servers[0]["root"] = "/Users/ahsalem/projects/cursus/webserve/project_code/testers/webserv_tester/www";
    // servers[0] ["index"] = "index.html";
    // servers[0] ["/"] = servers[0]["root"] + "/" + servers[0] ["index"];
    // servers[0] ["/images"] = servers[0]["root"] + "/images/";
    // servers[0] ["/styles"] = servers[0]["root"] + "/styles/";
    // servers[0] ["/js"] = servers[0]["root"] + "/js/";
    //intra tester config
    // servers[0]["root"] = "/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/intra/YoupiBanane";
    // servers[0] ["index"] = "youpi.bad_extension";
    //webserve tester config
    
    servers.push_back(std::map<std::string, std::string>());
    servers[0]["server_name"] = "127.0.0.1";
    servers[0]["Port"] = "3490";
    servers[0]["root"] = "/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/intra/YoupiBanane";
    servers[0]["index"] = "youpi.bad_extension";
    servers[0]["/"] = servers[0]["root"] + "/" + servers[0] ["index"];
    servers[0]["/images"] = servers[0]["root"] + "/images/";
    servers[0]["/styles"] = servers[0]["root"] + "/styles/";
    servers[0]["/js"] = servers[0]["root"] + "/js/";
    
    // servers.push_back(std::map<std::string, std::string>());
    // servers[2]["server_name"] = "127.0.0.1";
    // servers[2]["Port"] = "3491";
    // servers[2]["root"] = "/Users/ahsalem/projects/cursus/webserve/project_code/testers/our_tester/website";
    // servers[2]["index"] = "index.html";
    // servers[2]["/"] = servers[0]["root"] + "/" + servers[0] ["index"];
    // servers[2]["/images"] = servers[0]["root"] + "/images/";
    // servers[2]["/styles"] = servers[0]["root"] + "/styles/";
    // servers[2]["/js"] = servers[0]["root"] + "/js/";
    // //intra tester config
    //webserve tester config
    
}

Config::~Config()
{

}

