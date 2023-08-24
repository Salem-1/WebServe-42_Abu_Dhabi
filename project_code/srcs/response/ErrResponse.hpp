/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:21:52 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/24 20:29:30 by ahsalem          ###   ########.fr       */
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
        
        std::string	code(stringmap &server_info, std::string err);        
        void		statusCodes();
        std::string	erroredResponse(std::string err);
        std::string	constructCustomErrPacket(std::string err, std::string &full_file_to_string);
        std::string	    response_str;
        stringmap	    StatusCodes;
    private:
        std::string constructErroPath(std::string server_eror_info, std::string err);
        bool    readFileToString(std::string file_path, 
                                std::string &full_file_to_string);        
        std::string             full_file_to_string;
        std::string             regularErroredResponse(std::string err);
};      
#endif