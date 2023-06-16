#ifndef RESPOND_HPP
# define RESPOND_HPP

# include "../../includes/libs.hpp"
# include "../GET/GET_response.hpp"
# include "../GET/GET.hpp"

class Respond
{
    public:
        typedef std::map<std::string, std::vector<std::string> >    packet_map;
        typedef std::map<std::string, std::vector<std::string> >    response_pack;

public:
    Respond();
    Respond(int client_socket);
    ~Respond();
    void    respond(packet_map &request);
    int     client_socket;
    void    fill_response(packet_map &request);
    void    visualize_response();
    int     fill_status_code(std::string status_code, std::string message);
    void    send_all();
    response_pack   response;
    std::string     response_packet;
private:
    /* data */

};

#endif