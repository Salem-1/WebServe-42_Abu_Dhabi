/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:25 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/29 11:08:08 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "../../includes/libs.hpp"

class Socket
{
    public:
        Socket(std::string port);
        Socket( struct  addrinfo *try_another_info);
        virtual ~Socket();
        void	getMyAddinfo(void);
        void	openSocket(struct  addrinfo    *try_addr);
        void	tryOpenSocketAgain( struct  addrinfo *try_another_info);
        void	nonBlockSock(void);
        void	errorInSocket(std::string err);
		
        std::string		port;
        int				sockfd;
        struct addrinfo	hints;
        struct addrinfo	*res;
        struct addrinfo	*try_addr;
        int				try_again;
        // conf                server;
    private:
        Socket();


};
#endif