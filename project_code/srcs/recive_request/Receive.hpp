/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:56 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/12 08:54:20 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECEIVE_HPP
# define RECEIVE_HPP

// # include "../../includes/libs.hpp"
// # include "../../includes/webserve.hpp"
# include "Parser.hpp"
class Receive
{
    public:
        Receive();
        Receive(int read_sock);
        Receive(const Receive &obj2);
        Receive &operator= (const Receive &obj2);
        ~Receive();
        void				readPacket(char *buffer);
        void				receiveAll();
        void                flushReceive();
        int					read_sock;
        int					bytes_read;
        char				buffer[READ_BUFFER_SIZE];
        connection_state	state;
        Parser				parser;
		// ?? it is already typedefed in the class
        packet_map			&get_request_packet();
    private:
};
#endif