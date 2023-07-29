/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:20 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/29 10:52:36 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

void    intra_and_dummy_fill_till_config_parsing(conf &server);
void    python_tester_config(conf &server);
Config::Config()
{
    char buff[4000];
    std::string pwd(getcwd(buff, sizeof(buff)));
    // python_tester_config(servers);
    intra_and_dummy_fill_till_config_parsing(servers);

    fillPorts();
}

Config::~Config()
{

}

void    Config::fillPorts()
{
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        if (servers[i].find("Port") != servers[i].end())
        {
            std::cout << "inserting " << servers[i]["Port"] << std::endl;
            ports.insert(servers[i]["Port"]);
            std::cout << "inserted " << *(ports.begin()) << std::endl;
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
    servers[0]["/"] = servers[0]["root"] + "/" + servers[0] ["index"];
    servers[0]["/images"] = servers[0]["root"] + "/path_to_images/";
    servers[0]["/images index"] = servers[0]["/images"] + "base_image.jpg";
    servers[0]["/styles"] = servers[0]["root"] + "/styles/";
    servers[0]["/styles index"] = servers[0]["root"] + "/styles/" + "styles.css";
    servers[0]["/js"] = servers[0]["root"] + "/js/";
    servers[0]["/js index"] = servers[0]["root"] + "/js/" + "script.js";
    servers[0]["DELETE path"] = "POST";
   
   
    servers.push_back(std::map<std::string, std::string>());
    servers[1]["DELETE path"] = "POST";
    servers[1]["server_name"] = "127.0.0.1";
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
    servers[2]["DELETE path"] = "POST";
    servers[2]["server_name"] = "127.0.0.1";
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
    servers[2]["403"] = servers[2]["root"] +  "/" + "not_found.html";
    
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
        
}

void    intra_tester_config(conf &servers)
{
    char buff[4000];
    std::string pwd(getcwd(buff, sizeof(buff)));
    servers.push_back(std::map<std::string, std::string>());
    servers[0]["server_name"] = "127.0.0.1";
    servers[0]["Port"] = "3490";
    servers[0]["root"] = pwd + "/intra/YoupiBanane";
    servers[0]["index"] = "youpi.bad_extension";
    servers[0]["/"] = servers[0]["root"] + "/" + servers[0] ["index"];
    servers[0]["/images"] = servers[0]["root"] + "/path_to_images/";
    servers[0]["/images index"] = servers[0]["/images"] + "base_image.jpg";
    servers[0]["/styles"] = servers[0]["root"] + "/styles/";
    servers[0]["/styles index"] = servers[0]["root"] + "/styles/" + "styles.css";
    servers[0]["/js"] = servers[0]["root"] + "/js/";
    servers[0]["/js index"] = servers[0]["root"] + "/js/" + "script.js";
    servers[0]["DELETE path"] = "POST";
}