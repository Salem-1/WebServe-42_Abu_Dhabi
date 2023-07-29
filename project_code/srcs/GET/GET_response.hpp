/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET_response.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:14 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/29 17:40:11 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef GET_RESPONSE_HPP
# define GET_RESPONSE_HPP
# include "../../includes/libs.hpp"
# include "../response/ErrResponse.hpp"

class GET_response
{
    public:
            typedef std::map<std::string, std::vector<std::string> >    response_type;

    public:
        GET_response(response_type &res);
        ~GET_response();
        std::string     fill_get_response(std::map<std::string, std::string> &server_info);
        std::string     errored_response();
        std::string     construct_path(std::map<std::string, std::string> &server_info);
        void            fill_ok_response(std::map<std::string, std::string> &server_info);
        bool            sanitized_path(std::string path);
        bool            fill_bad_path(std::map<std::string,
                                std::string> &server_info);
        void            filling_response_packet(std::string &full_file_to_string, std::string file_path);
        void            construct_dir_response(std::vector<std::string> &ls,
                                std::string &full_file_to_string);
        std::string     getContentType(std::string file_path);
        std::string     response_packet;
        ErrResponse     err;
        response_type   reponse_check;
};
# endif