/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET_response.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:14 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/12 18:07:57 by ahsalem          ###   ########.fr       */
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
        std::string     response_packet;
        ErrResponse     err;
        response_type   reponse_check;
};
# endif