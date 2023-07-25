#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>

#define PORT 4444
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int client_socket;
    char *response;
    size_t response_len;
} ClientData;

void handle_request(int client_socket) {
    char request[BUFFER_SIZE];

    memset(request, 0, BUFFER_SIZE);

    read(client_socket, request, sizeof(request));
    printf("Received request:\n%s\n", request);
}

int main() {
    char response[BUFFER_SIZE];
    memset(response, 0, BUFFER_SIZE);
    snprintf(response, BUFFER_SIZE,
             "HTTP/1.1 200 OK\r\n"
             "Content-Length: %lu\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "<html><body><h1>Hello, World!</h1></body></html>",
             strlen("<html><body><h1>Hello, World!</h1></body></html>")
    );

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int kq;
    struct kevent events[MAX_EVENTS];
    ClientData clients[MAX_EVENTS]; // Store data for each client socket

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Create a new kqueue instance
    kq = kqueue();
    if (kq == -1) {
        perror("kqueue creation failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Initialize the event filter for monitoring server_socket for incoming connections
    struct kevent event;
    EV_SET(&event, server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);

    // Register the event with kqueue
    int ret = kevent(kq, &event, 1, NULL, 0, NULL);
    if (ret == -1) {
        perror("kevent registration failed");
        close(kq);
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    while (1) {
        int event_count = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (event_count == -1) {
            perror("kevent error");
            break;
        }

        for (int i = 0; i < event_count; ++i) {
            int fd = events[i].ident;
            int filter = events[i].filter;

            if (fd == server_socket) {
                // New connection arrived on the server socket, accept it
                client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
                if (client_socket < 0) {
                    perror("Accept failed");
                    continue;
                }

                // Add the new client socket to the kqueue for monitoring read and write events
                struct kevent event;
                EV_SET(&event, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                ret = kevent(kq, &event, 1, NULL, 0, NULL);
                if (ret == -1) {
                    perror("kevent registration failed");
                    close(client_socket);
                    continue;
                }

                // Store client data
                clients[i].client_socket = client_socket;
                clients[i].response = response;
                clients[i].response_len = strlen(response);
            } else {
                if (filter == EVFILT_READ) {
                    // Existing client socket has data ready to read
                    handle_request(fd);

                    // Stop monitoring for read events
                    struct kevent event;
                    EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    ret = kevent(kq, &event, 1, NULL, 0, NULL);
                    if (ret == -1) {
                        perror("kevent deregistration failed");
                    }

                    // Start monitoring for write events
                    EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
                    ret = kevent(kq, &event, 1, NULL, 0, NULL);
                    if (ret == -1) {
                        perror("kevent registration failed for write");
                        close(fd);
                    }
                } else if (filter == EVFILT_WRITE) {
                    // Existing client socket is ready for writing
                    ssize_t bytes_sent = write(fd, clients[i].response, clients[i].response_len);
                    if (bytes_sent == -1) {
                        perror("Error writing to client");
                        close(fd);
                    } else if (bytes_sent < clients[i].response_len) {
                        // Not all data was sent, continue monitoring for write events
                        struct kevent event;
                        EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
                        ret = kevent(kq, &event, 1, NULL, 0, NULL);
                        if (ret == -1) {
                            perror("kevent re-registration failed for write");
                            close(fd);
                        }
                    } else {
                        // All data sent, close the client socket
                        close(fd);
                    }
                }
            }
        }
    }

    // Close the kqueue and server socket
    close(kq);
    close(server_socket);

    return 0;
}
