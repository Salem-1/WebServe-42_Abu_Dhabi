//only works on linux
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, epoll_fd, event_count;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    struct epoll_event event, events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];

    // Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind the socket to the server address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, SOMAXCONN) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Epoll creation failed");
        exit(EXIT_FAILURE);
    }

    // Add server socket to epoll
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("Epoll control (add) failed");
        exit(EXIT_FAILURE);
    }

    printf("Web server is running. Listening on port 8080...\n");

    while (1) {
        // Wait for events
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (event_count == -1) {
            perror("Epoll wait failed");
            exit(EXIT_FAILURE);
        }

        // Handle events
        for (int i = 0; i < event_count; i++) {
            // Handle new connection
            if (events[i].data.fd == server_fd) {
                new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
                if (new_socket == -1) {
                    perror("Accept failed");
                    exit(EXIT_FAILURE);
                }
                printf("New connection established. Client socket fd: %d\n", new_socket);

                // Add client socket to epoll
                event.events = EPOLLIN;
                event.data.fd = new_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                    perror("Epoll control (add) failed");
                    exit(EXIT_FAILURE);
                }
            }
            // Handle client data
            else {
                int client_socket = events[i].data.fd;

                // Receive data from client
                int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
                if (bytes_read == -1) {
                    perror("Receive failed");
                    exit(EXIT_FAILURE);
                }
                if (bytes_read == 0) {
                    // Connection closed by client
                    printf("Connection closed. Client socket fd: %d\n", client_socket);

                    // Remove client socket from epoll
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_socket, NULL) == -1) {
                        perror("Epoll control (delete) failed");
                        exit(EXIT_FAILURE);
                    }
                    close(client_socket);
                }
                else {
                    // Process received data (e.g., send a response)
                    // ...

                    // Send response to client
                    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
                    int bytes_sent = send(client_socket, response, strlen(response), 0);
                    if (bytes_sent == -1) {
                        perror("Send failed");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }

    // Clean up
    close(server_fd);
    close(epoll_fd);

    return 0;
}
