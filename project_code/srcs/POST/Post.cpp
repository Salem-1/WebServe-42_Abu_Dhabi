#include "Post.hpp"

Post::Post(packet_map &request_map, t_request &full_request, 
    stringmap &server_info, response_packet & response)
    : PUT(request_map, full_request, server_info, response)
{}

Post::~Post()
{

}

void	Post::printPostHeader()
{
	std::cout << BOLDYELLOW << "\nPOST request header is: " << std::endl << RESET;
	std::cout << BOLDYELLOW << this->_request.header << std::endl << RESET;
}

void	Post::printPostBody()
{
	std::cout << BOLDYELLOW << "\nPOST request body is: " << std::endl << RESET;
	std::cout << BOLDYELLOW << this->_request.body << std::endl << RESET;
}

void Post::printReceivedRequestMap()
{
	std::cout << YELLOW << "\nPOST request header map is: " << std::endl << RESET;
	// visualizeStringMap(this->_request_map);
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


std::string Post::get_response() const
{
	return (this->_response);
}


// static function to back for handleUpload method which is used to check if file exists
static bool fileExists(const std::string& name) 
{
	std::ifstream file(name.c_str());
	return file.good();
}

void Post::handleUpload(std::string path)
{

    // Parse the request header to get the content boundary
    std::string boundary = this->_request.header.substr(this->_request.header.find("boundary=") + 9);
	boundary = boundary.substr(0, boundary.find("\n"));

    // Find the start and end positions of the file data in the request body
  	size_t start = this->_request.body.find(boundary) + boundary.length() + 2;


    // Extract the file content from the body
    std::string fileContent = this->_request.body.substr(start);
	fileContent = fileContent.substr(fileContent.find("\n") + 1);
	fileContent = fileContent.substr(fileContent.find("\n") + 1);
	fileContent = fileContent.substr(fileContent.find("\n") + 1);
	fileContent = fileContent.substr(0, fileContent.length() - boundary.length() - 7);

    // Extract the filename from the body
    size_t filenameStart = this->_request.body.find("filename=\"", start) + 10;
    size_t filenameEnd = this->_request.body.find("\"", filenameStart);
    std::string filename = this->_request.body.substr(filenameStart, filenameEnd - filenameStart);
	if (!isDirectory(path))
		throw(std::runtime_error("501"));
	filename = path + filename;

	// Check if the file already exists
	std::string newFileName = filename;
	size_t dotPos = filename.find(".");
	int number = 1;
	while (fileExists(newFileName)) 
	{
		std::ostringstream oss;
		oss << number;
		if (dotPos != std::string::npos)
			newFileName = filename.substr(0, dotPos) + "_" + oss.str() + filename.substr(dotPos);
		else
			newFileName = filename + "_" + oss.str();
		++number;
	}


    // Save the file content to a file
    std::ofstream outputFile(newFileName.c_str(), std::ios::binary);
    if (outputFile) {
        outputFile.write(fileContent.c_str(), fileContent.size());
        outputFile.close();
        std::cout << "File content saved to: " << filename << std::endl;
    } else {
        std::cerr << "Error saving file." << std::endl;
		return ;
    }
	this->_response = "HTTP/1.1 201 Created\r\n"
				"Server: Phantoms\r\n"
				"Content-Type: text/html\r\n"
				"Location: https://example.com/resources/123\r\n" //TODO: change to the actual location
				"Content-Length: 51"
				"\r\nConnection: close\r\n\r\n"
				"<h1>File upload successful! Your file was received.</h1>\n";
}

void Post::handlePost()
{
    std::string path = constructPath(_server_info);
	std::cout << "post path = " << path << std::endl;
	if (this->_request_map.find("Content-Type:") == this->_request_map.end() 
		|| this->_request_map["Content-Type:"].empty())
		return ;
	if (this->_request_map["Content-Type:"][0] == "multipart/form-data;")
		Post::handleUpload(path);
	else
    {
        if (!sanitizedPath(path))
            return ;
        postBody(path);
		return ;
    }
}

bool    Post::postBody(std::string path)
{
    std::ofstream outfile(path.c_str(), std::ios::out | std::ios::trunc);
    if (outfile.fail())
        throw(std::runtime_error("500"));
    outfile << _request.body;
    outfile.close();
    _response = fillOkResponse();
    std::cout << _response << std::endl;
    return (true);
}
 
bool	Post::isDirectory(std::string path)
{
	DIR *dir;
    if ((dir  = opendir(path.c_str())) != NULL)
    {
		closedir(dir);
		std::cout << "is dir" << std::endl;
		return (true);
	}
	std::cout << "is not directory " << std::endl;
	return (false);
}

/*
HTTP/1.1 201 Created
Date: Mon, 09 Aug 2023 12:34:56 GMT
Server: ExampleServer/1.0
*/