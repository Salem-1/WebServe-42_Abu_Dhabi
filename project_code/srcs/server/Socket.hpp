/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:25 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/24 15:37:26 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "../../includes/libs.hpp"

class Socket
{
     public:
        typedef std::map<std::string, std::string>  conf; 
    public:
        Socket(conf server);
        Socket( struct  addrinfo *try_another_info);
        virtual ~Socket();
        struct  addrinfo    hints;
        struct  addrinfo    *res;
        struct  addrinfo    *try_addr;
        std::string         servername;
        void                get_my_addinfo(void);
        void                open_socket(struct  addrinfo    *try_addr);
        void                try_open_socket_again( struct  addrinfo *try_another_info);
        void                non_block_sock(void);
        void                error_in_socket(std::string err);
        int                 sockfd;
        int                 try_again;
        conf                server;
    private:
        Socket();


};
#endif