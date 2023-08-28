#include "../includes/libs.hpp"
#include "../includes/webserve.hpp"


int main(int argc, char **argv, char **env)
{

	if (argc < 2)
        run_default(env);
	if (argc == 2)
        run_server(env, argv[1]);
    else
        std::cout << BOLDRED << "wrong number of arguments" << RESET <<std::endl;


    return (0);
}

