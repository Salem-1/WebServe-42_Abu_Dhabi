/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:22 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/24 15:38:23 by ahsalem          ###   ########.fr       */
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
        Client(int  client_socket, std::map<std::string, std::string> &server);
        Client(const Client &obj2);
        Client &operator= (const Client &obj2);
        ~Client();
        void                                handle_request();
        int                                 get_timeout();
        connection_state                    state;
        int                                 client_socket;
        clock_t                             start_time;
        int                                 connection_duration;
        Receive                             receiver;
        Respond                             responder;
        std::string                         response_packet;
        std::map<std::string, std::string>  server_info;
    private:
};

#endif