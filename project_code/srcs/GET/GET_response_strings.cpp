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
    visualize_string_map(mimes);
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
   
}