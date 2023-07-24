/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:58 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/23 14:14:27 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
    memset(buffer, 0, BUFFER_SIZE);
    read_packet(buffer);
    parser.set_byteread_and_readsock(bytes_read, read_sock);
    parser.parse(buffer);
    vis_str(buffer, "inside recievea;");
    if (bytes_read == 0)
    {
        std::cout << "byter read = " << bytes_read << "will kill connection\n";
        state = KILL_CONNECTION;
        return ;
    }
    if (parser.read_again)
        state = KEEP_ALIVE;
    return ;
}


void    Receive::read_packet(char *buffer)
{
    bytes_read = recv(read_sock, buffer, BUFFER_SIZE, 0);
    if (strlen(buffer) < 10000)
        std::cout << bytes_read <<" bytes_read buffer is\n<"<< buffer << ">" << std::endl;
    else
        std::cout << "recieved a large packet\n";
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