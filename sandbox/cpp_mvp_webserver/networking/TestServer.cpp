#include "TestServer.hpp"


ft::TestServer::TestServer():
    SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY ,10)
{
     so = new ListeningSocket(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY ,10);
    address = so->get_address();
    addrlen = sizeof(address);
    launch();
}

void    ft::TestServer::accepter()
{
   
    new_socket = accept(so->get_sock(),
        (struct sockaddr*)&address, (socklen_t *)addrlen);
    read(new_socket, buffer, 3000);

}

void    ft::TestServer::handler()
{
    std::cout << buffer << std::endl;
}

void    ft::TestServer::responder()
{
        char *hello = "Hello from server";
        write (new_socket, hello, strlen(hello));
        close(new_socket);
}

void    ft::TestServer::launch()
{
    while(1)
    {
        std::cout << " Waititng.............." << std::endl;
        accepter();
        handler();
        responder();
        std::cout << " Done.............." << std::endl;
        sleep(1);
    }
}





