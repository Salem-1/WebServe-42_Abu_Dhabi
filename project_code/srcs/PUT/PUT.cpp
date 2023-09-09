#include "PUT.hpp"

PUT::PUT(packet_map &request_map, t_request &full_request, stringmap &server_info):
    _request_map(request_map), _request(full_request), _server_info(server_info)
{
	//fill the default response as error
	this->_response = "HTTP/1.1 500 Internal Server Error\r\n"
                       "Content-Type: text/html\r\n"
                       "Content-Length: 223\r\n\r\n"
                       "<!DOCTYPE html>\n"
                       "<html>\n"
                       "<head>\n"
                       "    <title>Internal Server Error</title>\n"
                       "</head>\n"
                       "<body>\n"
                       "    <h1>500 Internal Server Error</h1>\n"
                       "    <p>The server encountered an unexpected condition that prevented it from fulfilling the request.</p>\n"
                       "</body>\n"
                       "</html>\n";
}

PUT::PUT(packet_map &request_map, t_request &full_request, stringmap &server_info, response_packet &response):
    _request_map(request_map), _request(full_request), _server_info(server_info) , _response_pack(response)
{
	//fill the default response as error
	this->_response = "HTTP/1.1 500 Internal Server Error\r\n"
                       "Content-Type: text/html\r\n"
                       "Content-Length: 223\r\n\r\n"
                       "<!DOCTYPE html>\n"
                       "<html>\n"
                       "<head>\n"
                       "    <title>Internal Server Error</title>\n"
                       "</head>\n"
                       "<body>\n"
                       "    <h1>500 Internal Server Error</h1>\n"
                       "    <p>The server encountered an unexpected condition that prevented it from fulfilling the request.</p>\n"
                       "</body>\n"
                       "</html>\n";
}

PUT::~PUT()
{

}

void	PUT::printPUTHeader()
{
	std::cout << BOLDYELLOW << "\nPUT request header is: " << std::endl << RESET;
	std::cout << BOLDYELLOW << this->_request.header << std::endl << RESET;
}

void	PUT::printPUTBody()
{
	std::cout << BOLDYELLOW << "\nPUT request body is: " << std::endl << RESET;
	std::cout << BOLDYELLOW << this->_request.body << std::endl << RESET;
}





void PUT::handlePUT()
{
    std::string path = constructPath(_server_info);
    if (!sanitizedPath(path))
        return ;
    putBody(path);
	return ;


}
bool    PUT::putBody(std::string path)
{
    std::ofstream outfile(path.c_str(), std::ios::out | std::ios::trunc);
    if (outfile.fail())
        throw(std::runtime_error("500"));
    outfile << _request.body;
    outfile.close();
    fillOkResponse();
    std::cout << _response << std::endl;
    return (true);
}

std::string    PUT::fillOkResponse()
{
    this->_response = "HTTP/1.1 201 Created\r\n"
	            "Server: Phantoms\r\n"
                "Date: "
                + getTimeBuffer()
		        + "\r\n";
    return (this->_response);

}

std::string    PUT::constructPath(stringmap &server_info)
{
    std::string path = _response_pack["Path"][1];
    std::string dir;
    if (path == "/")
        return (server_info[path]);
    if (std::count(path.begin(), path.end(), '/') == 2  && path[path.length() - 1] == '/')
        path = path.substr(0, path.length() - 1);
    if (std::count(path.begin(), path.end(), '/') < 2 )
    {
        if (inMap(server_info, path))
        {
            if (server_info.find(path + " index") != server_info.end())
                return (server_info[path] + server_info[path + " index"]);
            else
                return (server_info[path]);
        }
       
    }
    else
        dir = path.substr(0, path.substr(1, path.length()).find("/") + 1);

    if (path[path.length() - 1] == '/' && dir.length() == path.length() - 1)
    {
        if (server_info.find(dir) != server_info.end())
            return (server_info[dir + " index"]);
    }
    std::string rest_of_path = path.substr(dir.length() + 1, path.length());

    
    //the error is here

    if (server_info.find(dir) != server_info.end())
    {
        return (server_info[dir] + rest_of_path);
    }

    return (server_info["root"] + path);
}


bool PUT::sanitizedPath(std::string path)
{
    std::vector<std::string> malicous_inputs;
    malicous_inputs.push_back("..");
    malicous_inputs.push_back("*");
    malicous_inputs.push_back("!");
    malicous_inputs.push_back("~");
    malicous_inputs.push_back("//");
    for (std::vector<std::string>::iterator it = malicous_inputs.begin();  it != malicous_inputs.end(); ++it)
    {
        if (path.find(*it) != std::string::npos)
        {
            std::cout << "malicous part in path = <" << *it << ">" << std::endl;
            throw(std::runtime_error("400"));
            return (false);
        }
    }
    return (true);
}
