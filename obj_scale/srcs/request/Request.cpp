#include "../../includes/request/Request.hpp"

Request::Request(const std::string& request) : fullRequest(request)
{
    // Parse the request to extract the request type, header, and body
    std::istringstream requestStream(request);
    std::string requestLine;

    if (std::getline(requestStream, requestLine)) {
        // Assuming the request line has the format: "METHOD URI HTTP/1.1"
        std::istringstream lineStream(requestLine);
        lineStream >> requestType;

        // Read the request header and body
        std::ostringstream headerStream, bodyStream;
        bool headerDone = false;
        std::string line;

        while (std::getline(requestStream, line)) {
            if (line.empty()) {
                headerDone = true;
                continue;
            }

            if (!headerDone) {
                headerStream << line << '\n';
            } else {
                bodyStream << line << '\n';
            }
        }

        requestHeader = headerStream.str();
        requestBody = bodyStream.str();
    }
}

const std::string& Request::getRequest() const
{
    return fullRequest;
}

const std::string& Request::getRequestType() const
{
    return requestType;
}

const std::string& Request::getRequestHeader() const
{
    return requestHeader;
}

const std::string& Request::getRequestBody() const
{
    return requestBody;
}

bool Request::isValid() const
{
    return requestValid;
}

bool Request::parseRequest(const std::string& request)
{
    // Split the request into lines to parse the request line and header
    std::istringstream requestStream(request);
    std::string requestLine;
    if (std::getline(requestStream, requestLine)) {
        return parseRequestLine(requestLine);
    }

    return false;
}

bool Request::parseRequestLine(const std::string& requestLine)
{
    // Assuming the request line has the format: "METHOD URI HTTP/1.1"
    std::istringstream lineStream(requestLine);
    std::string method, uri, httpVersion;

    if (lineStream >> method >> uri >> httpVersion) {
        if (httpVersion == "HTTP/1.1") {
            // Set the request type and header
            requestType = method;
            requestHeader = requestLine;

            // Read the request body if present
            std::string line;
            while (std::getline(lineStream, line) && !line.empty()) {
                requestHeader += "\n" + line;
            }

            // Read the request body
            std::ostringstream bodyStream;
            bodyStream << lineStream.rdbuf();
            requestBody = bodyStream.str();

            return true;
        } else {
            std::cout << "Invalid HTTP version: " << httpVersion << std::endl;
        }
    } else {
        std::cout << "Invalid request line: " << requestLine << std::endl;
    }

    return false;
}
