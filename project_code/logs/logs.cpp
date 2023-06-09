#include "../includes/libs.hpp"

void printAddrInfo( struct addrinfo *ai)
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