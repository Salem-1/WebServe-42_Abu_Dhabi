#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
int main()
{
    DIR *dir;
    struct dirent *entry;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        return 1;
    }
    printf("cwd is %s \n", cwd);
    dir = opendir(cwd);
    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}
