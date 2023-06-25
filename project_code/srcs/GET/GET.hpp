/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:11 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/24 20:08:07 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef GET_HPP
# define GET_HPP
# include "../../includes/libs.hpp"
# include "GET_response.hpp"

class GET
{
    public:
        typedef std::map<std::string, std::vector<std::string> >    packet_map;
        typedef std::map<std::string, std::vector<std::string> >    response_packet;
    public:
        GET(packet_map &request, response_packet &response);
        ~GET();
        void    fill_request_line(packet_map &request);
        void    prepare_get_response(std::map<std::string, std::string> &server_info);
        // GET &operator= (const GET &obj2);
        // GET(const GET &obj2);
        packet_map      &request;
        response_packet &response;
};  
#endif