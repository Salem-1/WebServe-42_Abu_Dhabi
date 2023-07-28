/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:56 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/28 23:54:18 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECEIVE_HPP
# define RECEIVE_HPP

// # include "../../includes/libs.hpp"
// # include "../../includes/webserve.hpp"
# include "Parser.hpp"
// # include "Parser.hpp"
class Receive
{
    public:
        Receive();
        Receive(int read_sock);
        Receive(const Receive &obj2);
        Receive &operator= (const Receive &obj2);
        ~Receive();
        void                readPacket(char *buffer);
        void                receiveAll();
        int                 read_sock;
        int                 bytes_read;
        char                buffer[BUFFER_SIZE];
        connection_state    state;
        Parser              parser;
		// ?? it is already typedefed in the class
        std::map<std::string, std::vector<std::string> >          &get_request_packet();
    private:
};
#endif