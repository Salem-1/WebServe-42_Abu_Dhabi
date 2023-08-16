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

std::string Respond::responseCGI(packet_map &request, stringmap &server_info, std::string &cgi_path, ErrResponse &err, std::string &body)
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
		return (execute(server_info, full_cgi_path, query, err));
	return (postExecute(server_info, full_cgi_path, query, err, body));
}

// std::string	fillingResponsePacket(std::string &full_file_to_string)
// {
// 	std::string response_packet;
// 	response_packet = "HTTP/1.1 200 OK \r\n";
// 	response_packet += "Server: webserve/1.0\r\n";
// 	response_packet += "Date: ";
// 	response_packet += getTimeBuffer();
// 	response_packet += "Content-Type: text/plain txt \r\n";
// 	std::stringstream ss;
// 	ss << full_file_to_string.length();
//     response_packet += "Content-Length: " + ss.str() + "\r\n\r\n";
// 	response_packet += full_file_to_string; 
// 	return (response_packet);
// }

void	childExecute(int *fd, std::string &path, std::string str_args)
{
	int env_size = 3;
	int i_env = -1;
	int i_args = -1;
	std::string hrdcode[] = {"SERVER_PROTOCOL=HTTP/1.1",
							"REQUEST_METHOD=POST", 
							"PATH_INFO=/Users/ayassin/Documents/git_files/WebServe_42_Abu_Dhabi/project_code/intra/YoupiBanane/youpi.bla"};
	char **env = NULL;
	char **args = NULL;
	try
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			throw(std::runtime_error("dup failed"));
		close(fd[1]);
		close(fd[0]);
		const char *temp_path = path.c_str();
		env = new char*[env_size + 1];
		env[env_size] = NULL;
		++i_env;
		for (; i_env < env_size; ++i_env)
		{
			env[i_env] = new char[200];
			strcpy(env[i_env], hrdcode[i_env].c_str());
		}
		if (str_args != "")
		{
			args = new char*[2];
			args[1] = NULL;
			++i_args;
			args[i_args] = new char[200];
			strcpy(args[i_args++], str_args.c_str());
			std::cout << BOLD << args[0] << RESET << std::endl;
		}
		if (execve(temp_path, args, env) ==  -1)
		{
			perror("execve failed: ");
			std::cerr << "It freakin faild to execute " << temp_path << std::endl;
		}
		for (int j = 0; j < env_size; ++j)
			delete env[j];
		delete[] env;
		std::cerr << BOLDMAGENTA <<  "*********************\n " << RESET;
		exit(127);

	}
	catch (std::bad_alloc &e)
	{
		for (int j = 0; j < i_env; ++j)
			delete env[j];
		if (i_env > 0)
			delete[] env;
		if (i_args == 1)
			delete args[0];
		else if (i_args == 0)
			delete [] args;
		std::cerr << BOLDMAGENTA << e.what() << "*********************\n " << RESET;
		exit(1);
	}
}

std::string readAndWrite(int infd, int outfd, std::string &body)
{
	std::string output;
	int write_size = 10;
	ssize_t n_read = 0;
	size_t		pos = 0;
	while (1) {
		if (pos <= body.length())
		{
			if (pos + write_size < body.length())
				n_read = write(infd, &(body[pos]), write_size);
			else
				n_read = write(infd, &(body[pos]), body.length() - pos);
			if (n_read < 0) {}
			pos += write_size;
			if (pos > body.length())
				close(infd);
		}
		char buffer[write_size];
		ssize_t count = read(outfd, buffer, write_size);
		if (count == -1) {
			perror("read failed");
			throw(std::runtime_error("read faild"));
		} else if (count == 0 && pos >= body.length()) 
		{
			break;
		} else if (count != 0)
		{
			output.append(buffer, count);
		}
	}

	close(outfd);
	return output;
}
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

void writeToChild(int *fd, std::string body)
{
	int write_size = 10;
	ssize_t n = 0;
	for (size_t i = 0; i < body.length(); i += write_size)
	{
		if (i + write_size <= body.length())
			n = write(fd[1], &(body[i]), write_size);
		else
			n = write(fd[1], &(body[i]), body.length() - i);

		if (n < 0)
		{
			// closePipe(infd);
			// closePipe(outfd);
			// perror("Write inside CGI:");
			throw(std::runtime_error("out read faild"));
		}
	}
	close(fd[1]);
}

std::string Respond::execute(stringmap &server_info, std::string path, std::string &args, ErrResponse &err)
{
	int fd[2];
	int status = 0;
	int id = 0;
	std::string output;
	(void) args;

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
			childExecute(fd, path, "");
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
		return (output);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (err.code(server_info, "501"));
	}
}



std::string Respond::postExecute(stringmap &server_info, std::string path, std::string &args, ErrResponse &err, std::string &body)
{
	int infd[2];
	int outfd[2];
	int status = 0;
	int id = 0;
	std::string output;
	(void) args;

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
			// int write_size = 10;
			// for (size_t i = 0; i < body.length(); i += write_size)
			// {
			// 	ssize_t n = write(infd[1], &(body[i]), write_size);
			// 	if (n < 0)
			// 	{
			// 		perror("Write inside CGI:");
			// 		// throw(std::runtime_error("out read faild"));
			// 	}
			// }
			if ((dup2(infd[0], STDIN_FILENO) == -1))
				throw(std::runtime_error("dup stdin failed"));
			close(infd[1]);
			close(infd[0]);
			path = "/Users/ayassin/Documents/git_files/WebServe_42_Abu_Dhabi/project_code/intra/cgi-bin/cgi_tester";
			childExecute(outfd, path, "");
		}
		// {
		// 	id = fork();
		// 	if (id == 0)
		// 	{
		// 		close(outfd[1]);
		// 		close(outfd[0]);
		// 		path = "/bin/echo";
		// 		childExecute(infd, path, body);
		// 	}
		// }
		close(outfd[1]);
		close(infd[0]);
		// output = readAndWrite(infd[1], outfd[0], body);
		writeToChild(infd, body);
		output = readFromChild(outfd[0]);
		// close(outfd[0]);
		while (waitpid(-1, &status, 0) > 0) {}
		if (WEXITSTATUS(status))
		{
			if (WEXITSTATUS(status) == 127)
				return (err.code(server_info, "404"));

		}
		vis_str(output, "inside CGI response");
		return (output);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (err.code(server_info, "501"));
	}
}
