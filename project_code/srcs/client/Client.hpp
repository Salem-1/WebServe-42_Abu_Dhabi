#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../../includes/libs.hpp"

class Client
{
    public:
        Client(){};
        Client(int  client_socket);
        Client(const Client &obj2);
        Client &operator= (const Client &obj2);
        ~Client();
        void     handle_request();
        size_t  time;
        alive   state;
        int     client_socket;
    private:
};

#endif