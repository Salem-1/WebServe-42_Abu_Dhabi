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
        if (tmp[0] == reponse_check["Path"][1] && fill_redirection_vector(tmp))
            return (true);
    }
    return (false);
}


/*
    if list dir off
    construct path to serve the index of the parent directory
    give to the noremal filling 
    inshalla
*/
void    GET_response::fillOkResponse(stringmap &server_info)
{
    response_packet = "";
    if(redirectedPacket(server_info))
    {
        fillRedirectedPacket();
        return ;
    }
    std::string file_path = constructPath(server_info);

    std::cout << BOLDMAGENTA << "requested file path = "
    		<< RESET << file_path << std::endl << RESET;
        
    if (!sanitizedPath(file_path)&& fillBadPath(server_info))
        return ;
    
    std::cout << MAGENTA << "constructed path = " << file_path << std::endl << RESET;
    DIR *dir;
    std::string full_file_to_string;
    if ((dir  = opendir(file_path.c_str())) != NULL)
    {
        std::string dir_list_option = reponse_check["dir"][0] + " autoindex";
        if (reponse_check["dir"].size() != 1)
        {
            response_packet = err.code(server_info, "400");
            return ;
        }
        if (server_info.find(dir_list_option) != server_info.end())
        {
            if (server_info[dir_list_option] == "on")
            {
                struct dirent *files;
                std::vector<std::string> ls;
                ls.push_back(file_path);
                while ((files = readdir(dir)) != NULL)
                    ls.push_back(files->d_name);
                constructDirResponse(ls, full_file_to_string);
            }
            else
            {
                response_packet = err.code(server_info, "403");
                return ;
            }
        }
        else
        {
            if (server_info.find(reponse_check["dir"][0] + " index") != server_info.end())
            {
                file_path += "/" + server_info[reponse_check["dir"][0] + " index"];
                std::cout << YELLOW << "modified file  path = " << file_path << std::endl << RESET;
                if (!readFileToString(server_info, file_path, full_file_to_string))
                    return;
            }
        }
    }
    else
    {
        if (!readFileToString(server_info, file_path, full_file_to_string))
            return;
    }
    fillingResponsePacket(full_file_to_string, file_path);

}


bool    GET_response::readFileToString(stringmap &server_info, 
                                        std::string &file_path, 
                                        std::string &full_file_to_string)
{
    std::ifstream infile(file_path.c_str());
    if (infile.fail() && fillBadPath(server_info))
        return (false);
    std::stringstream content_stream;
    content_stream  << infile.rdbuf();
    full_file_to_string = content_stream.str();
    return (true);
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
    std::cout << MAGENTA << "requested path = " << path << std::endl << RESET ;

    if (path == "/")
        return (server_info[path]);
    if (std::count(path.begin(), path.end(), '/') < 2)
    {
        if (server_info.find(path) != server_info.end())
        {
            if (server_info.find(path + " index") != server_info.end())
                return (server_info[path] +server_info[path + " index"]);
        }
        // else
        //     return (server_info["root"] + path);
    }
    std::string dir = path.substr(0, path.substr(1, path.length()).find("/") + 1);
    std::cout << MAGENTA << "dir == " << dir << " path = " << path << std::endl << RESET;
    // exit(0);
    if (path[path.length() - 1] == '/' && dir.length() == path.length() - 1)
    {
        std::cout << "yes it's only dir" << std::endl;
        if (server_info.find(dir) != server_info.end())
            return (server_info[dir + " index"]);
    }
    // images/cat.jpeg
    std::string rest_of_path = path.substr(dir.length() + 1, path.length());

    std::cout << MAGENTA << "rest of path = " << rest_of_path << std::endl << RESET;
    
    //the error is here
    std::cout << "dir = " << dir << std::endl;

    if (server_info.find(dir) != server_info.end())
    {
        print_to_file("testers/our_tester/logs/dir_path.txt", server_info[dir]);
        return (server_info[dir] + rest_of_path);
    }
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


int GET_response::fill_redirection_vector(std::vector<std::string> tmp)
{
    redirection["old_path"] = tmp[0];
    redirection["new_path"] = tmp[1];
    redirection["Status-code"] = tmp[2];
    return (1);
}
