#include "../../includes/webserve.hpp"

RequestHandler::RequestHandler(char *request_buffer)
{
	this->request = Request(request_buffer);
}
