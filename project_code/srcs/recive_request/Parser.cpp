#include "Parser.hpp"


Parser::Parser(): read_again(0), buffer(NULL), bytes_read(0)
{

}
Parser::Parser(char *buff, int bytes_read): read_again(0), buffer(buff), bytes_read(bytes_read)
{

}

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

}