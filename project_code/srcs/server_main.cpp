#include "../includes/libs.hpp"
#include "../includes/webserve.hpp"


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

	std::vector<std::pair<std::string, std::vector<std::string> > > configstarter;

	if (argc == 2)
	{
		ConfigHandler config(argv[1]);
		config.handleConfig();
		configstarter = config.getConfigstarter();
	}

    
	exit(0); // Remove this and remember to go to ConfigHandler.cpp and remove the exit(0)
			// at TODO and convert the errorAndExit() to throw an exception
	Config  servers;
    // Config.fillPorts();
    signal(SIGPIPE, &handle_pipes);

    for (std::set<std::string>::iterator it = servers.ports.begin();
            it != servers.ports.end(); ++it)
    {
        std::cout << "port " << *it << std::endl;
        Listner binded_sock(*it);
        servers.sockets.push_back(binded_sock.sockfd);
    }
    std::cout << "server waiting for connection....\n" << std::endl;
    Kque socket_manager(servers.sockets);
    socket_manager.watchFds(servers.servers);
    return (0);
}
