#include "DELETE.hpp"


DELETE::DELETE()
{

}
DELETE::~DELETE()
{

}

std::string    DELETE::deleteResponseFiller(packet_map &request, response_packet &response, stringmap &server_info)

{

    fillRequestLine(request, response);
    fillPath(request, response, "DELETE");
    return (fillDeleteResponse(response, server_info));
}

void    DELETE::fillRequestLine(packet_map &request, response_packet &response)
{
    if ((request.find("POST") != request.end() || request.find("GET") != request.end())
        && fillStatuCode(response, "400", "Invalid multiple methods inside GET"))
        return ;
    if ((request["DELETE"][0].length() + request["DELETE"][1].length()) > HEADER_MAX_LENGTH
            && fillStatuCode(response, "414", "URI Too Long"))
        return ;
    if (!(request["DELETE"][1] == "HTTP/1.1")
        && (fillStatuCode(response, "505", "version not   supported")))
        return ;
    if ((request["DELETE"].size() != 2) 
        && (fillStatuCode(response, "400", "DELETE vec has wrong number items bad request")))
        return ;
    else
        response["HTTP version"].push_back(request["DELETE"][1]);
    response["method"].push_back("DELETE");

}

std::string     DELETE::fillDeleteResponse(response_packet &response,stringmap  &server_info)
{
    if (response["Status-code"][0] != "200")
        return (err.code(server_info, response["Status-code"][0]));
    else if (response["Path"].size() < 2 && fillStatuCode(response , "400", "bad file path format"))
        return (err.code(server_info, response["Status-code"][0]));
    else
        return (fillOkResponse(response, server_info)); 
}


std::string DELETE::fillOkResponse(response_packet &response, stringmap &server_info)
{
    //all the heavy lifting is here inshalla
    std::string path = constructPath(response, server_info);
    if (!sanitizedPath(path, server_info))
        return (err.code(server_info, "403"));
    if (access(path.c_str(), F_OK) != 0)
    {
        return (err.code(server_info, "404"));
    }
    if (std::remove(path.c_str()) != 0)
        return (err.code(server_info, "404"));
    return (successfulDeletePacket());
}

std::string DELETE::successfulDeletePacket()
{
    std::string success_delete = "HTTP/1.1 204 No Content\n";
    success_delete += "Date: " + getTimeBuffer() +"\n";
    success_delete += "Server: Webserv 1.0\n";
    return (success_delete);
}
bool DELETE::sanitizedPath(std::string path, stringmap &server_info)
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
            return (false);
        }
    }
    if (path.find(server_info["DELETE path"]) == std::string::npos)
    {
        return (false);
    }
    return (true);
}

std::string    DELETE::constructPath(response_packet &response , stringmap &server_info)
{

    std::string path = server_info["DELETE path"] + response["Path"][1];
    if (path == "/")
        return (server_info[path]);
    if (std::count(path.begin(), path.end(), '/') < 2)
    {
        if (server_info.find(path) != server_info.end())
            return (server_info[path + " index"]);
        else
            return (server_info["root"] + path);
    }
    std::string dir = path.substr(0, path.substr(1, path.length()).find("/") + 1);
    // /images/ case
    if (path[path.length() - 1] == '/' && dir.length() == path.length() - 1)
    {
        if (server_info.find(dir) != server_info.end())
            return (server_info[dir + " index"]);
    }
    // images/cat.jpeg
    std::string rest_of_path = path.substr(dir.length() + 1, path.length());
    if (server_info.find(dir) != server_info.end())
        return (server_info[dir] + rest_of_path);
    return (server_info["root"] + path);
}
