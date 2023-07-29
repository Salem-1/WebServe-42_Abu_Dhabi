#include "../includes/libs.hpp"

int main() {
    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        return 1;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        close(kq);
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_socket);
        close(kq);
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == -1) {
        perror("listen");
        close(server_socket);
        close(kq);
        return 1;
    }

    struct kevent events[MAX_EVENTS];
    struct kevent new_event;
    EV_SET(&new_event, server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);

    // Register the server socket with the kqueue
    int ret = kevent(kq, &new_event, 1, NULL, 0, NULL);
    if (ret == -1) {
        perror("kevent");
        close(server_socket);
        close(kq);
        return 1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    while (true) {
        int num_events = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (num_events == -1) {
            perror("kevent");
            close(server_socket);
            close(kq);
            return 1;
        }

        for (int i = 0; i < num_events; i++) {
            int fd = events[i].ident;

            if (fd == server_socket) {
                // Accept a new client connection
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
                if (client_socket == -1) {
                    perror("accept");
                    continue;
                }

                // Register the new client socket with the kqueue
                EV_SET(&new_event, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                ret = kevent(kq, &new_event, 1, NULL, 0, NULL);
                if (ret == -1) {
                    perror("kevent");
                    close(client_socket);
                    continue;
                }

                std::cout << "New client connected: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
            } else {
                // Handle data from a client
                RequestHandler requestHandler(fd);
                requestHandler.acceptClient();

                // Process the request
                requestHandler.processRequest();

                // Remove the client socket from the kqueue
                EV_SET(&new_event, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                ret = kevent(kq, &new_event, 1, NULL, 0, NULL);
                if (ret == -1) {
                    perror("kevent");
                }

                std::cout << "Client disconnected" << std::endl;
            }
        }
    }

    close(server_socket);
    close(kq);
    return 0;
}
