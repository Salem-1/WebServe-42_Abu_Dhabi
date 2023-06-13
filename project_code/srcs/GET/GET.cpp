#include "GET.hpp"

GET::GET(): packet_counter(0), i(0)
{
    fill_valid_headers();
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

void    GET::handle(packet_map packet)
{
    request = packet;
    std::cout << "visualizing insid Get request" << std::endl;
    visualize_request_packet();
    fill_response();
}

void    GET::fill_response()
{
    response["path"] = request["GET"][0];
    response["HTTP version"] = request["GET"][1];
};



void    GET::visualize_request_packet()
{
    std::cout << "visualizing requesest packet API\n\n" << std::endl;
    std::cout << "{" << std::endl;
    for (packet_map::iterator hit = request.begin();
            hit != request.end(); ++hit)
    {
        std::cout << "\"" << hit->first << "\": [" ;
        for (std::vector<std::string>::iterator it = hit->second.begin();
                it != hit->second.end(); ++it)
        {
            std::cout << "\"" << *it << "\", ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "}\n packet visualization ends\n\n\n";
}

void  GET::fill_valid_headers()
{
    valid_headers.push_back("GET");
    valid_headers.push_back(" Standard headers :");
    valid_headers.push_back(" A-IM :");
    valid_headers.push_back(" Accept :");
    valid_headers.push_back(" Accept-Charset :");
    valid_headers.push_back(" Accept-Encoding :");
    valid_headers.push_back(" Accept-Language :");
    valid_headers.push_back(" Accept-Datetime :");
    valid_headers.push_back(" Access-Control-Request-Method :");
    valid_headers.push_back(" Access-Control-Request-Headers :");
    valid_headers.push_back(" Authorization :");
    valid_headers.push_back(" Cache-Control :");
    valid_headers.push_back(" Connection :");
    valid_headers.push_back(" Content-Length :");
    valid_headers.push_back(" Content-Type :");
    valid_headers.push_back(" Cookie :");
    valid_headers.push_back(" Date :");
    valid_headers.push_back(" Expect :");
    valid_headers.push_back(" Forwarded :");
    valid_headers.push_back(" From :");
    valid_headers.push_back(" Host :");
    valid_headers.push_back(" If-Match :");
    valid_headers.push_back(" If-Modified-Since :");
    valid_headers.push_back(" If-None-Match :");
    valid_headers.push_back(" If-Range :");
    valid_headers.push_back(" If-Unmodified-Since :");
    valid_headers.push_back(" Max-Forwards :");
    valid_headers.push_back(" Origin :");
    valid_headers.push_back(" Pragma :");
    valid_headers.push_back(" Proxy-Authorization :");
    valid_headers.push_back(" Range :");
    valid_headers.push_back(" Referer :");
    valid_headers.push_back(" TE :");
    valid_headers.push_back(" User-Agent :");
    valid_headers.push_back(" Upgrade :");
    valid_headers.push_back(" Via :");
    valid_headers.push_back(" Warning :");
    valid_headers.push_back(" Non-standard headers :");
    valid_headers.push_back(" Dnt :");
    valid_headers.push_back(" X-Requested-With :");
    valid_headers.push_back(" X-CSRF-Token :");
}