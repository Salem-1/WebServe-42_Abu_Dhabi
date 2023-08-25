#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <regex>
extern char** environ;
//use examle
//> ./ft_dig http intra.42.fr
void    printAddrInfo(const struct addrinfo *ai) ;
void    resolve_dns(std::pair<std::string, std::string> dns);
void    resolve_protcol_dns_vars(std::pair<std::string, std::string> &dns);
void    print_html_headers();
void    print_html_footer();
int main() {
    std::pair<std::string, std::string> dns;
    resolve_protcol_dns_vars(dns);
    resolve_dns(dns);
    std::cout << "HTTP/1.1 200 OK\r\n";
	std::cout << "Content-Type: text/html\r\n";
	std::cout << "Content-Length: 2180" ;
	std::cout << "\r\nConnection: keep-alive\r\n\r\n";
    print_html_headers();
    
    std::cout << " <h1>DDOS on " << dns.second << " report</h1>" << std::endl;
    std::cout << " <p>on www.42.intra.fr server is down, you will go to jail⚖️.</p>" << std::endl;
    std::cout << " <p>Here is the attack logs</p>" << std::endl;
    std::cout << " <div class=\"terminal\">" << std::endl;
    std::cout << "   <pre>" << std::endl;
    std::cout << " $ ./launch_ddos $DNS $PROTOCOL  | ./take_server_down &       " << std::endl;        
    std::cout << " $cat attack_logs | head -5 | awk 'print {$1 \\n$2\\n$3\\n$4}'   " << std::endl; 
    std::cout << std::endl;                    
    for (int i = 0 ; i < 5 ; i++)
    {
       std::cout << "[2023-08-25 12:34:56] 192.168.1.101 - GET /page1.html HTTP/1.1   -" << std::endl;
       std::cout <<  "200 1234 " << dns.second   << " Mozilla/5.0 (Windows NT " << std::endl;
       std::cout << "10.0; Win64; x64) .AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123";
       std::cout << " 456.78.9 Safari/537.36  " << std::endl;
    }
    print_html_footer();
    return 0;
}

void    print_html_headers()
{
    std::cout <<"<!DOCTYPE html> " << std::endl;
    std::cout <<"<html> " << std::endl;
    std::cout <<"<head> " << std::endl;
    std::cout <<"  <title>DDOS attack report</title> " << std::endl;
    std::cout <<"  <meta charset=\"UTF-8\"> " << std::endl;
    std::cout <<"  <style> " << std::endl;
    std::cout <<"    body { " << std::endl;
    std::cout <<"      background-color: #000; " << std::endl;
    std::cout <<"      font-family: 'Courier New', monospace; " << std::endl;
    std::cout <<"      margin: 0; " << std::endl;
    std::cout <<"      display: flex; " << std::endl;
    std::cout <<"      align-items: center; " << std::endl;
    std::cout <<"      justify-content: center; " << std::endl;
    std::cout <<"      height: 100vh; " << std::endl;
    std::cout <<"    } " << std::endl;
    std::cout <<" " << std::endl;
    std::cout <<"    .terminal-container { " << std::endl;
    std::cout <<"      text-align: center; " << std::endl;
    std::cout <<"    } " << std::endl;
    std::cout <<" " << std::endl;
    std::cout <<"    .terminal { " << std::endl;
    std::cout <<"      width: 500px; " << std::endl;
    std::cout <<"      background-color: #333; " << std::endl;
    std::cout <<"      border-radius: 10px; " << std::endl;
    std::cout <<"      padding: 20px; " << std::endl;
    std::cout <<"      box-shadow: 0 0 20px rgba(0, 0, 0, 0.6); " << std::endl;
    std::cout <<"      color: #fff; " << std::endl;
    std::cout <<"      overflow: auto; " << std::endl;
    std::cout <<"    } " << std::endl;
    std::cout <<"    h1{ " << std::endl;
    std::cout <<"        color: #fff; " << std::endl;
    std::cout <<"    } " << std::endl;
    std::cout <<"    p{ " << std::endl;
    std::cout <<"        color: #fff; " << std::endl;
    std::cout <<"    } " << std::endl;
    std::cout <<"  </style> " << std::endl;
    std::cout <<" </head> " << std::endl;
    std::cout <<" <body> " << std::endl;
    std::cout <<"  <div class=\"terminal-container\"> " << std::endl;
}
                                                        

void    print_html_footer()
{
    std::cout << "      </pre> " << std::endl;
    std::cout << "    </div> " << std::endl;
    std::cout << "  </div> " << std::endl;
    std::cout << "</body> " << std::endl;
    std::cout << "</html> " << std::endl;
}


void    resolve_protcol_dns_vars(std::pair<std::string, std::string> &dns_pair)
{
     std::string input;
     std::string cpy ;
     std::string protocol;
     std::string dns;
    while (std::getline(std::cin, input)) {
        cpy += input;
        // std::cout << "" << input << std::endl;
    }
    if (cpy.find('=') == std::string::npos || (cpy.find(':') == std::string::npos 
        && cpy.find("%3A%2F%2F") == std::string::npos))
    {
        // std::cout <<"Bad url" << std::endl;
        // std::cout <<"501" << std::endl;
        exit(1);
    }
    std::string site = cpy.substr(cpy.find('=') + 1, cpy.length() - 1);
    // std::cout << "site:" << site << std::endl;
    if (cpy.find(':') == std::string::npos )
        protocol = site.substr(0, site.find("%3A%2F%2F"));
    else
        protocol = site.substr(0, site.find(":"));
    // std::cout << "Protocol:" << protocol << std::endl;
    if (site.find("w") == std::string::npos)
        dns = site.substr(site.find("%2F") + 3, site.length() - 1);
    else
        dns = site.substr(site.find("w"), site.length() - 1);
    // std::cout << "DNS:" << dns << std::endl;

    dns_pair.first = protocol;
    dns_pair.second = dns;
}

void    resolve_dns(std::pair<std::string, std::string> dns)
{
  

    // for (int i = 0; environ[i] != nullptr; ++i) {
        // std::cout << environ[i] << std::endl;
    // }

    // std::cout << "------------------------" << std::endl;
    struct addrinfo hints, *result, *rp;
    int status;
    // Clear the hints structure
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // Use TCP

    // Resolve the address information
    status = getaddrinfo(dns.second.c_str() , dns.first.c_str(), &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        // exit(1);
    }

    // Iterate through the linked list of addresses
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        // Access the address information in the current struct addrinfo
        // Perform network operations with the retrieved address

        // Example: Print the IP address as a string
        char ipstr[INET6_ADDRSTRLEN];
        void *addr;
        if (rp->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            addr = &(ipv4->sin_addr);
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)rp->ai_addr;
            addr = &(ipv6->sin6_addr);
        }
        inet_ntop(rp->ai_family, addr, ipstr, sizeof(ipstr));
        printf("\n---------------------------------------------\n");
        printAddrInfo(rp);

    }

    // Free the memory allocated by getaddrinfo
    freeaddrinfo(result);
}


void printAddrInfo(const struct addrinfo *ai) {
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

