/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listner.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:31 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/23 21:01:10 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTNER_HPP
# define LISTNER_HPP
#include "Socket.hpp"

class Listner: public Socket
{
    public:
        Listner(std::string port);
        ~Listner();
        int	getSockfd();
		void bindFD();
}; 
#endif