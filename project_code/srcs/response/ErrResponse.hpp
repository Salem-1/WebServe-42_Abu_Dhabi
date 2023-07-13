/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 17:21:52 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/12 18:41:32 by ahsalem          ###   ########.fr       */
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
        
        void                                status_codes();
        std::string                         get_timebuffer();
        std::string                         errored_response(
                                                std::string err);
        std::string                         construct_custom_err_packet(
                                              std::string err, std::string &full_file_to_string);

        std::string                         response_packet;
        std::map<std::string, std::string>  StatusCodes;
        
};
#endif