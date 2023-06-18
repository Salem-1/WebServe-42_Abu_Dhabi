#ifndef LISTNER_HPP
# define LISTNER_HPP
#include "Socket.hpp"

class Listner: public Socket
{
     public:
        typedef std::map<std::string, std::string>  conf; 
    public:
        Listner(conf server);
        ~Listner();
        int get_sockfd();
}; 
#endif