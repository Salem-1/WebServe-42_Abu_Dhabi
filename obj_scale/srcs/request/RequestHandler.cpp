#include "../../includes/request/RequestHandler.hpp"

RequestHandler::RequestHandler(int server_socket) : server_socket(server_socket)
{
    // Initialize other private variables if needed
}

RequestHandler::~RequestHandler()
{
    // Close the client socket if it was opened
    if (client_socket != -1) {
        close(client_socket);
    }
}

void RequestHandler::acceptClient()
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket == -1) {
        perror("accept");
        return;
    }

    // Read the request from the client socket
    char buffer[1024];
    ssize_t bytesRead = read(client_socket, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        perror("read");
        close(client_socket);
        return;
    }

    // Parse the client request
    std::string request(buffer, bytesRead);
    // requestValid = parseRequest(request);

    // Close the client socket
    close(client_socket);
}



// const std::string& RequestHandler::getRequestType() const
// {
//     return requestType;
// }

// const std::string& RequestHandler::getRequestHeader() const
// {
//     return requestHeader;
// }

// const std::string& RequestHandler::getRequestBody() const
// {
//     return requestBody;
// }

// void RequestHandler::processRequest()
// {
//     if (!requestValid) {
//         std::cout << "Invalid request, cannot process." << std::endl;
//         return;
//     }

//     // Implement your request processing logic here based on the request type
//     // For this example, let's just print the request data and send a simple response
//     std::cout << "Handling " << requestType << " request:\n"
//               << "Header: " << requestHeader << '\n'
//               << "Body: " << requestBody << std::endl;

//     std::string response = "Response for " + requestType + " request";
//     ssize_t bytesSent = write(client_socket, response.c_str(), response.size());
//     if (bytesSent == -1) {
//         perror("write");
//     }
// }
