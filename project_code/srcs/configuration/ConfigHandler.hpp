#ifndef CONFIGHANDLER_HPP
#define CONFIGHANDLER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>


// To open configuration specified by user and read it into a string
class ConfigHandler {
private:
    std::string filename;
	std::string fileContent;
	std::vector<std::pair<std::string, std::string> > configstarter;

    bool	isValidFile();
	// Tries to open the file and read it into a string
    void	readConfigFile();
	void	parseConfig();
	bool	curlyBracesEvenCheckRemoveExtras(std::string& str);
	void	splitAndFillVector(std::vector<std::string>& vec, const std::string& str, const std::string& delim);
	int		fillPair(std::string& str, std::pair<std::string, std::vector<std::string> >& pair);
	void	errorAndExit(const std::string& msg);
	void	tremString(std::string& str, const std::string& toRemove);
	void	tremStringOnce(std::string& str, const std::string& toRemove);
	// bool	fillServers(std::vector<std::string>& servers);

public:
    ConfigHandler(const std::string& filePath);
	void	handleConfig();
};

#endif
