#include "../includes/libs.hpp"
#include "../includes/webserve.hpp"


void                handle_pipes(int sig);
static void         runServer(void);
tokenized_conf      fillDummyIntra();

int main(int argc, char **argv)
{
    (void)argc; (void)argv;
	// if (argc > 2)
	// {
	// 	std::cerr << "Error: too many arguments" << std::endl;
	// 	exit(1);
	// }
	// if (argc == 2)
	// {
	// 	ConfigHandler config(argv[1]);
	// 	config.readConfigFile();
	// 	exit(0);
	// }
    runServer();
    return (0);
}


static void    runServer()
{
    // Config  servers;
    tokenized_conf intra_filled = fillDummyIntra();
    ServerFill fillServers(intra_filled);

    fillServers.visualizeTokens();
    fillServers.servers.visualize_config();
    fillServers.servers.fillPorts();
 
    signal(SIGPIPE, &handle_pipes);

    for (std::set<std::string>::iterator it = fillServers.servers.ports.begin();
            it != fillServers.servers.ports.end(); ++it)
    {
        std::cout << "port " << *it << std::endl;
        Listner binded_sock(*it);
        fillServers.servers.sockets.push_back(binded_sock.sockfd);
    }
    std::cout << "server waiting for connection....\n" << std::endl;
    Kque socket_manager(fillServers.servers.sockets);
    socket_manager.watchFds(fillServers.servers.servers);
}

void handle_pipes(int sig)
{
    if (sig == SIGPIPE)
    {
        std::cout << MAGENTA  << "\nBroken pipe: Client disconnected during sending " << RESET <<std::endl;
    }
}

tokenized_conf   fillDummyIntra()
{
    tokenized_conf tokenized_server;
    std::vector<std::string> locations;
    std::string              essentials = "listen 3490;server_name 127.0.0.1  ;root /intra/YoupiBanane;index youpi.bad_extension;\nDELETE_path POST;";    
    locations.push_back("location /; methods GET   ;");
    locations.push_back("location /directory; \nroot /intra/YoupiBanane;\nindex youpi.bad_extension;\n");
    locations.push_back("    location /post_body ;\nmethods POST ;\nclient_max_body_size 100;\n");
    locations.push_back("location .bla;index   /cgi-bin/cgi_tester;\n");
    tokenized_server.push_back(std::pair<std::string, std::vector<std::string> > (essentials, locations));
    return (tokenized_server);
}
