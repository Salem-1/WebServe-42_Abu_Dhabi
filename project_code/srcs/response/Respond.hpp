/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:50 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/27 20:44:39 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPOND_HPP
# define RESPOND_HPP

# include "../../includes/libs.hpp"
# include "../GET/GET_response.hpp"
# include "../GET/GET.hpp"
# include "../DELETE/DELETE.hpp"
# include "../POST/Post.hpp"


class Respond
{
    public:
        typedef std::map<std::string, std::vector<std::string> >    packet_map;
        typedef std::map<std::string, std::vector<std::string> >    response_pack;

    public:
        Respond();
        Respond(int client_socket);
        ~Respond();
        void                                flush_response(void);
         void                                send_all(connection_state &state);
        void                                respond(packet_map &request,
                                                conf &servers, std::string port);
        void                                fill_response(packet_map &request,
                                                std::map<std::string, std::string> &server_info);
        void                                visualize_response(void);
        int                                 fill_status_code(std::string status_code,
                                                std::string message);
        std::map<std::string, std::string>  get_server_info(packet_map &request,conf &servers, std::string port);
        std::string                         normal_GET_Response(
                                                packet_map &request, std::map<std::string, std::string> &server_info);
        int                                 client_socket;
        response_pack                       response;
        std::string                         response_packet;
        pthread_t                           sendThread;
        bool                                sending;
	private:
		std::string					isCGI(packet_map &request);
		std::string					responseCGI(packet_map &request, stringmap &server_info, std::string &cgi_path);
        
    private:
        int     check_poisoned_url(packet_map &request);
        size_t     response_bytes_sent;
    
};
// void* send_all_thread(void* arg);

#endif