#include "../includes/webserve.hpp"
#include "../includes/libs.hpp"


int main()
{
    Config  servers;
    //create set of needed ports
    for (std::set<std::string>::iterator it = servers.ports.begin();
            it != servers.ports.end(); ++it)
    {
        std::cout << "port " << *it << std::endl;
        Listner binded_sock(*it);
        servers.sockets.push_back(binded_sock.sockfd);
        usleep(500);
    }
    std::cout << "server waiting for connection....\n" << std::endl;
    Kque socket_manager(servers.sockets);
    socket_manager.watchFds(servers.servers);
    //open sockets in array of sockets
    //give the array of sockets to the kque, 
    //make all in loops in side kque, which is the add initial socket ot the kq as events as I expect
    //give teh Config to all clients
    //inside the request parse for server name
    //celebrate
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
