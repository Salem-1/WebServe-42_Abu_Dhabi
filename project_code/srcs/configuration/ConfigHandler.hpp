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
	std::vector<std::pair<std::string, std::vector<std::string> > > configstarter;

    bool	isValidFile();
    void	readConfigFile();
	void	parseConfig();
	bool	curlyBracesEvenCheckRemoveExtras(std::string& str);
	void	splitAndFillVector(const std::string& str, const std::string& delim);
	int		fillPair(std::string& str, std::pair<std::string, std::vector<std::string> >& pair);
	int		prepareLocation(std::string& str);
	void	tremString(std::string& str, const std::string& toRemove);
	void	tremStringOnce(std::string& str, const std::string& toRemove);
	void	errorAndExit(const std::string& msg);

public:
    ConfigHandler(const std::string& filePath);
	~ConfigHandler(){};
	void	handleConfig();
	std::vector<std::pair<std::string, std::vector<std::string> > > getConfigstarter() const;
};

#endif
