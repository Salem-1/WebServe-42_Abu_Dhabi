#include "tester.hpp"



void handle_pipes(int sig);
void    runServer(Config &servers);

int main()
{
    test_Essentials();
    test_lcation_firstLine();
    test_location_root();
    test_index();
    test_autoindex();
    test_methods();
    test_error_page();
    test_redirection();
    test_cgi_bin();
    test_bodySize_location();
    test_cgi_extentions_location();
    non_sensedirective_location();
    test_no_root_no_location_root();
    test_multiple_servers();
    test_DELETE_path();
    test_root_index();
    test_intra_config();
    test_dummy_intra_fill_config();
    // check_redirection_out();
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