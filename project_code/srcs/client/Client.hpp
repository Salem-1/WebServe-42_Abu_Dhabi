/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:22 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/23 07:17:07 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../response/Respond.hpp"
# include "../../includes/libs.hpp"
# include "../recive_request/Receive.hpp"

class Client
{
    public:
        Client(){};
        Client(int  client_socket, conf servers);
        Client(const Client &obj2);
        Client &operator= (const Client &obj2);
        ~Client();
        
		void				handleRequest(struct kevent &event);
        int					getTimeout();
        std::string			getPort(int client_socket);
		connection_state	state;
        int					client_socket;
        clock_t				start_time;
        int					connection_duration;
        Receive				receiver;
        Respond				responder;
        std::string			response_string;
        conf				servers;
};

#endif