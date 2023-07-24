/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:22 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/23 15:06:58 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../../includes/libs.hpp"
# include "../recive_request/Receive.hpp"
# include "../response/Respond.hpp"

class Client
{
    public:
        Client(){};
        Client(int  client_socket, conf servers);
        Client(const Client &obj2);
        Client &operator= (const Client &obj2);
        ~Client();
        void                                handle_request(struct kevent event);
        int                                 get_timeout();
        std::string                         get_port(int client_socket);
        connection_state                    state;
        int                                 client_socket;
        clock_t                             start_time;
        int                                 connection_duration;
        Receive                             receiver;
        Respond                             responder;
        std::string                         response_packet;
        conf                                servers;
        
    private:
};

#endif