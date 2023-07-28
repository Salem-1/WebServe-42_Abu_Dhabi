/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:11 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/28 23:42:35 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef GET_HPP
# define GET_HPP
# include "../../includes/libs.hpp"
// # include "../../includes/webserve.hpp"
# include "GET_response.hpp"

class GET
{
    public:
        GET(packet_map &request, response_packet &response);
        ~GET();
        void    fillRequestLine(packet_map &request); // repeated code
        void    prepareGetResponse(std::map<std::string, std::string> &server_info);
        // GET &operator= (const GET &obj2);
        // GET(const GET &obj2);
        packet_map      &request;
        response_packet &response;
};  
#endif