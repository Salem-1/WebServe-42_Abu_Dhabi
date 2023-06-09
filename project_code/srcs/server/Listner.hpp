#ifndef LISTNER_HPP
# define LISTNER_HPP
#include "Socket.hpp"

class Listner: public Socket
{
    public:
        Listner(std::string PORT);
        ~Listner();
        int get_sockfd();
}; 
#endif