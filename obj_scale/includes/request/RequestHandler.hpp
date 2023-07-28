#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../libs.hpp"
#include <iostream>
#include <sstream>

class RequestHandler {
	/**
	 * @brief It will handel http requests and send back the response
	 * 		   to the client.
	 * 		  It will also parse the request and store it in a Request object.
	 * 		  The Request object will be used to get the request type, header
	 * 		   and body.
	 * 
	 */
private:
    int server_socket;
    int client_socket;
    Request	request;



public:
    RequestHandler(char *request_buffer);
    ~RequestHandler();

	void handleRequest();
	void sendResponse(const std::string& response);

	
	
private:
    // Private helper functions to parse the request

};

#endif 
