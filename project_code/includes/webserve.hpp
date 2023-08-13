/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserve.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:37:03 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/13 11:53:35 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVE_HPP
# define WEBSERVE_HPP

# include "../srcs/recive_request/Parser.hpp"
# include "../srcs/client/Client.hpp"
# include "../srcs/parsing/parsing_lib.hpp"
# include "../srcs/recive_request/Receive.hpp"
# include "../srcs/response/Respond.hpp"
# include "../srcs/server/Listner.hpp"
# include "../srcs/GET/GET.hpp"
# include "../srcs/GET/GET_response.hpp"
# include "../srcs/response/ErrResponse.hpp"
# include "../srcs/configuration/Config.hpp"
# include "../srcs/configuration/ServerFill.hpp"
# include "libs.hpp"
# include "../srcs/configuration/Config.hpp"
# include "../srcs/POST/Post.hpp"
# include "../srcs/PUT/PUT.hpp"

#include "request/Request.hpp"
#include "request/RequestHandler.hpp"
# include "../srcs/DELETE/DELETE.hpp"

# include "../srcs/server/Kque.hpp"
#endif