#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../libs.hpp"
#include <iostream>
#include <sstream>

class RequestHandler {
private:
    int server_socket;
    int client_socket;
    Request	request;

public:
    RequestHandler(int server_socket);
    ~RequestHandler();

    void acceptClient();
    
    const std::string& getRequestType() const;
    const std::string& getRequestHeader() const;
    const std::string& getRequestBody() const;

    // You can add other public methods to handle request processing
    void processRequest();

private:
    // Private helper functions to parse the request

};

#endif 
