#include "Socket.hpp"

Socket::Socket(): try_again(1)
{
    std::cout << "Hello from the socket" << std::endl;
}

Socket::Socket(std::string PORT): servername(PORT), sockfd(0), try_again(1)
{

    get_my_addinfo();
    for (struct  addrinfo    *tr_addr = res; tr_addr != NULL && try_again; tr_addr = tr_addr->ai_next)
    {
        try_addr = tr_addr;
        open_socket(tr_addr);
    }
    if (!try_addr)
        error_in_socket("couldn't open socket: ");
    non_block_sock();
};

void    Socket::try_open_socket_again( struct  addrinfo *try_another_info)
{
    for (struct addrinfo* addr = try_another_info; addr != NULL && try_again; addr = addr->ai_next)
    {
        try_addr = addr;
        open_socket(addr);
    }
    if (!try_addr)
        error_in_socket("couldn't open socket: ");
    non_block_sock();
}

Socket::~Socket()
{
    freeaddrinfo(res);
}


void    Socket::get_my_addinfo(void)
{
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags= AI_PASSIVE;
    if (getaddrinfo(NULL, servername.c_str(), &hints, &res) < 0)
        error_in_socket("Addrinfo inside socket Error: ");
}

void    Socket::open_socket(struct  addrinfo    *try_addr)
{
    sockfd = socket(try_addr->ai_family,  try_addr->ai_socktype, try_addr->ai_protocol);

    if (sockfd < 0)
        perror("open_socket Error: ");
    else
        try_again = 0;
}

void    Socket::non_block_sock(void)
{
    int yes = 1;
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
        error_in_socket("fcntl nonblock error: ");
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
        error_in_socket("error setting socket option: ");

}
void    Socket::error_in_socket(std::string err)
{
    perror(err.c_str());
    freeaddrinfo(res);
    exit(1);   
}