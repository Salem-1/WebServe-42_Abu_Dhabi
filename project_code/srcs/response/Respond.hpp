/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:50 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/12 14:40:50 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPOND_HPP
# define RESPOND_HPP

# include "../../includes/libs.hpp"
# include "../GET/GET_response.hpp"
# include "../GET/GET.hpp"
# include "../DELETE/DELETE.hpp"

class Respond
{
    public:
        typedef std::map<std::string, std::vector<std::string> >    packet_map;
        typedef std::map<std::string, std::vector<std::string> >    response_pack;

public:
    Respond();
    Respond(int client_socket);
    ~Respond();
    void                                flush_response();
    void                                send_all();
    void                                respond(packet_map &request,
                                            conf &servers, std::string port);
    void                                fill_response(packet_map &request,
                                            std::map<std::string, std::string> &server_info);
    void                                fill_errored_response(
                                            std::map<std::string, std::string> &server_info);
    void                                visualize_response();
    int                                 fill_status_code(std::string status_code,
                                            std::string message);
    std::map<std::string, std::string>  get_server_info(packet_map &request,conf &servers, std::string port);
    std::string                         normal_GET_Response(
                                            packet_map &request, std::map<std::string, std::string> &server_info);
    int                                 client_socket;
    response_pack   response;
    std::string     response_packet;
private:
    int     check_poisoned_url(packet_map &request);
    /* data */

};

#endif