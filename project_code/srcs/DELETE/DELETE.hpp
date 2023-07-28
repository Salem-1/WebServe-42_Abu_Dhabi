/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:16 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/28 23:41:57 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
# define DELETE_HPP
# include "../../includes/libs.hpp"
#include "../parsing/parsing_lib.hpp"
// # include "../../includes/webserve.hpp"
#include "../response/ErrResponse.hpp"
# include "../response/ErrResponse.hpp"

class DELETE
{
    public:
        DELETE();
        ~DELETE();
        std::string         fillDeleteResponse(response_packet &response,
                                std::map<std::string, std::string> &server_info);
        std::string         deleteResponseFiller(packet_map &request, response_packet &response, std::map<std::string, std::string> &server_info);
        std::string         fillOkResponse(response_packet &response, std::map<std::string, std::string>  &server_info);
        void                fillRequestLine(packet_map &request, response_packet &response);
        bool                sanitizedPath(std::string path, std::map<std::string, std::string>  &server_info);
        void                statusCodes();  
        std::string         constructPath(response_packet &response,
                                std::map<std::string, std::string> &server_info);
        std::string         erroredResponse(std::string error_code, std::string error_message);
        std::string         getTimeBuffer();
        std::string         successfulDeletePacket();
        std::map<std::string, std::string>    StatusCodes;
        
        packet_map                                      request;
        response_packet                                 response;
        ErrResponse                                     err;

};  
#endif