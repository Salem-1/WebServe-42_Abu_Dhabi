#include "Respond.hpp"

std::string	Respond::isCGI(packet_map &request)
{
	packet_map::iterator it = request.find("GET");
	if (it == request.end())
		it = request.find("POST");
	if (it == request.end())
		return ("");
	if (it->second.size() > 0 && it->second[0].find("cgi-bin") != std::string::npos)
		return (it->second[0]);
	return ("");
}

std::string	fillingResponsePacket(std::string &full_file_to_string)
{
	std::string response_packet;
	response_packet = "HTTP/1.1 200 OK \r\n";
	response_packet += "Server: webserve/1.0\r\n";
	response_packet += "Date: ";
	response_packet += getTimeBuffer();
	response_packet += "Content-Type: text/plain txt \r\n";
	std::stringstream ss;
	ss << full_file_to_string.length();
    response_packet += "Content-Length: " + ss.str() + "\r\n\r\n";
	response_packet += full_file_to_string; 
	return (response_packet);
}

std::string Respond::execute(stringmap &server_info, std::string path, std::string args)
{
	int fd[2];
	(void) path;
	(void) args;
	(void) fd;

	try 
	{
		if (pipe(fd) == -1)
			throw(std::runtime_error("pipe faild"));
		int id = fork();
		if (id == -1)
			throw(std::runtime_error("fork failed"));
		if (id == 0)
		{
			while ((dup2(fd[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
			close(fd[1]);
			close(fd[0]);
			// std::cerr << BOLDMAGENTA <<  "*********************\n " << RESET;
			const char *temp_path = path.c_str();
			// std::cerr << temp_path << std::endl;
			char **args = (char **)malloc(sizeof(*args) * (2));
			if (args == NULL)
				return (NULL);
			args[0] = NULL;
			args[1] = NULL;
			if (execve(temp_path, args, NULL) ==  -1)
				std::cerr << "It freakin faild to execute " << temp_path << std::endl;
			free(args);
			close(fd[0]);
			close(fd[1]);
			std::cerr << BOLDMAGENTA <<  "*********************\n " << RESET;
			exit(0);
		}
		close(fd[1]);
		std::string output;
		while (1) {
			char buffer[10];
			ssize_t count = read(fd[0], buffer, sizeof(buffer));
			if (count == -1) {
				if (errno == EINTR) {
					continue;
				} else {
					perror("read failed");
					throw(std::runtime_error("read faild"));
				}
			} else if (count == 0) {
				break;
			} else {
				output.append(buffer, count);
			}
		}
		close(fd[0]);
		int status;
		while (waitpid(-1, &status, 0) > 0) {}
		vis_str(output, "inside CGI response");
		// std::cout << BOLDBLACK << output << RESET << std::endl;
		return (fillingResponsePacket(output));
		ErrResponse err;
		return (err.code(server_info, "200"));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		ErrResponse err;
		return (err.code(server_info, "501"));
	}
	
	
}

std::string Respond::responseCGI(packet_map &request, stringmap &server_info, std::string &cgi_path)
{
	std::string query;
	(void) request;
    std::cout << "inside response CGI" << std::endl;
	if (cgi_path.find("?") != std::string::npos)
	{
		query = cgi_path.substr(cgi_path.find("?") + 1, cgi_path.length() - 1);
		cgi_path = cgi_path.substr(0, cgi_path.find("?"));
		// server_info["query"] = query;
	}
	std::string full_cgi_path = server_info["cgi-bin"] + cgi_path;
	std::cout << BOLDGREEN << "full path = " << full_cgi_path << std::endl << RESET;
	std::cout << BOLDGREEN << "query = " << query << std::endl << RESET;
	return (execute(server_info, full_cgi_path, query)); 
}