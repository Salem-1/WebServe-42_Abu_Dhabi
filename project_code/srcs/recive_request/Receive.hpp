#ifndef RECEIVE_HPP
# define RECEIVE_HPP

# include "../../includes/libs.hpp"

class Receive
{
    public:
        Receive();
        Receive(int read_sock);
        Receive(const Receive &obj2);
        Receive &operator= (const Receive &obj2);
        ~Receive();
        void                receive_all();
        int                 read_sock;
        int                 bytes_read;
        char                buffer[BUFFER_SIZE];
        connection_state    state;
        // Parser              parse_row;
        
    private:
};
#endif