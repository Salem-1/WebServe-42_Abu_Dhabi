#include "Parser.hpp"


Parser::Parser(): read_again(0), bytes_read(0), read_sock(0)
{

}
// Parser::Parser(char *buff, int bytes_read): read_again(0), buffer(buff), bytes_read(bytes_read)
// {

// }

Parser::Parser(const Parser &obj2)
{
    *this = obj2;
}

Parser &Parser::operator= (const Parser &obj2)
{
    if (this != &obj2)
    {

    }
    return (*this);
}
Parser::~Parser()
{

}

void    Parser::parse(char *new_buffer)
{
    if (!bytes_read)  
    {
        read_again = 0;
        return ;
    }
    std::string str(new_buffer);
    packet += str;

    std::cout<< "\ninside parser packet:\n" << packet <<std::endl;
    
    //TODO 
    //inshalla
    //check if the packet is complete, send to the appropriate method
    //get response 
    //else wait for another buffer 
    // switch packet[0]
    // {
    //     case 'G':
    //         GET_request.handle(std::string str(buffer), bytes_read);

    // }
}

void    Parser::set_byteread_and_readsock(int bytes, int sock)
{
    this->bytes_read = bytes;
    this->read_sock = sock;
}