#include "../includes/libs.hpp"
#include "../includes/webserve.hpp"


void handle_pipes(int sig)
{
    if (sig == SIGPIPE)
    {
        std::cout << MAGENTA  << "\nBroken pipe: Client disconnected during sending " << RESET <<std::endl;
    }
}

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    (void)env;

    Config  servers;
    
    fillEnvPath(servers.servers, env);
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
