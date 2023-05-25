#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/event.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    
    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("socket");
        exit(1);
    }
    
    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);
    
    // Bind server socket to address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("bind");
        exit(1);
    }
    
    // Start listening for client connections
    if (listen(serverSocket, SOMAXCONN) == -1) {
        perror("listen");
        exit(1);
    }
    
    // Create kqueue
    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        exit(1);
    }
    
    // Add server socket to kqueue
    struct kevent event;
    EV_SET(&event, serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
        perror("kevent");
        exit(1);
    }
    
    struct kevent events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];
    
    while (1) {
        // Wait for events
        int n = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (n == -1) {
            perror("kevent");
            exit(1);
        }
        
        // Handle events
        for (int i = 0; i < n; i++) {
            int fd = events[i].ident;
            int filter = events[i].filter;
            
            if (fd == serverSocket) {
                // New client connection
                clientSocket = accept(serverSocket,
                     (struct sockaddr*)&clientAddress, &clientAddressLength);
                if (clientSocket == -1) {
                    perror("accept");
                    continue;
                }
                
                // Add client socket to kqueue
                EV_SET(&event, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
                    perror("kevent");
                    exit(1);
                }
            } else {
                // Client socket event
                ssize_t bytesRead = recv(fd, buffer, BUFFER_SIZE, 0);
                if (bytesRead == -1) {
                    perror("recv");
                    continue;
                } else if (bytesRead == 0) {
                    // Client disconnected
                    EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
                        perror("kevent");
                        exit(1);
                    }
                    close(fd);
                } else {
                    // Process client request
                    printf("Received data from client: %.*s\n", (int)bytesRead, buffer);
                    
                    // Echo response back to client
                    ssize_t bytesSent = send(fd, buffer, bytesRead, 0);
                    if (bytesSent == -1) {
                        perror("send");
                    }
                }
            }
        }
    }
    
    close(serverSocket);
    return 0;
}
