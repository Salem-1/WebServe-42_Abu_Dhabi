#include "Receive.hpp"

Receive::Receive(): state(KEEP_ALIVE)
{
};

Receive::Receive(int read_sock): read_sock(read_sock), state(KEEP_ALIVE)
{
};

Receive &Receive::operator= (const Receive &obj2)
{
    if (this != &obj2)
    {
        this->read_sock = obj2.read_sock;
        this->bytes_read = obj2.bytes_read;
        this->state = obj2.state;
    }
    return (*this);
}
Receive:: Receive(const Receive &obj2)
{
    *this = obj2;
}

Receive::~Receive()
{}

void    Receive::receive_all()
{
    bytes_read = recv(read_sock, buffer, BUFFER_SIZE, 0);
    //all parsing in the program happens here inshalla
    
    //parsing ends
    if (bytes_read == 0)
        state = KILL_CONNECTION;
    else
    {
        printf("recieved data from client\n");
        printf("%.*s/n", (int)bytes_read, buffer);
    }
}

