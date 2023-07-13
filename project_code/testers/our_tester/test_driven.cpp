#include "../../includes/webserve.hpp"

int main()
{
    ErrResponse err;
    Config  servers;
    std::cout << "default error 400 = /n";
    std::cout << err.code(servers.servers[0], "400") << std::endl;; 
    (void)err;
    std::cout << "test err ends" << std::endl;
}

