#include "Config.hpp"

/*
    servers[0][".bla options"] = "post_cgi get_cgi";
    
   std::string extension;
   if (server_info.find(extension + " options") != server_info.end())
   {
        std::vector<std::string> options = split(server_info[extension + " options"], " ");
        if (options.find(post_cgi) != options.end())
        //dosomething
   }
*/
void    intra_and_dummy_fill_till_config_parsing(conf &server);
void    python_tester_config(conf &server);
void    list_dir_options(conf &servers);
void   intra_config(conf &servers);
Config::Config()
{
    char buff[4000];
    std::string pwd(getcwd(buff, sizeof(buff)));
    intra_and_dummy_fill_till_config_parsing(servers);
    // intra_config(servers);
    // list_dir_options(servers);

    fillPorts();
}


Config::~Config()
{

}

//"DELETE path" parameter for safety feature for our use  
void    Config::fillPorts()
{
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        if (servers[i].find("Port") != servers[i].end())
        {
            std::cout << "inserting " << servers[i]["Port"] << std::endl;
            ports.insert(servers[i]["Port"]);
            std::cout << "inserted " << servers[i]["Port"] << std::endl;
        }
        else
            throw(std::invalid_argument("Error in reading configuration file, missing port number in a server"));
    }
}

void    python_tester_config(conf &servers)
{

    char buff[4000];
    std::string pwd(getcwd(buff, sizeof(buff)));
    servers.push_back(std::map<std::string, std::string>());
    servers[0]["server_name"] = "default_server";
    servers[0]["error_page"] = "404 test/www/error404.html";
    servers[0]["Port"] = "9090";
    servers[0]["root"] = pwd + "/www";
    servers[0]["index"] = "index.html";
    servers[0]["/"] = servers[0]["root"] + "/" + servers[0] ["index"];
    servers[0]["/2"] = servers[0]["root"] + "/www";
    servers[0]["/2 index"] = servers[0]["root"] + "/js/" + "second.html";
    servers[0]["/virtual"] = servers[0]["root"] + "/www";
    servers[0]["/protected"] = servers[0]["root"] + "/www/protected";
    servers[0]["/post"] = servers[0]["root"] + "/www";
    servers[0]["/put"] = servers[0]["root"] + "/www";
    servers[0]["/index"] = servers[0]["root"] + "/www";
   
    servers.push_back(std::map<std::string, std::string>());
    servers[1]["server_name"] = "localhost default_server";
    servers[1]["Port"] = "9091";
    servers[1]["root"] = pwd + "/www";
    servers[1]["index"] = "second.html";
    servers[1]["/"] = servers[1]["root"] + "/" + servers[1] ["index"];

    servers.push_back(std::map<std::string, std::string>());
    servers[2]["server_name"] = "google.com";
    servers[2]["Port"] = "9090";
    servers[2]["root"] = pwd + "/www";
    servers[2]["index"] = "google.html";
    servers[2]["/"] = servers[2]["root"] + "/" + servers[2] ["index"];
    servers[2]["/auto"] = servers[2]["root"] + "/www/auto";
   
   
}



void    intra_and_dummy_fill_till_config_parsing(conf &servers)
{
    char buff[4000];
    std::string pwd(getcwd(buff, sizeof(buff)));
    servers.push_back(std::map<std::string, std::string>());
     servers[0]["server_name"] = "127.0.0.1";
    servers[0]["Port"] = "3490";
    servers[0]["root"] = pwd + "/intra/YoupiBanane";
    servers[0]["index"] = "youpi.bad_extension";
    servers[0]["/"] = servers[0]["root"] + "/" + servers[0]["index"];
    servers[0]["/ methods"] = "GET";
    servers[0]["Max-Body"] = "100";
    servers[0]["/directory"] = servers[0]["root"] + "/";
    servers[0]["/directory index"] = "youpi.bad_extension" ;
    servers[0]["/put_test methods"] = "PUT" ;
    servers[0]["/put_test"] = servers[0]["root"] + "/PUT/";
    // servers[0]["/Yeah autoindex"] = "off";
    servers[0]["DELETE path"] = "POST";
    
   
   
    servers.push_back(std::map<std::string, std::string>());
    servers[1]["DELETE path"] = "POST";
    servers[1]["server_name"] = "127.0.0.1 localhost";
    // servers[1]["server_name"] = "localhost";
    servers[1]["Port"] = "4444";
    servers[1]["root"] = pwd + "/intra/YoupiBanane";
    servers[1]["index"] = "youpi.bad_extension";
    servers[1]["/"] = servers[1]["root"] + "/" + servers[1] ["index"];
    servers[1]["/images"] = servers[1]["root"] + "/path_to_images/";
    servers[1]["/images index"] = servers[1]["/images"] + "base_image.jpg";
    servers[1]["/styles"] = servers[1]["root"] + "/styles/";
    servers[1]["/styles index"] = servers[1]["root"] + "/styles/" + "styles.css";
    servers[1]["/js"] = servers[1]["root"] + "/js/";
    servers[1]["/js index"] = servers[1]["root"] + "/js/" + "script.js";
    
   
    servers.push_back(std::map<std::string, std::string>());
    servers[2]["DELETE path"] = "/POST";
    servers[2]["server_name"] = "127.0.0.1 localhost";
    // servers[2]["server_name"] = "localhost";
    servers[2]["Port"] = "5555";
    servers[2]["root"] = pwd + "/intra/website";
    servers[2]["index"] = "index.html";
    servers[2]["/"] = servers[2]["root"] + "/" + servers[2] ["index"];
    servers[2]["/images"] = servers[2]["root"] + "/path_to_images/";
    servers[2]["/images index"] = servers[2]["/images"] + "base_image.jpg";
    servers[2]["/styles"] = servers[2]["root"] + "/styles/";
    servers[2]["/styles index"] = servers[2]["root"] + "/styles/" + "styles.css";
    servers[2]["/js"] = servers[2]["root"] + "/js/";
    servers[2]["/js index"] = servers[2]["root"] + "/js/" + "script.js";
    servers[2]["404"] = servers[2]["root"] +  "/" + "not_found.html";
	servers[2]["cgi-bin"] = pwd + "/intra";
    
    servers.push_back(std::map<std::string, std::string>());
    servers[3]["DELETE path"] = "POST";
    servers[3]["server_name"] = "defaultserver";
    servers[3]["Port"] = "4444";
    servers[3]["root"] = pwd + "/intra/website";
    servers[3]["index"] = "index.html";
    servers[3]["/"] = servers[3]["root"] + "/" + servers[3] ["index"];
    servers[3]["/images"] = servers[3]["root"] + "/path_to_images/";
    servers[3]["/images index"] = servers[3]["/images"] + "base_image.jpg";
    servers[3]["/styles"] = servers[3]["root"] + "/styles/";
    servers[3]["/styles index"] = servers[3]["root"] + "/styles/" + "styles.css";
    servers[3]["/js"] = servers[3]["root"] + "/js/";
    servers[3]["/js index"] = servers[3]["root"] + "/js/" + "script.js";
    servers[3]["Methods"] = "GET DELETE";
        
}

void   intra_config(conf &servers)
{
    char buff[4000];
    std::string pwd(getcwd(buff, sizeof(buff)));
    servers.push_back(std::map<std::string, std::string>());
    servers[0]["server_name"] = "127.0.0.1";
    servers[0]["Port"] = "3490";
    servers[0]["root"] = pwd + "/intra/YoupiBanane";
    servers[0]["index"] = "youpi.bad_extension";
    servers[0]["/"] = servers[0]["root"] + "/" + servers[0]["index"];
    servers[0]["/ methods"] = "GET";
    servers[0]["Max-Body"] = "100";
    servers[0]["/directory"] = servers[0]["root"] + "/";
    servers[0]["/directory index"] = "youpi.bad_extension" ;
    servers[0]["/put_test methods"] = "PUT" ;
    servers[0]["/put_test"] = servers[0]["root"] + "/PUT/";
    // servers[0]["/Yeah autoindex"] = "off";
    servers[0]["DELETE path"] = "POST";
}


/**
     servers.push_back(std::map<std::string, std::string>());
    servers[0]["server_name"] = "127.0.0.1";
    servers[0]["Port"] = "3490";
    servers[0]["root"] = pwd + "/intra/YoupiBanane";
    servers[0]["index"] = "youpi.bad_extension";
    servers[0]["/"] = servers[0]["root"] + "/" + servers[0] ["index"];
    servers[0]["/directory"] = servers[0]["root"];
    servers[0]["//directory index"] = servers[0]["/"] ;
    servers[0]["/styles"] = servers[0]["root"] + "/styles/";
    servers[0]["/styles index"] = servers[0]["root"] + "/styles/" + "styles.css";
    servers[0]["/js"] = servers[0]["root"] + "/js/";
    servers[0]["/js index"] = servers[0]["root"] + "/js/" + "script.js";
    servers[0]["DELETE path"] = "POST";
    servers[3]["Methods"] = "GET";




    ["server_name"] = "127.0.0.1";
    ["Port"] = "3490";
    ["root"] = pwd + "/intra/YoupiBanane";
    ["index"] = "youpi.bad_extension";
    ["/"] = servers[0]["root"] + "/" + servers[0]["index"];
    ["/ methods"] = "GET";
    ["Max-Body"] = "100";
    ["/directory"] = servers[0]["root"] + "/";
    ["/directory index"] = "youpi.bad_extension" ;
    ["/put_test methods"] = "PUT" ;
    ["/put_test"] = servers[0]["root"] + "/PUT/";
    ["/Yeah autoindex"] = "off";
    ["/post_body max_lenghth"] = "100";
    ["/post_body methods"] = "POST";
    ["DELETE path"] = "POST";
*/


void    list_dir_options(conf &servers)
{
    char buff[4000];
    std::string pwd(getcwd(buff, sizeof(buff)));
    servers.push_back(std::map<std::string, std::string>());
    servers[0]["DELETE path"] = "";
    servers[0]["server_name"] = "127.0.0.1 localhost";
    servers[0]["Port"] = "5555";
    servers[0]["root"] = pwd + "/intra/website";
    servers[0]["index"] = "index.html";
    servers[0]["/"] = servers[0]["root"] + "/" + servers[0] ["index"];
    servers[0]["404"] = servers[0]["root"] +  "/" + "not_found.html";
    
    servers[0]["/attacks methods"] = "GET POST";
    servers[0]["/dir methods"] = "DELETE";
    servers[0]["/POST methods"] = "POST";
    servers[0]["/nightmares methods"] = "GET";
    servers[0]["/sleep"] = servers[0]["root"] +  "/nightmares/";
    servers[0]["/sleep index"] = servers[0]["/sleep"] + "index.html";
    servers[0]["/nightmares autoindex"] = "on";
    servers[0]["/attacks autoindex"] = "off";
    servers[0]["/secret autoindex"] = "off";
    
    
    servers[0]["Max-Body"] = "8000";
    servers[0]["Redirections"] = "/ransomware attacks/ransom.html  302 , /ddos attacks/ddos.html 301";
}
