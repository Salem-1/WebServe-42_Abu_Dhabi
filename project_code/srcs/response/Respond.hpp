/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Respond.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:50 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/30 07:57:04 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPOND_HPP
# define RESPOND_HPP

# include "../POST/Post.hpp"
# include "../DELETE/DELETE.hpp"
# include "../GET/GET.hpp"

# include "../../includes/libs.hpp"
// # include "../../includes/webserve.hpp"


class Respond
{

    public:
        Respond();
        Respond(int client_socket);
        ~Respond();
        void		flushResponse(void);
        void		sendAll(connection_state &state);
        void		respond(packet_map &request, t_request &body, conf &servers, std::string port);
        void		fillResponse(packet_map &request, t_request &body, stringmap &server_info);
        void		visualizeResponse(void);
        int			fillStatuCode(std::string status_code, std::string message);
        stringmap	getServerInfo(packet_map &request,conf &servers, std::string port);
        std::string	normalGETResponse( packet_map &request, stringmap &server_info);
        stringmap        &nearest_server(conf &servers,
                        std::vector<int> &nominated_servers, std::pair<std::string, std::string> host_port);
		int				client_socket;
        response_packet	response;
        std::string		response_string;
        pthread_t		sendThread;
        bool			sending;
		
	private:
		std::string					isCGI(packet_map &request);
		std::string					responseCGI(packet_map &request, stringmap &server_info, std::string &cgi_path);
        
    private:
        int		checkPoisonedURL(packet_map &request);
        size_t	response_bytes_sent;
    
};
// void* sendAll_thread(void* arg);

#endif