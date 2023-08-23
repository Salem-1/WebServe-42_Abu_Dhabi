#ifndef CONFIGHANDLER_HPP
#define CONFIGHANDLER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>


// To open configuration specified by user and read it into a string
class ConfigHandler {
private:
    std::string filename;
	std::string fileContent;

    bool isValidFile();

public:
    ConfigHandler(const std::string& filePath);
	// Tries to open the file and read it into a string
    void readConfigFile();
	// void parseConfig();
};

#endif
