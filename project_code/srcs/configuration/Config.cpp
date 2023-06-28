/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:20 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/28 13:05:26 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config()
{
    char buff[4000];
    std::string pwd(getwd(buff));
    std::cout << "pwd = " << pwd << std::endl;
    sleep(1);
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
    // servers[0]["root"] = pwd + "/intra/YoupiBanane";
    // servers[0] ["index"] = "youpi.bad_extension";
    //webserve tester config
    
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
    servers[2]["root"] = pwd + "/website";
    servers[2]["index"] = "index.html";
    servers[2]["/"] = servers[2]["root"] + "/" + servers[2] ["index"];
    servers[2]["/images"] = servers[2]["root"] + "/path_to_images/";
    servers[2]["/images index"] = servers[2]["/images"] + "base_image.jpg";
    servers[2]["/styles"] = servers[2]["root"] + "/styles/";
    servers[2]["/styles index"] = servers[2]["root"] + "/styles/" + "styles.css";
    servers[2]["/js"] = servers[2]["root"] + "/js/";
    servers[2]["/js index"] = servers[2]["root"] + "/js/" + "script.js";
    
    servers.push_back(std::map<std::string, std::string>());
    servers[3]["DELETE path"] = "POST";
    servers[3]["server_name"] = "localhost";
    servers[3]["Port"] = "6666";
    servers[3]["root"] = pwd + "/website";
    servers[3]["index"] = "index.html";
    servers[3]["/"] = servers[3]["root"] + "/" + servers[3] ["index"];
    servers[3]["/images"] = servers[3]["root"] + "/path_to_images/";
    servers[3]["/images index"] = servers[3]["/images"] + "base_image.jpg";
    servers[3]["/styles"] = servers[3]["root"] + "/styles/";
    servers[3]["/styles index"] = servers[3]["root"] + "/styles/" + "styles.css";
    servers[3]["/js"] = servers[3]["root"] + "/js/";
    servers[3]["/js index"] = servers[3]["root"] + "/js/" + "script.js";
    
    // servers.push_back(std::map<std::string, std::string>());
    // servers[2]["server_name"] = "127.0.0.1";
    // servers[2]["Port"] = "3491";
    // servers[2]["root"] = pwd + "/website";
    // servers[2]["index"] = "index.html";
    // servers[2]["/"] = servers[0]["root"] + "/" + servers[0] ["index"];
    // servers[2]["/images"] = servers[0]["root"] + "/images/";
    // servers[2]["/styles"] = servers[0]["root"] + "/styles/";
    // servers[2]["/js"] = servers[0]["root"] + "/js/";
    // //intra tester config

    //webserve tester config
 for (unsigned long i = 0; i < servers.size(); i++)
    {
        std::cout << "i = " << i << std::endl;
        if (servers[i].find("Port") != servers[i].end())
        {
            std::cout << "inserting " << servers[i]["Port"] << std::endl;
            ports.insert(servers[i]["Port"]);
            std::cout << "inserted " << *(ports.begin()) << std::endl;
        }
        else
            throw(std::invalid_argument("Error in reading configuration file, missing port number in a server"));
    }
    // fill_ports();
}

Config::~Config()
{

}

void    Config::fill_ports()
{
    // for (conf::iterator it = servers.begin(); it != servers.end(); it++)
    // {
    for (unsigned long i = 0; i < servers.size(); i++)
    {
        if (servers[i].find("Port") != servers[i].end())
        {
            std::cout << "inserting " << servers[i]["PORT"] << std::endl;
            ports.insert(servers[i]["PORT"]);
            std::cout << "inserted " << *(ports.begin()) << std::endl;
        }
        else
            throw(std::invalid_argument("Error in reading configuration file, missing port number in a server"));
    }
}