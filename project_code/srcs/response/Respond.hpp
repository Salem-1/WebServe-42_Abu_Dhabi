

#ifndef RESPOND_HPP
# define RESPOND_HPP

# include "../POST/Post.hpp"
# include "../PUT/PUT.hpp"
# include "../DELETE/DELETE.hpp"
# include "../GET/GET.hpp"

# include "../../includes/libs.hpp"
// # include "../../includes/webserve.hpp"


class Respond
{

    public:
        Respond();
        Respond(int client_socket);
        ~Respond();
        void		    flushResponse(void);
        void		    sendAll(connection_state &state);
        void		    respond(packet_map &request, t_request &body, conf &servers, std::string port);
        void		    fillResponse(packet_map &request, t_request &body, stringmap &server_info);
        void		    visualizeResponse(void);
        int			    fillStatuCode(std::string status_code, std::string message);
        stringmap	    getServerInfo(packet_map &request,conf &servers, std::string port);
        std::string	    normalGETResponse( packet_map &request, stringmap &server_info);
        bool            isSupportedMethod(std::string method, std::vector<std::string> &supported_methods);
        void            fillSupportedMethods(
                            std::vector<std::string> &supported_methods, stringmap &server_info
                            , std::string method, packet_map& request);
	private:
		static void		closePipe(int *fd);
		std::string	    isCGI(packet_map &request);
		std::string	    responseCGI(packet_map &request, stringmap &server_info, std::string &cgi_path, ErrResponse &err, std::string &body);
		std::string	    execute(stringmap &server_info, std::string path, std::string &args, ErrResponse &err);
		std::string 	postExecute(stringmap &server_info, std::string path, std::string &args, ErrResponse &err, std::string &body);
        int		        checkPoisonedURL(packet_map &request);
        std::string     fillRequestedHostName(packet_map &request, std::string &port, unsigned long &j);
    public:
		int				client_socket;
        response_packet	response;
        std::string		response_string;
        pthread_t		sendThread;
        bool			sending;
		// ErrResponse     err;

    private:
        size_t	                    response_bytes_sent;
        std::vector<int>            nominated_servers;
        std::vector<std::string>    server_names;
        bool            isput;
    
};
// void* sendAll_thread(void* arg);

#endif