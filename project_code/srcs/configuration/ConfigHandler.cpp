#include "ConfigHandler.hpp"

ConfigHandler::ConfigHandler(const std::string& filePath) : filename(filePath) {}

bool ConfigHandler::isValidFile() {
    if (filename.length() > 5 && filename.substr(filename.length() - 5) == ".conf") {
        std::ifstream file(filename.c_str());
        return file.good();
    }
    return false;
}

void ConfigHandler::readConfigFile() {
    if (!isValidFile()) 
    {
        std::cerr << "Invalid or non-existent configuration file: " << filename << std::endl;
        exit(1);
    }
	
	std::ifstream inFile(filename.c_str());
    if (!inFile) {
        std::cerr << "Error: Could not open " << filename << " for reading.\n";
        exit(1);
    }
	
    // Read the content of the existing file into a stringstream
    std::stringstream contentStream;
    contentStream << inFile.rdbuf();
    inFile.close();

	fileContent = contentStream.str();
}
