/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET_response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahsalem <ahsalem@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 16:33:09 by ahsalem           #+#    #+#             */
/*   Updated: 2023/07/31 14:38:29 by ahsalem          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GET_response.hpp"

GET_response::GET_response(response_type &res): reponse_check(res)
{
};

GET_response::~GET_response()
{}

std::string     GET_response::fillGetResponse(stringmap &server_info)
{
    if (*(reponse_check["Status-code"].begin()) != "200")
        return (err.code(server_info, reponse_check["Status-code"][0]));
    else if (reponse_check["Path"].size() < 2 && fillStatuCode(reponse_check , "400", "bad file path format"))
        return (err.code(server_info, reponse_check["Status-code"][0]));
    else
        fillOkResponse(server_info); 
    return (response_packet);
}
bool    GET_response::redirectedPacket(stringmap &server_info)
{
    if (server_info.find("Redirections") == server_info.end() || server_info["Redirections"].empty())
        return (false);
    std::vector<std::string> redirections_strs = split(server_info["Redirections"], ",");
    if (redirections_strs.empty())
        return (false);
    std::vector<std::string> tmp;
    for (size_t i = 0; i < redirections_strs.size(); ++i)
    {
        tmp = split(redirections_strs[i], " ");
        if (tmp.size() != 3)
            return (false);
        if (tmp[0] == reponse_check["Path"][1])
        {
            redirection["old_path"] = tmp[0];
            redirection["new_path"] = tmp[1];
            redirection["Status-code"] = tmp[2];
            visualize_string_map(redirection);
            return (true);
        }
    }
    return (false);
}

void   GET_response::fillRedirectedPacket(void)
{
    response_packet = "HTTP/1.1 ";
    response_packet += redirection["Status-code"] + " ";
    response_packet +=  err.StatusCodes[redirection["Status-code"]] + " \r\n";
    response_packet += "Server: webserve/1.0\r\n";
    response_packet += "Location: " + redirection["new_path"] + "\r\n";
    response_packet += "Date: ";
    response_packet += err.getTimeBuffer();
    response_packet += "Connection: close\r\n";
    response_packet += "Content-Length: 0\r\n";
    response_packet += "Content-Type: text/html; charset=UTF-8 \r\n\r\n";
   
}
void    GET_response::fillOkResponse(stringmap &server_info)
{
    response_packet = "";
    if(redirectedPacket(server_info))
    {
        fillRedirectedPacket();
        // exit(0);
        return ;
    }
    std::string file_path = constructPath(server_info);
    std::cout << BOLDMAGENTA << "requested file path = "
    		<< RESET << file_path << std::endl <<RESET;
    if (!sanitizedPath(file_path)&& fillBadPath(server_info))
        return ;
    std::cout << MAGENTA << "constructed path = " << file_path << std::endl << RESET;
    DIR *dir;
    std::string full_file_to_string;
    if ((dir  = opendir(file_path.c_str())) != NULL)
    {
        if (server_info.find("autoindex") != server_info.end())
        {
            if (server_info["autoindex"] == "off")
            {
                response_packet = err.code(server_info, "403");
                return ;
            }
        }
        struct dirent *files;
        std::vector<std::string> ls;
        ls.push_back(file_path);
        while ((files = readdir(dir)) != NULL)
            ls.push_back(files->d_name);
        constructDirResponse(ls, full_file_to_string);
    }
    else
    {
        std::ifstream infile(file_path.c_str());
        if (infile.fail() && fillBadPath(server_info))
            return ;
        std::stringstream content_stream;
        content_stream  << infile.rdbuf();
        full_file_to_string = content_stream.str();
    }
    fillingResponsePacket(full_file_to_string, file_path);
}

void    GET_response::constructDirResponse(std::vector<std::string> &ls,
    std::string &full_file_to_string)
{
    std::string file_name = ls[0].substr(ls[0].rfind("/"), ls[0].length());
    full_file_to_string = "<!DOCTYPE html>";
    full_file_to_string += "<html>";
    full_file_to_string += "<head>";
    full_file_to_string += "    <title>Index of" +  file_name + "</title>";
    full_file_to_string += "</head>";
    full_file_to_string += "<body>";
    full_file_to_string += "  <h1>Index of " + file_name +"</h1>";
    full_file_to_string += "<ul>";
    full_file_to_string += "    <ul>";
    for (std::vector<std::string>::iterator it = ++ls.begin();
        it != ls.end(); ++it) 
        full_file_to_string += "        <li>" + *it +"</li>";
    full_file_to_string += "    </ul>";
    full_file_to_string += "</body>";
    full_file_to_string += "</html>";
}
void     GET_response::fillingResponsePacket(std::string &full_file_to_string,  std::string file_path)
{
    response_packet = "HTTP/1.1 200 OK \r\n";
    response_packet += "Server: webserve/1.0\r\n";
    response_packet += "Date: ";
    response_packet += err.getTimeBuffer();
    response_packet += "Content-Type: " + getContentType(file_path) +" \r\n";
	std::stringstream ss;
	ss << full_file_to_string.length();
    response_packet += "Content-Length: " + ss.str() + "\r\n\r\n";
    response_packet += full_file_to_string; 
}


bool    GET_response::fillBadPath(stringmap &server_info)
{
        fillStatuCode(reponse_check , "404", "file not found ya basha!");
        response_packet = err.code(server_info, reponse_check["Status-code"][0]);
        vis_str(response_packet, "inside GET_response has large response not gonna visualize\n");
        return (true);

}
std::string    GET_response::constructPath(stringmap &server_info)
{

    std::string path = reponse_check["Path"][1];
    std::cout << "Given path = " << path << std::endl;
    std::cout << MAGENTA << "construcing path = " << path << std::endl << RESET ;
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
    std::cout << MAGENTA << "dir == " << dir << " path = " << path << std::endl << RESET;
    // /images/ case
    if (path[path.length() - 1] == '/' && dir.length() == path.length() - 1)
    {
        std::cout << "yes it's only dir" << std::endl;
        if (server_info.find(dir) != server_info.end())
            return (server_info[dir + " index"]);
    }
    // images/cat.jpeg
    std::string rest_of_path = path.substr(dir.length() + 1, path.length());
    std::cout << MAGENTA << "rest of path = " << rest_of_path << std::endl << RESET;
    if (server_info.find(dir) != server_info.end())
        return (server_info[dir] + rest_of_path);
    return (server_info["root"] + path);
}


bool GET_response::sanitizedPath(std::string path)
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
    return (true);
}

void    get_mime(std::map<std::string, std::string> &mimes)
{
    std::string mimes_str = "text/plain txt\n";
    mimes_str += "text/html html\n";
    mimes_str += "text/html htm\n";
    mimes_str += "text/css css\n";
    mimes_str += "text/javascript js\n";
    mimes_str += "application/json json\n";
    mimes_str += "application/xml xml\n";
    mimes_str += "application/pdf pdf\n";
    mimes_str += "application/zip zip\n";
    mimes_str += "application/gzip gz\n";
    mimes_str += "application/msword doc\n";
    mimes_str += "application/vnd.ms-excel xls\n";
    mimes_str += "application/vnd.ms-powerpoint ppt\n";
    mimes_str += "application/vnd.openxmlformats-officedocument.wordprocessingml.document docx\n";
    mimes_str += "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet xlsx\n";
    mimes_str += "application/vnd.openxmlformats-officedocument.presentationml.presentation pptx\n";
    mimes_str += "image/jpeg jpeg, jpg\n";
    mimes_str += "image/png png\n";
    mimes_str += "image/gif gif\n";
    mimes_str += "image/svg+xml svg\n";
    mimes_str += "audio/mpeg mp3\n";
    mimes_str += "audio/wav wav\n";
    mimes_str += "audio/ogg ogg\n";
    mimes_str += "video/mp4 mp4\n";
    mimes_str += "video/mpeg mpeg\n";
    mimes_str += "video/quicktime mov\n";
    mimes_str += "video/x-msvideo avi\n";
    mimes_str += "application/octet-stream bin\n";
    std::vector<std::string> mime_lines = split(mimes_str, "\n");
    for (std::vector<std::string>::iterator it = mime_lines.begin();
        it != mime_lines.end(); it++)
    {
        std::vector<std::string> tmp = split(*it, " ");
        if (tmp.size() != 2)
            continue ;
        mimes[tmp[1]] = tmp[0]; 
    }
    visualize_string_map(mimes);
    // mimes += "application/x-www-form-urlencoded (No specific file extension)\n";
    // mimes += "multipart/form-data (No specific file extension)\n";
}

std::string GET_response::getContentType(std::string file_path)
{
    size_t  file_location = file_path.rfind("/");
    if (file_location == std::string::npos || file_location == file_path.length() - 1)
        return ("text/html");
    std::string file_name = file_path.substr(file_location + 1, file_path.length());
    std::map<std::string, std::string>  mimes;
    size_t dot_location = file_name.rfind(".");
    if (dot_location == std::string::npos 
        || dot_location == file_name.length() - 1)
        return ("text/html");
    std::string file_extension = file_name.substr(dot_location + 1, file_name.length() - 1);
    get_mime(mimes);
        std::cout << "\n\n\nfile extension is " << file_extension << std::endl;
    if (mimes.find(file_extension) != mimes.end())
    {
        std::cout << "encoding is " << mimes[file_extension] << std::endl;
        return (mimes[file_extension]);
    }

    return ("text/html");
}