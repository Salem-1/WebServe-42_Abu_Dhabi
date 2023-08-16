#include "Respond.hpp"


void    Respond::visualizeResponse()
{
    std::cout << BOLDRED << "\nVisualizing reponse API\n" << std::endl;
    std::cout << "{" << std::endl << RESET;
    for (response_packet::iterator it = response.begin(); it != response.end(); it++)
    {
        if ((it->first).length() < 10000)
            std::cout << BOLDBLUE << "  \"" << it->first << "\": [" << RESET;
        else
            std::cout << "\"" << "large packet not gonna visualize" << "\", ";

         for (std::vector<std::string>::iterator vit = it->second.begin(); vit != it->second.end(); ++vit)
        {
            if ((*vit).length() < 10000)
                std::cout << "\"" << *vit << "\", ";
            else
                std::cout << "\"" << "large packet not gonna visualize" << "\", ";
        }
        std::cout << BOLDBLUE << "]" << std::endl << RESET;
    }
    std::cout << BOLDRED << "}" << std::endl << RESET;
}
