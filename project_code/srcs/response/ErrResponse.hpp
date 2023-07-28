/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:21:52 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/28 23:53:42 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRRESPONSE_HPP
# define ERRRESPONSE_HPP
# include "../../includes/libs.hpp"

class ErrResponse
{
    public:
        ErrResponse();
        ~ErrResponse();
        std::string code(
                std::map<std::string, std::string> &server_info, std::string err);        
        
        void                                statusCodes();
        std::string                         getTimeBuffer();
        std::string                         erroredResponse(
                                                std::string err);
        std::string                         constructCustomErrPacket(
                                              std::string err, std::string &full_file_to_string);

        std::string                         response_packet;
        std::map<std::string, std::string>  StatusCodes;
        
};
#endif