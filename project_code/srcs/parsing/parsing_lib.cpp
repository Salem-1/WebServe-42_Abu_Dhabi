#include "parsing_lib.hpp"

std::vector<std::string> split(std::string str, std::string delimeter)
{
    std::vector<std::string>    result;

    if (!str.length())
        result.push_back("");
   else if (str.find(delimeter) == std::string::npos || !delimeter.length())
        result.push_back(str);
    if (str.find(delimeter) == std::string::npos || !str.length() || !delimeter.length())
        return (result);
    while (str.find(delimeter) != std::string::npos)
    {
        if (str.substr(0, str.find(delimeter))[0])
            result.push_back(str.substr(0, str.find(delimeter)));
        str = str.substr(str.find(delimeter) + delimeter.length(), str.length());   
    }
    if (str.substr(0, str.find(delimeter))[0])
        result.push_back(str.substr(0, str.find(delimeter)));
    return (result);
}