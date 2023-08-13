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

	if (argc == 2)
	{
		ConfigHandler config(argv[1]);
		config.readConfigFile();
		exit(0);
	}

    Config  servers;
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



// void    running_one_server(std::map<std::string, std::string> one_server)
// {
//     Listner binded_sock(one_server);

//     printf("server waiting for connection....\n");
//     Kque socket_manager(binded_sock.sockfd);
//     socket_manager.watchFds(one_server);
// }

// struct ServerRunner
// {
//     std::map<std::string, std::string> server;

//     ServerRunner(const std::map<std::string, std::string>& server)
//         : server(server)
//     {
//     }

//     void operator()()
//     {
//         running_one_server(server);
//     }
// };

// int main()
// {
//     Config  servers;
    
//     std::vector<std::thread> servers_threads;

//     for (unsigned long i = 0; i < servers.servers.size(); i++)
//     {
//         ServerRunner serverRunner(servers.servers[i]);

//         servers_threads.push_back(std::thread(serverRunner));  
//     }
//     for (unsigned long i = 0; i < servers_threads.size(); i++)
//     {
//         servers_threads[i].join();
//     }
//     visualize_servers(servers.servers);
//     return (0);
// }
