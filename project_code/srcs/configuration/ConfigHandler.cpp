#include "ConfigHandler.hpp"

ConfigHandler::ConfigHandler(const std::string& filePath) : filename(filePath) 
{}

void ConfigHandler::errorAndExit(const std::string& msg)
{
	std::cerr << "Configuration file:\n" << msg << std::endl;
	exit(1);
}

void ConfigHandler::handleConfig()
{
	readConfigFile();
	parseConfig();
}

void ConfigHandler::readConfigFile()
{
    if (!ConfigHandler::isValidFile())
		errorAndExit("Invalid or non-existent configuration file: " + filename);
	std::ifstream inFile(filename.c_str());
    if (!inFile)
		errorAndExit("Error: Could not open " + filename + " for reading.");

    // Read the content of the existing file into a stringstream
    std::stringstream contentStream;
    contentStream << inFile.rdbuf();
    inFile.close();

	fileContent = contentStream.str();
	if (fileContent.empty())
		errorAndExit("Error: empty configuration file");
}

bool ConfigHandler::isValidFile()
{
    if (filename.length() > 5 && filename.substr(filename.length() - 5) == ".conf") {
        std::ifstream file(filename.c_str());
        return file.good();
    }
    return false;
}


void ConfigHandler::parseConfig()
{
	if (!curlyBracesEvenCheckRemoveExtras(fileContent))
		errorAndExit("Error: curly brace/s");
	std::vector<std::string> servers;
	if (!fileContent.empty())
		splitAndFillVector(servers, fileContent, "server");
	else
		errorAndExit("Error: empty configuration file");
	// if (!fillServers(servers))
	// {
	// 	std::cerr << "Error: bad configuration argument/s" << std::endl;
	// 	exit(1);
	// }
	// std::string line;
	// std::stringstream ss(fileContent);
	// while (std::getline(ss, line, '\n'))
	// {
	// 	std::cout << line << std::endl;
	// }
}

bool ConfigHandler::curlyBracesEvenCheckRemoveExtras(std::string& str)
{
	std::string result;
	bool isPrevousExtra = false;
	int curlyBraces = 0;
	size_t n = 0;
	while(n < str.length())
	{
		if (str[n] != '\n' && str[n] != '\t' && str[n] != ' ')
			break;
		n++;
	}
	for (size_t i = n; i < str.length(); i++)
	{
		if (str[i] == '{')
			curlyBraces++;
		else if (str[i] == '}')
			curlyBraces--;
		if (str[i] == '\n' || str[i] == '\t' || str[i] == ' ')
		{
			if (!isPrevousExtra)
			{
				result += str[i];
				isPrevousExtra = true;
			}
		}
		else
		{
			result += str[i];
			isPrevousExtra = false;
		}		
	}
	tremString(result, " \t\r\n");
	str = result;
	return (curlyBraces == 0);
}


int ConfigHandler::fillPair(std::string& str, std::pair<std::string, std::vector<std::string> >& pair)
{
	(void) pair;
	std::string delim = "location";
	std::string block;
	size_t start = 0;
	size_t end = str.find(delim);
	size_t edge;
	while (end != std::string::npos)
	{
		edge = end + (delim.length());
		if (str[edge] != '\n' && str[edge] != '\t' && str[edge] != ' ')
		{
            end = str.find(delim, edge);
            continue;
        }
		block = str.substr(start, end - start);
		tremString(block, " \t\r\n");
		if (curlyBracesEvenCheckRemoveExtras(str) && !str.empty())
		{
			std::cout << block << std::endl;												// delete
			std::cout << "##############################################" << std::endl;		// delete
		}
		else
			errorAndExit("Error: configuration path/s");
		start = end + delim.length();
		end = str.find(delim, start);
	}
	block = str.substr(start, str.length());
	std::cout << block << std::endl;												// delete
	std::cout << "##############################################" << std::endl;		// delete
	return (1);
}

void	ConfigHandler::splitAndFillVector(std::vector<std::string>& vec, const std::string& str, const std::string& delim)
{
	size_t start = 0;
	size_t end = str.find(delim);
	size_t edge;
	size_t	split_0 = 0;
	std::string	server;
	std::pair<std::string, std::vector<std::string> > pair;
	while (end != std::string::npos)
	{
		edge = end + delim.length();
		if (str[edge] != '{' && str[edge] != '\n' && str[edge] != '\t' && str[edge] != ' ')
		{
            end = str.find(delim, edge);
            continue;
        }
		if (str[edge] == ' ' || str[edge] == '\t' || str[edge] == '\n')
		{
			size_t i = edge;
			while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
				i++;
			if (str[i] != '{')
			{
				end = str.find(delim, edge);
				continue;
			}
		}
		server = str.substr(start, end - start);
		if (!split_0++)
		{
			if (!server.empty())
				errorAndExit("Error: bad configuration argument/s");
		}
		else if (curlyBracesEvenCheckRemoveExtras(server) && !server.empty() && server[0] == '{' && server[server.length() - 1] == '}')
		{
			tremStringOnce(server, "{}");
			if (!curlyBracesEvenCheckRemoveExtras(server) || server[server.length() - 1] != '}')
				errorAndExit("Error: curly brace/s");
			fillPair(server, pair);
			std::cout << "========================================================" << std::endl;		// delete
			vec.push_back(server);

		}
		else
			errorAndExit("Error: configuration variable/s");

		start = end + delim.length();
		end = str.find(delim, start);
	}
	server = str.substr(start, str.length());
	if (curlyBracesEvenCheckRemoveExtras(server) && !server.empty() && server[0] == '{' && server[server.length() - 1] == '}')
	{
		tremStringOnce(server, "{}");
		if (!curlyBracesEvenCheckRemoveExtras(server) || server[server.length() -1] != '}')
			errorAndExit("Error: curly brace/s");
		fillPair(server, pair);
		std::cout << "========================================================" << std::endl;
		vec.push_back(server);
	}
	else
		errorAndExit("Error: configuration variable/s");
}

void	ConfigHandler::tremString(std::string& str, const std::string& toRemove)
{
	size_t start = str.find_first_not_of(toRemove);
    if (start != std::string::npos)
        str.erase(0, start);
	else
	{
        str.clear(); // Entire string consists of the specified character
		return;
	}
	size_t end = str.find_last_not_of(toRemove);
    if (end != std::string::npos)
        str.erase(end + 1);
}

void	ConfigHandler::tremStringOnce(std::string& str, const std::string& toRemove)
{
	size_t start = str.find_first_of(toRemove);
	if (start != std::string::npos)
		str.erase(0, ++start);
	else
	{
		str.clear(); // Entire string consists of the specified character
		return;
	}
	size_t end = str.find_last_of(toRemove);
	if (end != std::string::npos)
		str.erase(end);
}
