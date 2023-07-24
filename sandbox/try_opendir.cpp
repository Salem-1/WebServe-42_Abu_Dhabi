#include <dirent.h>
#include <iostream>
#include <vector>
int main(int argc, char **argv)
{
    DIR *dir;
    if (argc == 1)
        dir  = opendir(".");
    else
        dir  = opendir(argv[1]);
    if (dir == NULL)
    {
        std::cout << "Error opening dir " << std::endl;
        return (1);
    }
    std::vector<std::string> ls;
    struct dirent *files;
    while ((files = readdir(dir)) != NULL)
        ls.push_back(files->d_name);
    int i = 1;
    for (std::vector<std::string>::iterator it = ls.begin(); it != ls.end(); it++)
        std::cout << *it << std::endl;
    (void)closedir(dir);
}
