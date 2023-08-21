#include "../includes/libs.hpp"
#include "../includes/webserve.hpp"


tokenized_conf    dummy_intra_token_fill();
void              run_server();
void handle_pipes(int sig)
{
    if (sig == SIGPIPE)
    {
        std::cout << MAGENTA  << "\nBroken pipe: Client disconnected during sending " << RESET <<std::endl;
    }
}
int main(int argc, char **argv)
{
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

    run_server();

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

void    run_server()
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
    locations.push_back("location /cgi-bin; root ../intra;");
    locations.push_back("location /put_test; methods PUT;  root /PUT/;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    
    locations.clear();
    essentials = " listen      4444; server_name 127.0.0.1 localhost; root        /intra/YoupiBanane; DELETE_path POST; index       youpi.bad_extension;";    
    locations.push_back("location /;  index   youpi.bad_extension;;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    
    locations.clear();
    essentials = " listen      5555; server_name 127.0.0.1 localhost; root        /intra/website; index       index.html;";    
    locations.push_back("location / ; index   index.html; error_page 404 not_found.html;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
   
    locations.clear();
    essentials = "    listen      4444; server_name defaultserver; DELETE_path POST; root        /intra/website; index       index.html;";    
    locations.push_back("location / ; methods GET DELETE;");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));

    ServerFill filled_servers(tokenized_server);
    filled_servers.servers.fillPorts();
    signal(SIGPIPE, &handle_pipes);

    for (std::set<std::string>::iterator it = filled_servers.servers.ports.begin();
            it != filled_servers.servers.ports.end(); ++it)
    {
        std::cout << "port " << *it << std::endl;
        Listner binded_sock(*it);
        filled_servers.servers.sockets.push_back(binded_sock.sockfd);
    }
    std::cout << "server waiting for connection....\n" << std::endl;
    Kque socket_manager(filled_servers.servers.sockets);
    socket_manager.watchFds(filled_servers.servers.servers);

}