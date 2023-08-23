/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayassin <ayassin@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:38:00 by ahsalem           #+#    #+#             */
/*   Updated: 2023/08/22 20:10:51 by ayassin          ###   ########.fr       */
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
        
        void					parse(char *new_buffer);
        void					setBytereadAndReadsock(int bytes, int sock);
        void					visualizeRequestPacket();
		void					purgeParsing();
    private:
        void					fillHeaderRequest(std::string &packet);
        int						checkHeaders();
		void					fillBodyRequest();
		void					flushParsing();
        bool                    earlyBadRequest(std::string packet);
        void					fillResponse();  
        void					fillValidHeaders();
        int						validPacketHeaders();
		std::string				parseChunks(const std::string &s, const std::string &delimiter);
    public:
		int						read_again;
		packet_map				request;
		t_request				full_request;
		std::set<std::string , ciLessLibC>	valid_headers;
		bool					fullheader;
	private:
		std::string				packet;
		int						bytes_read;
		int						read_sock;
		size_t					body_start_pos;
		bool					fullbody;
		bool					ischunked;
		size_t					chunklen;
};      

#endif