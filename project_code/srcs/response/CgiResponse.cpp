#include "Respond.hpp"

void Respond::closePipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

std::string	Respond::isCGI(packet_map &request)
{
	packet_map::iterator it = request.find("GET");
	if (it == request.end())
		it = request.find("POST");
	if (it == request.end())
		return ("");
	if (it->second.size() > 0 && (it->second[0].find("cgi-bin") != std::string::npos ||
		(it->second[0].find(".bla") != std::string::npos && it->first == "POST")))
		return (it->second[0]);
	
	return ("");
}

std::string Respond::responseCGI(packet_map &request, stringmap &server_info, std::string &cgi_path, t_request &full_request)
{
	std::string query;
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
	if (request.find("GET") != request.end())
		return (execute(request, full_request, server_info, full_cgi_path));
	return (postExecute(request, full_request, server_info, full_cgi_path));
}

std::string	fillingResponsePacket(packet_map &request, std::string &full_file_to_string)
{
	
	std::string status =  " 200 OK\r\n";
	std::string type = " text/plain txt\r\n";
	std::string body =  full_file_to_string;
	size_t  temp_pos = 0;
	size_t head_end = full_file_to_string.find("\r\n\r\n");
	if (head_end != full_file_to_string.npos)
	{
		std::string header_fields = full_file_to_string.substr(0, head_end + 4);
		body = full_file_to_string.substr(head_end + 4);
		temp_pos = header_fields.find("Status:");
		if (temp_pos != header_fields.npos)
			status = header_fields.substr(temp_pos + 7, header_fields.find("\r\n", temp_pos) + 2 - temp_pos - 7);
		temp_pos = header_fields.find("Content-Type:");
		if (temp_pos != header_fields.npos)
			type = header_fields.substr(temp_pos + 13, header_fields.find("\r\n", temp_pos)+ 2 - temp_pos - 13);
	}

	std::string response_packet;
	
	response_packet = "HTTP/1.1" + status;
	response_packet += "Server: Phantoms\r\n";
	response_packet += "Date: ";
	response_packet += getTimeBuffer();
	response_packet += "Content-Type:" + type;
	if (request.find("X-Secret-Header-For-Test:") !=  request.end())
		response_packet += "X-Secret-Header-For-Test: 1\r\n";
	std::stringstream ss;
	ss << body.length();
	response_packet += "Content-Length: " + ss.str() + "\r\n\r\n";
	response_packet += body; 
	return (response_packet);
}




// void	childExecute(int *fd, std::string &path)
// {
// 	int env_size = 4;
// 	int i_env = -1;
// 	std::string hrdcode[] = {"SERVER_PROTOCOL=HTTP/1.1",
// 							"REQUEST_METHOD=POST", 
// 							"PATH_INFO=/Users/ayassin/Documents/git_files/WebServe_42_Abu_Dhabi/project_code/intra/YoupiBanane/youpi.bla",
// 							"HTTP_X_SECRET_HEADER_FOR_TEST=1"};
// 	char **env = NULL;
// 	try
// 	{
// 		if (dup2(fd[1], STDOUT_FILENO) == -1)
// 			throw(std::runtime_error("dup failed"));
// 		close(fd[1]);
// 		close(fd[0]);
// 		const char *temp_path = path.c_str();
// 		env = new char*[env_size + 1];
// 		env[env_size] = NULL;
// 		++i_env;
// 		for (; i_env < env_size; ++i_env)
// 		{
// 			env[i_env] = new char[200];
// 			strcpy(env[i_env], hrdcode[i_env].c_str());
// 		}
// 		if (execve(temp_path, NULL, env) ==  -1)
// 		{
// 			perror("execve failed: ");
// 			std::cerr << "It freakin faild to execute " << temp_path << std::endl;
// 		}
// 		for (int j = 0; j < env_size; ++j)
// 			delete env[j];
// 		delete[] env;
// 		std::cerr << BOLDMAGENTA <<  "*********************\n " << RESET;
// 		exit(127);

// 	}
// 	catch (std::bad_alloc &e)
// 	{
// 		for (int j = 0; j < i_env; ++j)
// 			delete env[j];
// 		if (i_env > 0)
// 			delete[] env;
// 		std::cerr << BOLDMAGENTA << e.what() << "*********************\n " << RESET;
// 		exit(1);
// 	}
// }


std::string readFromChild(int fd)
{
	std::string output;
	while (1) {
		char buffer[10];
		ssize_t count = read(fd, buffer, sizeof(buffer));
		if (count == -1) {
			perror("read failed");
			throw(std::runtime_error("read faild"));
		} else if (count == 0) 
		{
			break;
		} else 
		{
			output.append(buffer, count);
		}
	}
	close(fd);
	return output;
}

std::string Respond::execute(packet_map &request, t_request &full_request, stringmap &server_info, std::string &path)
{
	int fd[2];
	int status = 0;
	int id = 0;
	std::string output;

	try 
	{
		if (pipe(fd) == -1)
			throw(std::runtime_error("pipe faild"));
		id = fork();
		if (id == -1)
		{
			closePipe(fd);
			throw(std::runtime_error("fork failed"));
		}
		if (id == 0)
		{
			ChildExec child(request, full_request, server_info, fd);
			child.childExecute(path);
			exit(127);
		}
			// childExecute(fd, path);
		close(fd[1]);
		output = readFromChild(fd[0]);
		close(fd[0]);
		while (waitpid(-1, &status, 0) > 0) {}
		if (WEXITSTATUS(status))
		{
			if (WEXITSTATUS(status) == 127)
				return (err.code(server_info, "404"));
			return (err.code(server_info, "501"));
		}
		vis_str(output, "inside CGI response");
		return (fillingResponsePacket(request, output));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (err.code(server_info, "501"));
	}
}



std::string Respond::postExecute(packet_map &request, t_request &full_request, stringmap &server_info, std::string &path)
{
	int infd[2];
	int outfd[2];
	int status = 0;
	int id = 0;
	std::string output;

	try 
	{
		if (pipe(infd) == -1)
			throw(std::runtime_error("pipe faild"));
		if (pipe(outfd) == -1)
		{
			closePipe(infd);
			throw(std::runtime_error("pipe faild"));
		}
		id = fork();
		if (id == -1)
		{
			closePipe(infd);
			closePipe(outfd);
			throw(std::runtime_error("fork failed"));
		}
		if (id == 0)
		{
			if ((dup2(infd[0], STDIN_FILENO) == -1))
				throw(std::runtime_error("dup stdin failed"));
			close(infd[1]);
			close(infd[0]);
			path = "/Users/ayassin/Documents/git_files/WebServe_42_Abu_Dhabi/project_code/intra/cgi-bin/cgi_tester";
			ChildExec child(request, full_request, server_info, outfd);
			child.childExecute(path);
			exit(127);
		}
		close(outfd[1]);
		close(infd[0]);
		output = ReadAndWirte(infd[1], outfd[0], full_request.body);
		while (waitpid(-1, &status, 0) > 0) {}
		if (WEXITSTATUS(status))
		{
			if (WEXITSTATUS(status) == 127)
				return (err.code(server_info, "404"));

		}
		vis_str(output, "inside CGI response");
		return (fillingResponsePacket(request ,output));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (err.code(server_info, "501"));
	}
}
