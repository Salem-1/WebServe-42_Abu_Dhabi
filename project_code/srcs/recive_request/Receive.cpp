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
    // memset(buffer, 0, BUFFER_SIZE);
    read_packet(buffer);
    parser.set_byteread_and_readsock(bytes_read, read_sock);
    parser.parse(buffer);
    if (bytes_read == 0)
        state = KILL_CONNECTION;
    else if (parser.read_again)
        receive_all();
    return ;
}


void    Receive::read_packet(char *buffer)
{
    bytes_read = recv(read_sock, buffer, BUFFER_SIZE, 0);
    if (bytes_read == -1)
    {
        perror("recv Error: ");
        state = KILL_CONNECTION;
        bytes_read= 0;
        return ;
    }
}

std::map<std::string, std::vector<std::string> >       &Receive::get_request_packet()
{
    return (parser.request);
}