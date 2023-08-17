#include "tester.hpp"



void handle_pipes(int sig);
void    runServer(Config &servers);

int main()
{
    tokenized_conf tokenized_server = dummy_conf_tokens();
    ServerFill    fill(tokenized_server);
    // fill.servers.visualize_config();
    // visualize_tokens(fill._conf_tokens);
    test_emptyEssentials(fill);
    test_lenEssentials(fill);
    test_mixSpacesEssentials(fill);
    test_essentialOccurance(fill);
    test_listenConf(fill);
    test_hostNameConf(fill);
    test_rootConf(fill);
    test_indexConf(fill);
    test_bodySizeConf(fill);
    test_manyConfs(fill);
    test_repeated_port();
    // runServer(servers);
    return (0);
}





















void    runServer(Config &servers)
{
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
}

void handle_pipes(int sig)
{
    if (sig == SIGPIPE)
    {
        std::cout << MAGENTA  << "\nBroken pipe: Client disconnected during sending " << RESET <<std::endl;
    }
}