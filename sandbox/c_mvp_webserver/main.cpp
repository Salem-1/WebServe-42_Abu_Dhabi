#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <time.h>

#define PORT 8080

void    print_time();

int main(int argc, char **argv)
{
    struct sockaddr_in  address;
    int                 server_fd;
    int                 new_socket;
    long                read_bytes;
    char                *hello;
    
    hello = "Hello from the  server";
    int addrlen = sizeof(address);

    //1-create the socket
    if ( (server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("In socket failure");
        exit(EXIT_FAILURE);
    }
    
    //2-identify (bind)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //equivelant tp 0.0.0.0 (will check your machine IP and use it)
    address.sin_port = htons(PORT); // htons convert port number bytes to the appropriate bytes format that fit port

    memset(address.sin_zero, '\0', sizeof(address.sin_zero));
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("Listen failure");
        exit(EXIT_FAILURE);
    }
        print_time();
        printf("> Connection opened waiting for client
        ");
    while(1)
    {
        //3-send and recieve
        // printf("=========================Waiting for new connection==========================\n");
        hello = readline("> ");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) <0)
        {
            perror("accept failure");
            exit(EXIT_FAILURE);
        }
        char buffer[30000] = {0};
        read_bytes = read(new_socket, buffer, 30000);
        printf("%s\n", buffer);
        print_time();
        write(new_socket, hello, strlen(hello));
        // printf("==========================Hello message sent=================================\n");
        
        //5-close
        close(new_socket);
        print_time();
        printf("> ");
    }
    return (0);
}


void    print_time()
{
    time_t currentTime;
    struct tm *localTime;
    char timeBuffer[9];

    time(&currentTime); // Get the current time
    localTime = localtime(&currentTime); // Convert it to the local time
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    printf("[%s]", timeBuffer);
}