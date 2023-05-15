#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <time.h>

#define PORT 8080

void    print_time();
int main(int argc, char **argv)
{
    int sock = 0;
    long valread;
    struct sockaddr_in serv_addr;
    char *hello = NULL;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    //convert IPV4 and IPV6 to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
         if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
         {
             perror("Socket creation failed");
             exit(EXIT_FAILURE);
         }
         if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
         {
             perror("Connect failed");
             exit(EXIT_FAILURE);
         }
        print_time();
        hello = readline(">  ");
        send(sock, hello, strlen(hello), 0);
        print_time();
        printf(" > ");
        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);
        sleep(1);
    }
    return (0);
}

 // Buffer to hold the formatted time string

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