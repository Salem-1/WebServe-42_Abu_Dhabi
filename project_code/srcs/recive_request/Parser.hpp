/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:00 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/12 17:08:07 by ayassin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "../../includes/libs.hpp"

class Parser 
{
    public:
        Parser();
        ~Parser();
        Parser(const Parser &obj2);
        Parser &operator= (const Parser &obj2);
        
        void					fillHeaderRequest(std::string packet);
        void					parse(char *new_buffer);
        void					setBytereadAndReadsock(int bytes, int sock);
        void					visualizeRequestPacket();
        int						checkHeaders();
        void					fillGetRequest(std::string packet);
		void					fillBodyRequest();
		void					flushParsing();
		void					purgeParsing();
    private:
        bool                    earlyBadRequest(std::string packet);
        void					fillResponse();  
        void					fillValidHeaders();
        int						validPacketHeaders();
        void					fillRequestLine();
        void					fillPath();
        void					visualizeResponse();
        int						fillStatuCode(std::string status_code, std::string message);
		int						chunkLength(std::string buffer);
		std::string				parseChunks(const std::string &s, const std::string &delimiter);
    public:
		int						read_again;
		std::string				packet;
		int						bytes_read;
		int						read_sock;
		std::string				reponse_packet;
		int						packet_counter;
		size_t					body_start_pos;
		packet_map				request;
		t_request				full_request;
		response_packet			response;
		std::set<std::string , ciLessLibC>	valid_headers;
		std::string				filled_response;
		bool                    is_post;
		bool					fullheader;
	private:
		bool					fullbody;
		bool					ischunked;
		bool					ischunkbody;
		size_t					chunklen;
};      

#endif