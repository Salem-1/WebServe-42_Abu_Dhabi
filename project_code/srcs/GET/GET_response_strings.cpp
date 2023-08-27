#include "GET_response.hpp"


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
    // visualize_string_map(mimes);
    // mimes += "application/x-www-form-urlencoded (No specific file extension)\n";
    // mimes += "multipart/form-data (No specific file extension)\n";
}

void     GET_response::fillingResponsePacket(std::string &full_file_to_string,  std::string file_path)
{
    response_packet = "HTTP/1.1 200 OK \r\n";
    response_packet += "Server: Phantoms\r\n";
    response_packet += "Date: ";
    response_packet += getTimeBuffer();
    response_packet += "Content-Type: " + getContentType(file_path) +" \r\n";
	std::stringstream ss;
	ss << full_file_to_string.length();
    //For CGI this can be chunked if you @Ahmed MAhdi decided to do so 
    response_packet += "Content-Length: " + ss.str() + "\r\n\r\n";
    response_packet += full_file_to_string; 
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

/*
if redirectedPAcket:
    fillRedirectedPacket;
    
getting filename
    size_t  file_location = file_path.rfind("/");
    if (file_location == std::string::npos || file_location == file_path.length() - 1)
        return ("text/html");
    std::string file_name = file_path.substr(file_location + 1, file_path.length());


    need the
    dir
    raw request->> last file (I believe from mime types)
    then will do the math inshalla
*/
bool    GET_response::redirectedPacket(stringmap &server_info, std::string &file_path)
{
    std::vector<std::string> redirection_vec;
    std::string file_name;
    size_t  file_location = file_path.rfind("/");
    std::string dir = server_info["constructed path dir"] ;
    if (file_location == std::string::npos || file_location == file_path.length() - 1)
        file_name = "/";
    else
        file_name = file_path.substr(file_location , file_path.length());
    std::string fetch_redirection = dir + " redirection";
    if (inMap(server_info, fetch_redirection))
    {
        if (server_info[fetch_redirection].find(file_name) != std::string::npos)
        redirection_vec = split(server_info[fetch_redirection], " , ");
        std::cout << "file name = " << file_name << std::endl;
        std::string winning_redirection = fetchWinningRedirection(redirection_vec, file_name);
        std::cout << "winning redirection = " << winning_redirection << std::endl;
        if (winning_redirection == "")
            return (false);
		std::vector<std::string> tmp = split(winning_redirection, " ");
		// std::cout << "response chcek path 1" << reponse_check["Path"][1] << std::endl;
		if (tmp.size() != 3)
			return (false);
		if (fill_redirection_vector(tmp))
			return (true);
    }
    return (false);
}

std::string    GET_response::fetchWinningRedirection(
                    std::vector<std::string> redirection_vec, std::string file_name)
{
    for (std::vector<std::string>::iterator it = redirection_vec.begin();
        it != redirection_vec.end(); it++)
    {
        std::cout << "searching for " << file_name << " in " << *it << std::endl;
        std::cout << file_name << " ==  " << split(*it, " ")[0] <<  std::endl;
        if (file_name == split(*it, " ")[0])
            return (*it);
    }
    return ("");
}

int GET_response::fill_redirection_vector(std::vector<std::string> tmp)
{
    redirection["old_path"] = tmp[0];
    redirection["new_path"] = tmp[1];
    redirection["Status-code"] = tmp[2];
    return (1);
}


void   GET_response::fillRedirectedPacket(void)
{
    response_packet = "HTTP/1.1 ";
    response_packet += redirection["Status-code"] + " ";
    response_packet +=  err.StatusCodes[redirection["Status-code"]] + " \r\n";
    response_packet += "Server: Phantoms\r\n";
    response_packet += "Location: " + redirection["new_path"] + "\r\n";
    response_packet += "Date: ";
    response_packet += getTimeBuffer();
    response_packet += "Connection: close\r\n";
    response_packet += "Content-Length: 0\r\n";
    response_packet += "Content-Type: text/html; charset=UTF-8 \r\n\r\n";
    std::cout << response_packet << std::endl;
    // exit (1);
}