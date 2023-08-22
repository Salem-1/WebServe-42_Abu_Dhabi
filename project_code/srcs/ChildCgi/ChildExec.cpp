#include "ChildExec.hpp"

ChildExec::ChildExec(packet_map &request_map, t_request &full_request, stringmap &server_info, int *fd): 
			PUT(request_map, full_request, server_info), _fd(fd)
	{
		_env = NULL;
	}

ChildExec::~ChildExec(){}

std::string ChildExec::standerdString(std::string str)
{
	size_t len = str.length() - 1;
	unsigned char temp;
	for(size_t i = 0; i < len; ++i)
	{
		temp = toupper(str[i]);
		if (isspace(temp) || temp == '-')
			temp = '_';
		str[i] = temp;
	}
	str[len] = '=';
	str = std::string("HTTP_") + str;
	return(str);
}

std::string ChildExec::vectorStr(std::vector<std::string> &vec)
{
	if (vec.size() == 0)
		return "";
	std::vector<std::string>::iterator it = vec.begin();
	std::string rtstr = *it;
	for (++it ;it != vec.end(); ++it)
	{
		rtstr += " " + *it;
	}
	return (rtstr);
}

char **ChildExec::envMaker(std::string path)
{
	std::string method ;
	size_t		i = 0;

	_request_map.find("GET") != _request_map.end()? method="GET" : method="POST";
	std::vector<std::string> env_vector;
	env_vector.push_back("REQUEST_METHOD=" + method);
	env_vector.push_back(std::string("SERVER_PROTOCOL=") + _request_map[method][1]);
	env_vector.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env_vector.push_back("SERVER_NAME=Phantoms");
	env_vector.push_back(std::string("SERVER_PORT=") +  _server_info["Port"]);
	env_vector.push_back(std::string("PATH_INFO=") + path);
	if (_server_info.find("query") != _server_info.end())
		env_vector.push_back(std::string("QUERY_STRING=") + _server_info["query"]);
	env_vector.push_back(std::string("PATH=") + _server_info["path to path"]);
	_request_map.erase(method);
	_request_map.erase("Status-Code");
	for(packet_map::iterator it = _request_map.begin(); it != _request_map.end(); ++it)
		env_vector.push_back(standerdString(it->first) + vectorStr(it->second));

	_env = new char*[env_vector.size() + 1];
	_env[env_vector.size()] = NULL;
	for (std::vector<std::string>::iterator it = env_vector.begin(); it  != env_vector.end(); ++it)
	{
		_env[i] = NULL;
		_env[i] = new char[it->size() + 1];
		bzero(_env[i], it->size() + 1);
		strcpy(_env[i], it->c_str());
		++i;
	}
	
	return _env;
}



void	ChildExec::childExecute(std::string path)
{
	char **args = NULL;
	try
	{
		if (dup2(_fd[1], STDOUT_FILENO) == -1)
			throw(std::runtime_error("dup failed"));
		close(_fd[1]);
		close(_fd[0]); 
		_fd = NULL;
		_env = envMaker(path);
		
		args = new char*[1];
		*args = NULL;
		if (execve(path.c_str(), args, _env) ==  -1)
		{
			perror("execve failed: ");
			std::cerr << "It freakin faild to execute " << path << std::endl;
		}
		delete [] args;
		for (int i = 0; _env[i] != NULL; ++i)
			delete [] _env[i];
		delete[] _env;
		std::cout<< "404";
		exit (127);
	}
	catch (std::bad_alloc &e)
	{
		close(_fd[1]);
		close(_fd[0]);
		for (int i = 0; _env != NULL && _env[i] != NULL; ++i)
			delete [] _env[i];
		if (_env != NULL)
			delete[] _env;
		if (args != NULL)
			delete[] args;
		std::cout<< "500";
		exit(1);
	}
}