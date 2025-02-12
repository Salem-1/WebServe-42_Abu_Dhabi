/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:56 by ahsalem           #+#    #+#             */
/*   Updated: 2023/09/04 21:02:30 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECEIVE_HPP
# define RECEIVE_HPP

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
};
#endif