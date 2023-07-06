/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listner.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:31 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/27 05:40:26 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTNER_HPP
# define LISTNER_HPP
#include "Socket.hpp"

class Listner: public Socket
{
     public:
        typedef std::map<std::string, std::string>  conf; 
    public:
        Listner(std::string port);
        ~Listner();
        int get_sockfd();
}; 
#endif