/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receive.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymohamed <ymohamed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:56 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/25 02:21:55 by ymohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECEIVE_HPP
# define RECEIVE_HPP

# include "../../includes/libs.hpp"
# include "Parser.hpp"
class Receive
{
    public:
         typedef  std::map<std::string, std::vector<std::string> >    packet_map;
    public:
        Receive();
        Receive(int read_sock);
        Receive(const Receive &obj2);
        Receive &operator= (const Receive &obj2);
        ~Receive();
        void                read_packet(char *buffer);
        void                receive_all();
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