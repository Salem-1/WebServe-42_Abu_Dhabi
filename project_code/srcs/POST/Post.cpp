#include "Post.hpp"

Post::Post(packet_map &request_map, t_request &full_request, stringmap &server_info)
{
	//fill the default response as error
	this->_response = "HTTP/1.1 500 Internal Server Error\r\n"
                       "Content-Type: text/html\r\n"
                       "Content-Length: 223\r\n\r\n"
                       "<!DOCTYPE html>\n"
                       "<html>\n"
                       "<head>\n"
                       "    <title>Internal Server Error</title>\n"
                       "</head>\n"
                       "<body>\n"
                       "    <h1>500 Internal Server Error</h1>\n"
                       "    <p>The server encountered an unexpected condition that prevented it from fulfilling the request.</p>\n"
                       "</body>\n"
                       "</html>\n";
	this->_request_map = request_map;
	this->_request = full_request;
	this->_server_info = server_info;
}

Post::~Post()
{

}

void	Post::printPostHeader()
{
	std::cout << YELLOW << "\nPOST request header is: " << std::endl;
	std::cout << this->_request.header << std::endl;
	std::cout << RESET;
}

void	Post::printPostBody()
{
	std::cout << "\nPOST request body is: " << std::endl;
	std::cout << this->_request.body << std::endl;
}

void Post::printReceivedRequestMap()
{
	std::cout << YELLOW << "\nPOST request header map is: " << std::endl;
	visualizeStringMap(this->_request_map);
	std::cout << RESET;
}

void Post::visualizeStringMap(packet_map &map)
{
    for (packet_map::iterator it = map.begin(); it != map.end(); it++)
    {
        std::cout << it->first << ": ";
        for (size_t i = 0; i < it->second.size(); i++)
        {
            std::cout << it->second[i];
            if (i != it->second.size() - 1)
            {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}


static int simpleBackend(std::string body, std::string &received_body)
{
	char buff[4000];
    std::string pwd(getcwd(buff, sizeof(buff)));
	std::string filePath = pwd + "/intra/website/post_backend/index.html";
	std::ifstream inFile(filePath.c_str());
    if (!inFile) {
        std::cerr << "Error: Could not open " << filePath << " for reading.\n";
        return 1;
    }

    // Read the content of the existing file into a stringstream
    std::stringstream contentStream;
    contentStream << inFile.rdbuf();
    inFile.close();

    // Find the specific comment to be modified
    std::string searchString = "<!-- INSERT COMMENT HERE -->";
    std::string content = contentStream.str();
    std::size_t pos = content.find(searchString);
    if (pos == std::string::npos) {
        std::cerr << "Error: Could not find the specified comment in the file.\n";
        return 1;
    }

    // Modify the content by inserting the body after the specified comment
    std::stringstream modifiedContent;
    modifiedContent << content.substr(0, pos + searchString.length());
    modifiedContent << "\n\t\t\t<p>" << body << "</p>";
    modifiedContent << content.substr(pos + searchString.length());

    std::ofstream outFile(filePath.c_str());
    if (!outFile) {
        std::cerr << "Error: Could not open " << filePath << " for writing.\n";
        return 1;
    }

    // Write the modified content back to the file
    outFile << modifiedContent.str();
    outFile.close();
	received_body = modifiedContent.str();

    return 0;
}

void Post::sendToBackend()
{
	int status = 1;
	std::string	received_body = "";
	if (this->_request.body.find("comment=") != std::string::npos)
	{
		std::string comment = this->_request.body.substr(this->_request.body.find("comment=") + 8);
		for (size_t i = 0; i < comment.length(); i++)
		{
			//parsing serilized url
			if (comment[i] == '+')
				comment[i] = ' ';
			else if (comment[i] == '%')
			{
				std::string hex = comment.substr(i + 1, 2);
				char chr = (char)std::strtol(hex.c_str(), NULL, 16);
				comment[i] = chr;
				comment.erase(i + 1, 2);
			}
		}
		status  = simpleBackend(comment, received_body);
	}
	if (status)
		return ;
	this->_response = "HTTP/1.1 200 OK\r\n"
					"Content-Type: text/html\r\n"
					"Content-Length: " + std::to_string(received_body.length()) 
					+ "\nConnection: keep-alive\r\n\r\n" + received_body;
}

std::string Post::get_response() const
{
	return (this->_response);
}
