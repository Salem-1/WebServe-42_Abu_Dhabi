#ifndef CHILDEXEC_HPP
# define CHILDEXEC_HPP

# include "../../includes/libs.hpp"
# include "../PUT/PUT.hpp"
#include "../response/ErrResponse.hpp"

class ChildExec: public PUT
{
private:

	int				*_fd;
	char			**_env;

	std::string standerdString(std::string str);
	std::string vectorStr(std::vector<std::string> &vec);
	char **envMaker(std::string path);



public:
	ChildExec(packet_map &request_map, t_request &full_request, stringmap &server_info, int *fd);
	~ChildExec();

	void childExecute(std::string path);
};

#endif 