#include "../includes/libs.hpp"
#include "../includes/webserve.hpp"


tokenized_conf    dummy_intra_token_fill();
void visualize_tokens(tokenized_conf &tokens);
int main(int argc, char **argv, char **env)
{

    tokenized_conf filled_tokens;
	if (argc < 2)
        run_default(env);
	if (argc == 2)
        run_server(env, argv[1]);
    // else
    //     throw(std::invalid_argument("wron conf file"));


    return (0);
}

