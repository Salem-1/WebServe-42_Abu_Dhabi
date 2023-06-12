#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../../includes/libs.hpp"
# include "../recive_request/Receive.hpp"

class Client
{
    public:
        Client(){};
        Client(int  client_socket);
        Client(const Client &obj2);
        Client &operator= (const Client &obj2);
        ~Client();
        void                handle_request();
        int                 get_timeout();
        connection_state    state;
        int                 client_socket;
        clock_t             start_time;
        int                 connection_duration;
        Receive             receiver;

    private:
};

#endif