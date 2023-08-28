#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <unistd.h>


static int simpleBackend(std::string body, std::string &received_body)
{
	char buff[4000];
    std::string pwd(getcwd(buff, sizeof(buff)));
	std::string filePath = pwd + "/intra/website/post_backend/index.html";
	std::ifstream inFile(filePath.c_str());
    if (!inFile) {
        exit (5);
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
        exit (5);
       
    }

    // Modify the content by inserting the body after the specified comment
    std::stringstream modifiedContent;
    modifiedContent << content.substr(0, pos + searchString.length());
	if (body.length() > 0 && body[0] != '\n' && body[0] != '\r')
    	modifiedContent << "\n\t\t\t<p>" << body << "</p>";
    modifiedContent << content.substr(pos + searchString.length());

    std::ofstream outFile(filePath.c_str());
    if (!outFile) {
        exit (5);
        
        return 1;
    }

    // Write the modified content back to the file
    outFile << modifiedContent.str();
    outFile.close();
	received_body = modifiedContent.str();

    return 0;
}

int main()
{
	extern char **environ;
	std::string requestMethod = std::getenv("REQUEST_METHOD");
	std::string contentType = std::getenv("HTTP_CONTENT_TYPE");

	if (requestMethod != "POST" || contentType != "application/x-www-form-urlencoded")
	{
			exit(5);
	}
	std::string input;
	std::string body;
	std::string received_body;
	while (std::getline(std::cin, input))
		body += input;
	int status = 1;
	if (body.find("comment=") != std::string::npos)
	{
		std::string comment = body.substr(body.find("comment=") + 8);
		// Remove special characters from the beginning of the comment
		while (comment[0] == ' ' || comment[0] == '\n' || comment[0] == '\r')
			comment.erase(0, 1);
		// Remove special characters from the end of the comment
		while (comment[comment.length() - 1] == ' ' || comment[comment.length() - 1] == '\n' || comment[comment.length() - 1] == '\r')
			comment.erase(comment.length() - 1, 1);
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
			if (comment[i] == '\n' || comment[i] == '\r')
				comment[i] = ' ';
		}
		status  = simpleBackend(comment, received_body);
	}
	if (status)
	{
			exit(5);
	}
	std::ostringstream ostr;
    ostr << received_body.length();
	std::string response = "HTTP/1.1 200 OK\r\n"
					"Content-Type: text/html\r\n"
					"Content-Length: " + ostr.str() 
					+ "\r\nConnection: keep-alive\r\n\r\n" + received_body;
	std::cout << response << std::endl;
	return 0;
}
