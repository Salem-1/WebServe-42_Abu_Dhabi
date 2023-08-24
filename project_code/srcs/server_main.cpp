#include "../includes/libs.hpp"
#include "../includes/webserve.hpp"


void handle_pipes(int sig)
{
    if (sig == SIGPIPE)
    {
        std::cout << MAGENTA  << "\nBroken pipe: Client disconnected during sending " << RESET <<std::endl;
    }
}
void vvisualize_tokens(tokenized_conf &tokens);
int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "Error: too many arguments" << std::endl;
		exit(1);
	}

	tokenized_conf configstarter;

	if (argc == 2)
	{
		ConfigHandler config(argv[1]);
		config.handleConfig();
		configstarter = config.getConfigstarter();
        vvisualize_tokens(configstarter);
        ServerFill filler(configstarter);
        filler.parseTokens();
    
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


void vvisualize_tokens(tokenized_conf &tokens)
{
    for (tokenized_conf::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        std::cout << BOLDYELLOW << "essentials: " << RESET << std::endl;
        std::cout << BOLDMAGENTA << it->first << RESET << std::endl;
        for (size_t i = 0; i < it->second.size(); i++)
        {
            std::cout << BOLDYELLOW << "Location " << i << RESET << std::endl;
            std::cout << it->second[i] << RESET << std::endl;
        }
    }
}
