#include "GET.hpp"

GET::GET(): packet_counter(0), i(0)
{

}

GET::~GET()
{}

GET &GET::operator= (const GET &obj2)
{
    if(this != &obj2)
    {

    }
    return (*this);
}

GET::GET(const GET &obj2)
{
    *this = obj2;
}

// void    GET::handle(std::string buffer, int len)
// {
//     i =  0;
//     for (i; i < len; i++)
//     {
//         switch (packet_counter)
//         {
//             case 0:
//                 check_method(buffer, len);
//         }
//         (void)i;
//     }
// }
// int GET::check_method(std::string buffer, int len)
// {
//     if (len > 3)
//     {
//         if (buffer[0] == 'G' && buffer[1] == 'E' 
//             && buffer[2] == 'T'  && buffer[3] == ' ')
//             request["method"] = "GET";
//         else
//             request["method"] = "invalid";
//     }
//     else
//         ;
// }