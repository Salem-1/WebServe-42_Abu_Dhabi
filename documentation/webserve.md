# Webserve project journey documentation:
Bism Ellah Elrahman Elraheem
please start by reading getting-started-tutrial-webserve.md

# Subject pdf notes:
HTTP:  Hypertext Transfer Protocol, it's the foudnation of data communication on the world wide web
OSI: open system intercommunication model, standardize the communication regardless the underlying technology.
Layer 4: Ensure data transferred from one point to another reliably and without errors (ex: TCP, UDP, SPX)
            -> Provide flow control
            -> Error handling
User agent: web browser or crawlers
Primary function of webserver is:
    Store, process, and deliver web pages to clients, also it can recieve content frorm client like while filling the forms.

Documetation of some of the allowed functions:

socket():
    #include <sys/socket.h>

         int
         socket(int domain, int type, int protocol);

    Socket creates an endpoint for communication, think of it as if you want to communicate with your family in home country, so you need weather telephone line or mail, socket creates that telephone line or email depending on your sepecification, but this is not enough there are more steps you should do to make the acutal phone call like: assiging this phone number to your name, then dial the number of your family to call, or put sim card in your mobile to get a phone call, all those extra steps are done by other function as we are going to see below inshalla.

    int
         socket(int domain, int type, int protocol);

    int domain:
    the protocol of communication like
    PF_INET / AF_INET : for IPV4
    PF_INET6          : for IPV6

    int type:
    Defines the semantic(system) of communication 
    SOCK_STREAM : (virtual circuit servce which like elictric circuits but with logical endpoints)like phone call, reliable and ordered stream of data, ensure messages are send and gurantee integrity
    SOCK_DGRAM: like sending letters, allow you to send small pieces of information called datagram, faster and lighter than SOCK_STREAM, but ther is no gurantee the informaition will arrive or arrive in order
    SOCKET_RAW: this one provide direct access to the underlying network protocol, allows send and recieve packets at the network layer bypassing transport layer like TCP or UDP, with this type you have full control over structure like headers and payloads, it's usually used for network tasks like monotoring and sniffing
    return value: file descroptor

    int protocol:
    help you finetune the behaviour of the type
    0 is the defualt
    for example 
    SOCK_STREAM with protocol 6 for TCP and 17 for UDP

    sockets must be in connected state before any data transferred, connection happens by connect() or connectx(), transferring data happens by read() write() send() recv(), then you must close the socket using close().

    SOCK_STREAM if failed will consder as broken connection and return -1 ETIMEDOUT errono, protocols usually keeps socket warm by forcing transmission every minute (pace maker "this is my own term"), if no response for extended period like(5 mins) SIGPIPE is raised if process sent broken stream, this causes naiive proccess wich don't handle the signal to exit.

    fcntl(2) allow non blocking I/O events via SIFIO, Setdocket(2) getdocketopt(2) used to set and get socket operation, I believe this design is object oriented programming as C++

    return value is file descriptor on success or -1 on failure 



bind():
    #include <sys/types.h>
    #include <sys/socket.h>

    int bind(int sockfd, const struct sockaddr *addr,
            socklen_t     addrlen);

    binds specific address to an opened socket, which means that:
    Imagine you went to Etisalat company to buy new mobile number, the first step you choose mobile number this is the socket creation step, then you need to link this mobile number to your ID or your name, this steo is the bind() function step
    
    int bind(int sockfd, const struct sockaddr *addr,
            socklen_t     addrlen);

    int sockfd: the opened socket file descraptor as above.

    const struct sockaddr *addr: the address we are going to bind our socket to, the sockaddr declared as follows:
     //struct sockaddr_in 
    { 
        __uint8_t         sin_len; 
        sa_family_t       sin_family; 
        in_port_t         sin_port; 
        struct in_addr    sin_addr; 
        char              sin_zero[8]; 
    };
    Before calling bind we should fill 3 key parts:
    sin_family:
        AF_INET : this is the address family we use to setup our socket (this represent IPV4)

    sin_port:
        Assigning port number (the transport the address)
        if (I am server)
        {
            I will assign the port number, since clients wants to know which port number I want to be used to communicate with me
        }
        else if (I am client)
        {
            Usually I will let the operating system decide the port for me 
        }

    sin_addr:
        IP of my machine, which means my machine will have one IP for each interface
        If (I have WIFI and Ethernet, I might have two IPS)
        {
            Usually we let the system decide for my, the special used for this is 0.0.0.0
            defined by INADDR_ANY 
        }

        address_len: 
            What is the length of this address,
            we specify this becuase address structure may differ based on the address family
            it's simply:

                sizeof(struct sockaddr_in);
        Example for filling the bind() function:

            struct sockaddr_in address;
            const int PORT = 8080; // the port I will open as server allowing other to connect to me 

            memset((char *)&address, 0, sizeof(address);
            addres.sin_family = AF_INET;
            address.sin_addr.s_addr = htonl(INADDR_ANY) //convert  ip  to binary bits
            address.sin_port = htons(PORT) //convert int port to binary bits

            if (bind(server_fd, (struct sockaddrs *)& address, sizeof(address)) < 0)
            {
                perror("bind fialed);
                return (0);
            }


listen():
     
     #include <sys/socket.h>

     int
     listen(int socket, int backlog);

    listen for the comming connection, it set socket in listening state, and specify number of pending connections.
    think of it as putting the newly bought simcard in your mobile, now the your mobile number is activate ready to recieve phone calls, whenever you are in phone call and some one else call you, the second caller will be but on waiting, if third caller called you at the same time his call will be rejected, so the backlog here is 2.
    after listen comes accept() and other connection handling steps till closing the connection "aka close()";
    On most Unix-like systems, the typical maximum value is around 128 or 128-4096. (will go for 128 inshalla)

     #include <sys/socket.h>

    int
    listen(int socket, int backlog);
    
    int socket : the opened socket file descriptor
    int backlog: number of queued connection requests

    N.B: you can keep track of connections by counter, also you can increase capacity of your server to handle multiple connections by using threads, which I see as an amazing option.

example program:

    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <pthread.h>

    #define MAX_PENDING_CONNECTIONS 10

    void *connectionHandler(void *arg);

    int main() {
        int sockfd;
        struct sockaddr_in server_addr;

        // Create a socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("socket");
            return 1;
        }

        // Bind the socket to a specific address and port
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(8080);

        if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("bind");
            return 1;
        }

        // Listen for incoming connections
        if (listen(sockfd, MAX_PENDING_CONNECTIONS) == -1) {
            perror("listen");
            return 1;
        }

        printf("Socket is listening for incoming connections.\n");

        // Accept and process connections using threads
        while (1) {
            int client_sockfd;
            struct sockaddr_in client_addr;
            socklen_t client_addrlen = sizeof(client_addr);

            // Accept a new connection
            client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addrlen);
            if (client_sockfd == -1) {
                perror("accept");
                continue;
            }

            // Create a thread to handle the connection
            pthread_t tid;
            if (pthread_create(&tid, NULL, connectionHandler, (void *)&client_sockfd) != 0) {
                perror("pthread_create");
                close(client_sockfd);
                continue;
            }

            // Detach the thread to allow it to run independently
            pthread_detach(tid);
        }

        // Close the socket
        close(sockfd);

        return 0;
    }

    void *connectionHandler(void *arg)
    {
        int client_sockfd = *((int *)arg);

        // Process the connection...

        // Close the client socket
        close(client_sockfd);

        pthread_exit(NULL);
    }
accept():
    #include <sys/socket.h>

     int
     accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);

    it takes the first connection request queued in listen, create new socket for it with the same properties of the orginal socket, it's used with SOCK_STREAM

    Creates a new socket with the same properties
    if no pending queued connections accetp() will wait (block) till recieve one only if socket is set to blocking,
    else if (socket is set to non-blocking && there are no pedning connections)
        then
            accept() return (error)

    #include <sys/socket.h>
     int
     accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);

    int socket : file descriptor resulted from bind
    struct sockaddr *restrict address: address is the address decided in the communication layer, will be same address used in bind
    socklen_t *restrict address_len: sizeof(address)
Practical example use case for socket(), bind(), listen(), accept(), read() and write(a):
// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
   char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 26\n\nHello from Ahmed's server!";

    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))  < 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 2) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        sleep(5);
        close(new_socket);
    }
    return 0;
}


select():

connect():
send():
revc():
htons(), htonls(), ntohs(), ntohl():
Convert bytes bettween network byte order to host byte order
use case ---> htons(PORT)  //used for port
getaddrinfo():
freeadrinfo():
setsockopt():
getsockname():
poll():
epoll():
epoll_create():
epoll_ctl():
epoll_wait():
kqueue():
kevent():
any equivelant to pool select kqueue epoll:
fcntl():
getprotobyname():


fcntl(): for macos, only as follows fcntl(fd, F_SETFL, O_NONBLOCK);
Any defined macro like FD_SET, FD_CLR, FD_ISSET, FD_ZERO:
C++98

access():
opendir():
readdir():
closedir():
execve():
dup():
dup2():
pipe():
strerror():
gai_strerror():
errno():
fork():
close():
read():
write():
waitpid():
kill():
signal():

RFC:
telnet:
NGINX:
CGI:

# Requirments:
1-Take config file as argument or use default path
2-Never block client, bounce back properly if necessary
3-Non blocking and using 1 poll() or equivelent for all the I/O oeprations(listen included)
    -poll() or equivelent must check read and write at the same time
    -Any read or write or send or recieve must go through poll()
    -Don't use poll or equivelent before reading the configuration file
    Use file descriptor in non blocking mode
4-Request for our server should never hang forever
5-server should be compatible with browser of our choice
6-Compare webserve to NGINX HTTP/1.1
7-HTTP status code must be accurate
8-Must have default error page if none provided
9-fork() used only in CGI(PHP, python, bash..etc)
10-serve fully static website
11-Client must be able to upload files
->Implement at least:
    12-GET
    13-POST
    14-DELETE
15-Server must be available at all cost even in stress tests
16-Server must be able to listen to multiple ports
---------------------------
in the configuration file:
17-Choose host port and host for each server
18-setup server_names or not
19-the first server for host:port will be the default for this host:port(it will answer all the requests that don't belong to another server)
20-Limit client body size
21-Setup default error page
22-setup routes with one or multiple of the following (no regix):
    23-define list of accepted HTTP methods
    24-define HTTP redierction
    25-define directory or a path where a file should be searched(for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet)
    26-turn off directory listing
    27-Set a defualt file to answer if request directory
    28-Execute CGI based on certain file extention
    29-Make it work with GET and POST
    30-Make the route accept uploading file and configure where should be saved
--------------------
CGI
31-use full path as PATH_INFO
32-for chunked request serrver has to unchunck it and expect EOF 
33-Call CGI with the file requested as first argument
34-CGI myst run in the correct directory for relative path file access
35-Serve should work with one CGI (Python, php-CGI...)
36-must provide some configuration and defualt basic files to test and demonstrate every feature during the 
# Bonus inshalla
37-Support cookies and session managment
38-Handle multiple CGI

evaluation
questions:
what is poll()?
what is telnet?
can I do man in the middle with telnet/myserver?
what is CGI?
what is the meaning of session managment?
what are coockies?
how my server can handle cookies?