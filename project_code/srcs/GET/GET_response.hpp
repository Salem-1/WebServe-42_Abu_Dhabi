/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET_response.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:14 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/29 17:40:11 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef GET_RESPONSE_HPP
# define GET_RESPONSE_HPP
# include "../../includes/libs.hpp"
# include "../parsing/parsing_lib.hpp"
// # include "../../includes/webserve.hpp"
# include "../response/ErrResponse.hpp"

class GET_response
{
    public:
        GET_response(response_type &res);
        ~GET_response();
        response_type   reponse_check;
        std::string	fillGetResponse(stringmap &server_info);
        std::string	erroredResponse(); // repeated code
        std::string	constructPath(stringmap &server_info, std::string> &server_info); // repeated code
        void		fillOkResponse(stringmap &server_info); // repeated code
        bool		sanitizedPath(std::string path); // repeated code
        bool		fillBadPath(std::map<std::string, std::string> &server_info);
        void		fillingResponsePacket(std::string &full_file_to_string, std::string file_path);
        void		constructDirResponse(std::vector<std::string> &ls, std::string &full_file_to_string);
       
		std::string     getContentType(std::string file_path);
		std::string		response_packet;
        ErrResponse		err;
        response_type	reponse_check;
};
# endif