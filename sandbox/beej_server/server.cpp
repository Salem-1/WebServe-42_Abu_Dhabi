#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#define PORT "3490"
#define BACKLOG 10

void printAddrInfo(const struct addrinfo *ai);
void    sigchild_handler(int s)
{
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    errno = saved_errno;
}

void    *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return (&(((struct sockaddr_in*)sa)->sin_addr));
    else
        return (&(((struct sockaddr_in6*)sa)->sin6_addr));
}


int main()
{
    int                         sockfd, new_fd;
    struct  addrinfo hints,     *serverinfo, *p;
    struct  sockaddr_storage    their_addr; //client address information
    socklen_t                   sin_size;
    struct sigaction            sa;
    int                         yes = 1;
    char                        s[INET6_ADDRSTRLEN];
    int                         rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; //will use my ip

    if ((rv == getaddrinfo(NULL, PORT, &hints, &serverinfo)) != 0) // getting my address info
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return (1);
    }
    //looping through addresses result and bind to the first one 
    for (p = serverinfo; p != NULL; p = p->ai_next)
    {
        printAddrInfo(p);
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }
        //avoiding bind busy error, i believe can be used for non blocking as well
        // printf("opened sockfd = %d\n", sockfd);
        if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
        {
            perror("fcntl error");
            exit(1);
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("server: socket");
            exit(1);
        }
        printf("opened sockfd = %d\n", sockfd);
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue ;
        }
        break ;
    }
    freeaddrinfo(serverinfo);
    if (p == NULL)
    {
        fprintf(stderr, "server failed to bind\n");
        exit(1);
    }
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen error");
        exit(1);
    }
    sa.sa_handler = sigchild_handler; //reap all dead prcesses
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
    printf("server waiting for connection....\n");
    while (1)
    {
        sin_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family,  get_in_addr((struct sockaddr *)&their_addr), s, sizeof (s));
        printf("server: got connection from %s\n", s);
        if (!fork())
        {
            close(sockfd);
            if (send(new_fd, "Hello World!", 13, 0) == -1)
                perror("send failed");
            close (new_fd);
            exit (0);
        }
        close(new_fd);
    }
    return (0);
}











//debuging 
void printAddrInfo(const struct addrinfo *ai)
{
     char ip[INET6_ADDRSTRLEN];
    printf("Flags: ");
    if (ai->ai_flags & AI_PASSIVE)
        printf("AI_PASSIVE ");
    if (ai->ai_flags & AI_CANONNAME)
        printf("AI_CANONNAME ");
    if (ai->ai_flags & AI_NUMERICHOST)
        printf("AI_NUMERICHOST ");
    if (ai->ai_flags & AI_NUMERICSERV)
        printf("AI_NUMERICSERV ");
    if (ai->ai_flags & AI_V4MAPPED)
        printf("AI_V4MAPPED ");
    if (ai->ai_flags & AI_ALL)
        printf("AI_ALL ");
    if (ai->ai_flags & AI_ADDRCONFIG)
        printf("AI_ADDRCONFIG ");
    printf("\n");

    printf("Family: ");
    switch (ai->ai_family) {
        case AF_UNSPEC:
            printf("AF_UNSPEC\n");
            break;
            case AF_INET:
                printf("AF_INET\n");
                break;
            case AF_INET6:
                printf("AF_INET6\n");
                break;
            default:
                printf("Unknown Address Family\n");
                break;
        }
    
        printf("Socket Type: ");
        switch (ai->ai_socktype) {
            case SOCK_STREAM:
                printf("SOCK_STREAM\n");
                break;
            case SOCK_DGRAM:
                printf("SOCK_DGRAM\n");
                break;
            case SOCK_RAW:
                printf("SOCK_RAW\n");
                break;
            default:
                printf("Unknown Socket Type\n");
                break;
        }
    
        printf("Protocol: ");
        switch (ai->ai_protocol) {
            case IPPROTO_TCP:
                printf("IPPROTO_TCP\n");
                break;
            case IPPROTO_UDP:
                printf("IPPROTO_UDP\n");
                break;
            default:
                printf("Unknown Protocol\n");
                break;
        }
    
        printf("Address Length: %u\n", (unsigned int) ai->ai_addrlen);
    
    if (ai->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) ai->ai_addr;
            inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
            printf("IP Address: %s\n", ip);
            printf("Port Number: %d\n", ntohs(ipv4->sin_port));
        } else if (ai->ai_family == AF_INET6) {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) ai->ai_addr;
            inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, INET6_ADDRSTRLEN);
            printf("IP Address: %s\n", ip);
            printf("Port Number: %d\n", ntohs(ipv6->sin6_port));
        } else {
        printf("Unknown Address Family\n");
            printf("Unknown Address Family\n");
        }
    
        printf("Next: %p\n", ai->ai_next);
}





