#include "Respond.hpp"

void Respond::closePipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

std::string	Respond::isCGI(stringmap &server_info, packet_map &request)
{
	std::string path = "";
	std::string query;
	packet_map::iterator it = request.find("GET");
	if (it == request.end())
		it = request.find("POST");
	if (it == request.end())
		return ("");
	else 
		path = it->second[0];
	if (path.find("?") != std::string::npos)
	{
		query = path.substr(path.find("?") + 1, path.length() - 1);
		path = path.substr(0, path.find("?"));
		server_info["query"] = query;
	}
	if (path.find("/cgi-bin/") == 0)
		return (server_info["/cgi-bin"] + path.substr(9));
	else if (it->first == "GET")
		return ("");
	else if (path.rfind(".") == path.npos)
		return ("");
	std::string file_extension = path.substr(path.rfind("."));
	if (server_info.find(file_extension) != server_info.end() && it->first == "POST")
	{
		path = server_info[file_extension];
		if (path.find("/cgi-bin/") != std::string::npos)
			return (path);
	}
	return ("");
}

std::string	Respond::fillingResponsePacket(stringmap &server_info, std::string &full_file_to_string)
{
	if (full_file_to_string.rfind("50", 1) == 0 || full_file_to_string.rfind("40", 1) == 0)
		return (err.code(server_info, full_file_to_string));
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
		temp_pos = header_fields.find("Set-Cookie:");
		if (temp_pos != header_fields.npos)
			type += header_fields.substr(temp_pos, header_fields.find("\r\n", temp_pos)+ 2);
	}

	std::string response_packet;
	
	response_packet = "HTTP/1.1" + status;
	response_packet += "Server: Phantoms\r\n";
	response_packet += "Date: ";
	response_packet += getTimeBuffer();
	response_packet += "Content-Type:" + type;
	std::stringstream ss;
	ss << body.length();
	response_packet += "Content-Length: " + ss.str() + "\r\n\r\n";
	response_packet += body; 
	return (response_packet);
}


std::string readFromChild(int fd)
{
	std::string output;
	while (1) {
		char buffer[10];
		ssize_t count = read(fd, buffer, sizeof(buffer));
		if (count == -1) {
			print_error("read failed");
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

std::string Respond::getExecute(packet_map &request, t_request &full_request, stringmap &server_info, std::string &path)
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
		}
		close(fd[1]);
		output = readFromChild(fd[0]);
		close(fd[0]);
		while (waitpid(-1, &status, 0) > 0) {}
		if (WEXITSTATUS(status) == 2)
				return (err.code(server_info, "400"));
		return (fillingResponsePacket(server_info, output));
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
			ChildExec child(request, full_request, server_info, outfd);
			child.childExecute(path);
		}
		close(outfd[1]);
		close(infd[0]);
		output = ReadAndWirte(infd[1], outfd[0], full_request.body);
		while (waitpid(-1, &status, 0) > 0) {}
		if (WEXITSTATUS(status) == 2)
				return (err.code(server_info, "413"));
		return (fillingResponsePacket(server_info, output));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (err.code(server_info, "501"));
	}
}
