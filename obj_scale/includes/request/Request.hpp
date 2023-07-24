#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sstream>

class Request
{
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
};

#endif
