#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fds[MAX_CLIENTS];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Set server address parameters
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind the socket to the server address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to bind");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    // Initialize client file descriptor array
    memset(client_fds, 0, sizeof(client_fds));

    while (1) {
        struct pollfd fds[MAX_CLIENTS + 1]; // Add one for the server socket
        int i, activity;

        // Set up the server socket for polling
        fds[0].fd = server_fd;
        fds[0].events = POLLIN;

        // Set up client sockets for polling
        for (i = 1; i <= MAX_CLIENTS; i++) {
            fds[i].fd = client_fds[i - 1];
            fds[i].events = POLLIN;
        }

        // Wait for activity on any of the sockets
        activity = poll(fds, MAX_CLIENTS + 1, -1);
        if (activity == -1) {
            perror("Polling error");
            exit(EXIT_FAILURE);
        }

        // Check for activity on the server socket
        if (fds[0].revents & POLLIN) {
            // Accept a new client connection
            int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
            if (client_fd == -1) {
                perror("Failed to accept connection");
                exit(EXIT_FAILURE);
            }

            // Add the new client socket to the array
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_fds[i] == 0) {
                    client_fds[i] = client_fd;
                    break;
                }
            }
        }

        // Check for activity on client sockets
        for (i = 1; i <= MAX_CLIENTS; i++) {
            if (fds[i].revents & POLLIN) {
                // Receive data from the client
                memset(buffer, 0, BUFFER_SIZE);
                ssize_t bytes_received = recv(fds[i].fd, buffer, BUFFER_SIZE - 1, 0);
                if (bytes_received == -1) {
                    perror("Failed to receive data");
                    exit(EXIT_FAILURE);
                } else if (bytes_received == 0) {
                    // Connection closed by the client
                    close(fds[i].fd);
                    client_fds[i - 1] = 0;
                    continue;
                }

                // Process the received data
                printf("Received from client: %s\n", buffer);

                // Send a response back to the client
                const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
                ssize_t bytes_sent = send(fds[i].fd, response, strlen(response), 0);
                if (bytes_sent == -1) {
                    perror("Failed to send response");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    // Close server socket
    close(server_fd);

    return 0;
}
