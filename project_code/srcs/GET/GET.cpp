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
    if (!valid_packet_headers())
    {
        std::cout << "invalid packet" <<  std::endl;
        return ;
    }
    fill_response();
    visualize_response();
}
int GET::valid_packet_headers()
{
    bool    valid = false;
    for (packet_map::iterator it= request.begin(); it != request.end(); ++it)
    {
        for (std::set<std::string>::iterator vit = valid_headers.begin(); vit != valid_headers.end(); ++vit)
        {
            if (*vit == it->first)
            {
                valid = true;
                break ;
            }
        }
        if (valid)
        {
            valid = false;
            continue ;
        }
        else
        {
            std::cout << "Header " << it->first << " not a valid header" << std::endl;
            return (0);
        }
    }
    return (1);
}
void    GET::fill_response()
{
    response["Status-code"].push_back("200");
    fill_request_line();
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
    valid_headers.insert("GET");
    valid_headers.insert("Standard headers:");
    valid_headers.insert("A-IM:");
    valid_headers.insert("Accept:");
    valid_headers.insert("Accept-Charset:");
    valid_headers.insert("Accept-Encoding:");
    valid_headers.insert("Accept-Language:");
    valid_headers.insert("Accept-Datetime:");
    valid_headers.insert("Access-Control-Request-Method:");
    valid_headers.insert("Access-Control-Request-Headers:");
    valid_headers.insert("Authorization:");
    valid_headers.insert("Cache-Control:");
    valid_headers.insert("Connection:");
    valid_headers.insert("Content-Length:");
    valid_headers.insert("Content-Type:");
    valid_headers.insert("Cookie:");
    valid_headers.insert("Date:");
    valid_headers.insert("Expect:");
    valid_headers.insert("Forwarded:");
    valid_headers.insert("From:");
    valid_headers.insert("Host:");
    valid_headers.insert("If-Match:");
    valid_headers.insert("If-Modified-Since:");
    valid_headers.insert("If-None-Match:");
    valid_headers.insert("If-Range:");
    valid_headers.insert("If-Unmodified-Since:");
    valid_headers.insert("Max-Forwards:");
    valid_headers.insert("Origin:");
    valid_headers.insert("Pragma:");
    valid_headers.insert("Proxy-Authorization:");
    valid_headers.insert("Range:");
    valid_headers.insert("Referer:");
    valid_headers.insert("TE:");
    valid_headers.insert("User-Agent:");
    valid_headers.insert("Upgrade:");
    valid_headers.insert("Via:");
    valid_headers.insert("Warning:");
    valid_headers.insert("Non-standard headers:");
    valid_headers.insert("Dnt:");
    valid_headers.insert("X-Requested-With:");
    valid_headers.insert("X-CSRF-Token:");
    valid_headers.insert("Sec-Fetch-Dest:");
    valid_headers.insert("Sec-Fetch-Dest:");
    valid_headers.insert("Sec-Fetch-Mode:");
    valid_headers.insert("Sec-Fetch-Site:");
    valid_headers.insert("Sec-Fetch-User:");
    valid_headers.insert("Upgrade-Insecure-Requests:");
    valid_headers.insert("sec-ch-ua-mobile:");
    valid_headers.insert("sec-ch-ua-platform:");
    valid_headers.insert("sec-ch-ua:");
}