#include "GET_response.hpp"

GET_response::GET_response(response_type res): reponse_check(res)
{};

GET_response::~GET_response()
{}

std::string     GET_response::fill_get_response(std::map<std::string, std::string> &server_info)
{
    if (*(reponse_check["Status-code"].begin()) != "200")
        return (errored_response());
    else if (reponse_check["Path"].size() < 2 && fill_status_code("400", "bad file path format"))
        return (errored_response());
    else
        fill_ok_response(server_info); 
    return (response_packet);
}
void        GET_response::fill_ok_response(std::map<std::string, std::string> &server_info)
{
    /*
    chunking or storing bytes on vector instead of string, or array of strings should happen
        here |
             v  
    */

    response_packet = "";

    // std::string file_path = DEFAULT_PATH + *(++reponse_check["Path"].begin())+ DEFAULT_LOCATION;
    std::string file_path = construct_path(server_info);
    std::cout << "constructed path = " << file_path << std::endl;
    std::ifstream infile(file_path.c_str());
    if (infile.fail())
    {
        std::cout << file_path << std::endl;
        fill_status_code("403", "file not found ya basha!");
        errored_response();
        return ;
    }
    std::stringstream content_stream;
    content_stream  << infile.rdbuf();
    std::string full_file_to_string = content_stream.str();
    //checking file size in bytes
    // infile.seekg(0, std::ios::end);
    // int file_size = infile.tellg();
    response_packet = "HTTP/1.1 " + *(reponse_check["Status-code"].begin()) 
        + " " + *(++reponse_check["Status-code"].begin()) + "\n";
    response_packet += "Server: webserve/1.0\n";
    response_packet += "Date: Wed, 15 Jun 2023 12:00:00 GMT\n";
    response_packet += "Content-Type: text/html; charset=utf-8\n";
    response_packet += "Content-Length: " + std::to_string(full_file_to_string.length()) + "\n\n";
    response_packet += full_file_to_string;
}

std::string    GET_response::construct_path(std::map<std::string, std::string> &server_info)
{

    std::string path = reponse_check["Path"][1];
    if (path == "/")
        return (server_info["/"]);
    else if (path.substr(1, path.length()).find("/") == std::string::npos)
        return (server_info["root"] + path);
    else
    {
        std::string dir_name = path.substr(0, path.length());
        if (server_info.find(dir_name) != server_info.end())
        {
            std::string file_name = path.substr(path.substr(1, path.length()).find("/"), path.length());
            path = server_info[dir_name] + file_name;
        }
    }
    return (path);
}

std::string GET_response::errored_response()
{
    response_packet = "HTTP/1.1 " + *(reponse_check["Status-code"].begin()) 
        + " " + *(++reponse_check["Status-code"].begin()) + "\n";
    response_packet += "Server: webserve/1.0\n";
    response_packet += "Date: Wed, 15 Jun 2023 12:00:00 GMT\n";
    response_packet += "Content-Type: text/html text/javascript test/css; charset=utf-8\n";
    response_packet += "Content-Length: 1050\n\n";
    response_packet += "<!DOCTYPE html>\n";
    response_packet += "<html>\n";
    response_packet += "<head>\n";
    response_packet += "    <title>";
    response_packet += *(reponse_check["Status-code"].begin()) ;
    response_packet += " " + *(++reponse_check["Status-code"].begin()) + "</title>\n";
    response_packet += "</head>\n";
    response_packet += "<body>\n";
    response_packet += "    <h1>error under construction 404 Not Found</h1>\n";
    response_packet += "</body>\n";
    response_packet += "</html>\n";
    response_packet += "<p>\n";
    response_packet += " -------- &&& &&  &amp; &amp; <br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;  && &amp;--&amp;-|&amp; ()|- @, &amp;&amp; <br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;  &amp;--(-&amp;&amp;-||-&amp; -_-)_&amp;-_&amp;<br>\n";
    response_packet += "    &nbsp; &amp;() &amp;--&amp;|()|-&amp;-- '% &amp; ()<br>\n";
    response_packet += "    &nbsp;&amp;_-&amp;_&amp;&amp;_- |&amp; |&amp;&amp;-&amp;__%_-_&amp;&amp; <br>\n";
    response_packet += "    &&&   && &amp; &amp;| &amp;| -&amp; &amp;% ()&amp; -&&<br>\n";
    response_packet += "    &nbsp;()&_---()&amp;-&amp;-|&amp;&amp;-&amp;&amp;--%---()~<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp; &&     -|||<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |||<br>\n";
    response_packet += "    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; , -=-~  .-^- _<br>\n";
    response_packet += "</p>\n";

    return (response_packet);
}


int GET_response::fill_status_code(std::string status_code, std::string message)
{
    reponse_check["Status-code"].clear();
    reponse_check["Status-code"].push_back(status_code);
    reponse_check["Status-code"].push_back(message);
    return (1);
}
