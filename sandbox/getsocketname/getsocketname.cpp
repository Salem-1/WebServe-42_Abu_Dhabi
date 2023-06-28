#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <net/if_dl.h>
#include <unistd.h>
void printSockNameInfo(int sockfd) {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Get the local address that the socket is bound to
    if (getsockname(sockfd, (struct sockaddr*)&addr, &addr_len) == -1) {
        perror("getsockname");
        exit(1);
    }

    // Convert the IP address to a human-readable string
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip_str, INET_ADDRSTRLEN);

    // Print the local address information
    printf("Socket Local Address: %s\n", ip_str);
    printf("Socket Local Port: %d\n", ntohs(addr.sin_port));





    // Get the MAC address
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(1);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_LINK)
            continue;

        if (strcmp(ifa->ifa_name, "en0") == 0) {  // Replace "en0" with your interface name
            struct sockaddr_dl *sdl = (struct sockaddr_dl*)ifa->ifa_addr;
            unsigned char mac[6];
            memcpy(mac, LLADDR(sdl), 6);
            printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            break;
        }
    }

    freeifaddrs(ifaddr);
}



int main() {
    int sockfd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Bind the socket to a specific address and port
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Get the local address that the socket is bound to
    printSockNameInfo(sockfd);
    // if (getsockname(sockfd, (struct sockaddr*)&addr, &addr_len) == -1) {
    //     perror("getsockname");
    //     exit(1);
    // }

    // // Convert the IP address to a human-readable string
    // char ip_str[INET_ADDRSTRLEN];
    // inet_ntop(AF_INET, &(addr.sin_addr), ip_str, INET_ADDRSTRLEN);

    // // Print the local address information
    // printf("Server is bound to %s:%d\n", ip_str, ntohs(addr.sin_port));

    // Close the socket
    close(sockfd);

    return 0;
}
