#include "../includes/libs.hpp"
#include "../includes/webserve.hpp"


tokenized_conf    dummy_intra_token_fill();

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    (void)env;
	if (argc > 2)
	{
		std::cerr << "Error: too many arguments" << std::endl;
		exit(1);
	}

	if (argc == 2)
	{
		ConfigHandler config(argv[1]);
		config.readConfigFile();
		exit(0);
	}

    run_server(env);

    return (0);
}

tokenized_conf    dummy_intra_token_fill()
{
    tokenized_conf tokenized_server;
    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;server_name 127.0.0.1  ;root /intra/YoupiBanane;index youpi.bad_extension;\nDELETE_path POST; client_max_body_size 10000000000;";    
    locations.push_back("location /; methods GET   ;");
    locations.push_back("location /directory; \nroot /;\nindex youpi.bad_extension;\n");
    locations.push_back("    location /post_body ;\n methods POST ;\nclient_max_body_size 100;\n");
    locations.push_back("location .bla;index   /../cgi-bin/cgi_tester;\n");
    locations.push_back("location /cgi-bin; root ../intra;");
    locations.push_back("location /put_test; methods PUT;  root /PUT/;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    return (tokenized_server);
}

void    fillEnvPath(conf &servers, char **env)
{
    std::string path = "";
    std::string     one_env;
    if (env != NULL)
    {
        for (int i = 0; env[i] != NULL; i++)
        {
            one_env.append(env[i]);
            if (one_env.find("PATH=") == 0)
            {
                path = one_env.substr(5, one_env.length() - 1);
                break ; 
            }
            one_env.clear();

        }
    }

    for (conf::iterator it = servers.begin(); it != servers.end()
            ; it++)
        (*it)["path to path"] = path;
}

void    run_server(char **env)
{
    // Config  servers;
// tokenized_conf tokenized_server;
//     std::vector<std::string> locations;
//     std::string              essentials = "listen 3490;server_name 127.0.0.1  ;root /intra/YoupiBanane;index youpi.bad_extension;\nDELETE_path POST; client_max_body_size 10000000000;";    
//     locations.push_back("location /; methods GET   ;");
//     locations.push_back("location /directory; \nroot /;\nindex youpi.bad_extension;\n");
//     locations.push_back("    location /post_body ;\n methods POST ;\nclient_max_body_size 100;\n");
//     locations.push_back("location .bla;index   /../cgi-bin/cgi_tester;\n");
//     locations.push_back("location /cgi-bin; root ../intra;");
//     locations.push_back("location /put_test; methods PUT;  root /PUT/;");
//     tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    tokenized_conf tokenized_server;
    std::vector<std::string> locations;
     std::string              essentials = "listen 3490;server_name 127.0.0.1  ;root /intra/YoupiBanane;index youpi.bad_extension;\nDELETE_path POST; client_max_body_size 10000000000;";    
    locations.push_back("location /; methods GET   ;");
    locations.push_back("location /directory; \nroot /;\nindex youpi.bad_extension;\n");
    locations.push_back("    location /post_body ;\n methods POST ;\nclient_max_body_size 100;\n");
    locations.push_back("location .bla;index   /../cgi-bin/cgi_tester;\n");
    locations.push_back("location /cgi-bin; root /..;");
    locations.push_back("location /put_test; methods PUT;  root /PUT/;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    
    locations.clear();
    essentials = " listen      4444; server_name 127.0.0.1 localhost; root        /intra/YoupiBanane; DELETE_path POST; index       youpi.bad_extension;";    
    locations.push_back("location /;  index   youpi.bad_extension;;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    
    locations.clear();
    essentials = " listen      5555; server_name 127.0.0.1 localhost; root        /intra/website; client_max_body_size 5; index       index.html;";    
    locations.push_back("location / ; index   index.html; error_page 404 not_found.html;");
    locations.push_back("location  /post_backend ;  client_max_body_size 50; ");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
   
    locations.clear();
    essentials = "    listen      4444; server_name defaultserver; DELETE_path POST; root        /intra/website; index       index.html;";    
    locations.push_back("location / ; methods GET DELETE;");
    locations.push_back("location  /attacks ;  autoindex off;root /attacks/ ;index ddos.html;");
    locations.push_back("location /strike;         root /attacks/; redirection /fall /nightmares/fall.html  301; redirection  /another_redir  /attacks/ransom.html 301;redirection /ransomware /strike/another_redir 302;  autoindex on;");
    // redirection /monseter nightmares/monster.html  301; error_page 404 not_found.html; error_page 403 monster;redirection  /another_redir  attacks/ransom.html 301; redirection /ddos attacks/ddos.html 301;
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
   
    ServerFill filled_servers(tokenized_server);
    filled_servers.servers.visualize_config();
    filled_servers.servers.fillPorts();
    fillEnvPath(filled_servers.servers.servers, env);
    runAtAllCost();


    try 
	{
		for (std::set<std::string>::iterator it = filled_servers.servers.ports.begin();
				it != filled_servers.servers.ports.end(); ++it)
		{
			std::cout << "port " << *it << std::endl;
			Listner binded_sock(*it);
			binded_sock.bindFD();
			filled_servers.servers.sockets.push_back(binded_sock.sockfd);
		}
		std::cout << "server waiting for connection....\n" << std::endl;
		Kque socket_manager(filled_servers.servers.sockets);
		socket_manager.watchFds(filled_servers.servers.servers);
		
	}
	catch (const std::exception& e)
	{
		std::string err(e.what());
		std::cout << BOLDRED << e.what() << RESET << std::endl;
	}
	return ;

}

