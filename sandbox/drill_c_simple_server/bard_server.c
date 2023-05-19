#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
  // Create a socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("socket");
    exit(1);
  }

  // Bind the socket to a port
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);
  server_address.sin_addr.s_addr = INADDR_ANY;
  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    perror("bind");
    exit(1);
  }

  // Listen for connections
  listen(server_socket, 5);

  // Create a set of file descriptors to monitor
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(server_socket, &readfds);

  // Loop forever, waiting for events
  while (1) {
    // Wait for an event to occur
    int nfds = select(server_socket + 1, &readfds, NULL, NULL, NULL);
    if (nfds < 0) {
      perror("select");
      exit(1);
    }

    // Check if a connection request has been received
    if (FD_ISSET(server_socket, &readfds)) {
      // Accept the connection
      int client_socket = accept(server_socket, NULL, NULL);
      if (client_socket < 0) {
        perror("accept");
        exit(1);
      }

      // Read data from the client
      char buffer[1024];
      int bytes_read = read(client_socket, buffer, sizeof(buffer));
      if (bytes_read < 0) {
        perror("read");
        exit(1);
      }

      // Print the data to the console
      printf("Received from client: %s\n", buffer);

      // Close the client socket
      close(client_socket);
    }
  }

  return 0;
}
