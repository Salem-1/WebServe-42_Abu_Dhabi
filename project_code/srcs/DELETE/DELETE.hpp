/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:16 by ahsalem           #+#    #+#             */
/*   Updated: 2023/06/25 16:59:30 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
# define DELETE_HPP
# include "../../includes/libs.hpp"


class DELETE
{
    public:
        typedef std::map<std::string, std::vector<std::string> >    packet_map;
        typedef std::map<std::string, std::vector<std::string> >    response_packet;
    public:
        DELETE();
        ~DELETE();
        std::string         fill_delete_response(response_packet &response,
                                std::map<std::string, std::string> &server_info);
        std::string         delete_response_filler(packet_map &request, response_packet &response, std::map<std::string, std::string> &server_info);
        std::string         fill_ok_response(response_packet &response, std::map<std::string, std::string>  &server_info);
        void                fill_request_line(packet_map &request, response_packet &response);
        bool                sanitized_path(std::string path);
        void                status_codes();  
        std::string         construct_path(response_packet &response,
                                std::map<std::string, std::string> &server_info);
        std::string         errored_response(std::string error_code, std::string error_message);
        std::string         get_timebuffer();
        std::string         successful_delete_packet();
        std::unordered_map<std::string, std::string>    StatusCodes;
        
        packet_map                                      request;
        response_packet                                 response;

};  
#endif