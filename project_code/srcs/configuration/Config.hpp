/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:19 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/27 05:57:33 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP
# include "../../includes/libs.hpp"

class Config
{
    public:
        typedef std::vector<std::map<std::string, std::string> > conf; 
    public:
        Config();
        ~Config();
        void    fill_ports();
        conf                        servers;
        std::set<std::string>       ports;
        std::vector<int>            sockets;
};
#endif