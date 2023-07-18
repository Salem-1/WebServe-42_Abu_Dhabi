#include <dirent.h>
#include <iostream>
#include <vector>
#include <fstream>

int main(int argc, char **argv)
{
     std::ifstream infile(argv[1]);
    if (infile.fail())
    {
        std::cout << "not file" << std::endl;
    }
    else
        std::cout << "it's a file" << std::endl;
}