#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sstream>

class Request
{
	/**
	 * @brief It will handel http requests 
	 * 			it will also parse the request
	 * 			it should have childs for each request type
	 * 				GET, POST, PUT, DELETE
	 * 			it will have general functions for all request types
	 */
public:
    Request(const std::string& request);

    const std::string& getRequest() const;
    const std::string& getRequestType() const;
    const std::string& getRequestHeader() const;
    const std::string& getRequestBody() const;
	bool isValid() const;



private:
    std::string fullRequest;
    std::string requestHeader;
    std::string requestBody;
    std::string requestType;
	bool requestValid;

	bool parseRequest(const std::string& request);
    bool parseRequestLine(const std::string& requestLine);
	bool parseRequestHeader(const std::string& requestHeader);
	bool parseRequestBody(const std::string& requestBody);
	
};

#endif
