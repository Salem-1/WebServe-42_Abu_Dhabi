/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:00 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/24 17:24:32 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "../../includes/libs.hpp"
# include "../GET/GET.hpp" 
class Parser 
{
    public:
         typedef std::map<std::string, std::vector<std::string> >    packet_map;
        typedef std::map<std::string, std::vector<std::string> >    response_pack;
    public:
        Parser();
        ~Parser();
        Parser(const Parser &obj2);
        Parser &operator= (const Parser &obj2);
        
        void                    fill_header_request(std::string packet);
        void                    parse(char *new_buffer);
        void                    set_byteread_and_readsock(int bytes, int sock);
        void                    visualize_request_packet();
        int                    check_headers();
        void                    fill_get_request(std::string packet);
    
    private:
        bool                    early_bad_request(std::string packet);
        void                    fill_response();  
        void                    fill_valid_headers();
        int                     valid_packet_headers();
        void                    fill_request_line();
        void                    fill_path();
        void                    visualize_response();
        int                     fill_status_code(std::string status_code, std::string message);
        void                    flush_parsing();
    public:
        int                     read_again;
        std::string             packet;
        int                     bytes_read;
        int                     read_sock;
        std::string             reponse_packet;
        int                     packet_counter;
        int                     i;
        packet_map              request;
        response_pack           response;
        std::set<std::string>   valid_headers;
        std::string             filled_response;
};

#endif