# Webserve project journey documentation
*Bism Ellah Elrahman Elraheem*

Please start by reading [getting-started-tutrial-webserve.md](getting-started-tutrial-webserve.md)

## Subject pdf notes
- **HTTP**: Hypertext Transfer Protocol, it's the foundation of data communication on the world wide web
- **OSI**: Open System Intercommunication model, standardize the communication regardless of the underlying technology.
- **Layer 4**: Ensure data transferred from one point to another reliably and without errors (ex: TCP, UDP, SPX)
  - Provide flow control
  - Error handling
- **User agent**: Web browser or crawlers

The primary function of a web server is to store, process, and deliver web pages to clients. It can also receive content from clients, like while filling out forms.

Documentation of some of the allowed functions:

**socket():**

    
    #include <sys/socket.h>

    int socket(int domain, int type, int protocol);


socket() creates an endpoint for communication. Think of it as if you want to communicate with your family in your home country, so you need either a telephone line or mail. Socket creates that telephone line or email depending on your specification. But this is not enough; there are more steps you should do to make the actual phone call, like assigning this phone number to your name, then dialing the number of your family to call, or putting a SIM card in your mobile to get a phone call. All those extra steps are done by other functions as we are going to see below, inshaAllah.


    int domain:
    the protocol of communication like
    PF_INET / AF_INET : for IPV4
    PF_INET6          : for IPV6
    AF_UNSPEC         : for IPV4 or IPV6

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


select():\
    It took me too much time to figure out what do select, so I choosed to leave my old perception of select down here to give you chance for step by step understanding, so I will start with the old explanation, then will follow with the new one:

Old perception:
    Look we have here a lot of concept to digest to understand select(), so please be patient with me.
    Think of select as traffic police man, it manage the traffic, who should go? who should wait, let the pedistrian stand in line to cross the street, and so forth to keep the traffic smooth and avoid blockage of the street.
    Select do the heavy lifting when you are working with many file descriptors, as some might be opened for reading, other for writing, as you might be recieving message from client or have new opened connection, or you are sending a response,
    so select() help you manage those file descriptors.
    select() has it's own helper macros that facilitate it's work,
    so let's start the heavy lifting:
    How do select save file descriptors?
    it saves and manages them in something called file descriptor set, here the sytntax:
    
        //declare file descriptor set
        fd_set      read_fd_set, write_fd_set, erro_fd_set;
        
        //initialize file descriptor set
        FD_ZERO(&read_fd_set);
        FD_ZERO(&write_fd_set);
        FD_ZERO(&erro_fd_set);
        
        //adding file descriptor to the file descriptor set
        FD_SET(fd, &new_set);
        
        //calling select function 'time out is how much time you would like select to take before timed out'
        select(range_of_fds_to_check
        , readfds, writefds, errorfds, timeout);
        
        //serching for fd in fd set
        FD_ISSET(search_me_fd, &read_fd_set);

        //removing fd from set
        FD_CLR(remove_me_fd, &read_fd_set);

    Well this was a lot to know in one shot, however as you know about fd sets you are grown enough to read the select() documentation:
        #include <sys/select.h>

     void 
     FD_CLR(fd, fd_set *fdset);

     void
     FD_COPY(fd_set *fdset_orig, fd_set *fdset_copy);

     int
     FD_ISSET(fd, fd_set *fdset);

     void
     FD_SET(fd, fd_set *fdset);

     void
     FD_ZERO(fd_set *fdset);

     int
     select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds,
         struct timeval *restrict timeout);
    
    Noe you know the staff above, you are grown up enough to read the man, type in the terminal man select, if you understand what they are talking about please explain to me because I don't yet, otherwise, let's try to breakdown what I we understand here:

    if you need to use fds bigger than FD_SETSIZE compile with\
    
        c++ -D_DARWIN_UNLIMITED_SELECT. 
    or

        #define FD_SETSIZE before including 
        #include <sys/types.h>

New understanding:
    Think of slecet() as follows:
        Imagine you are a Chief in famous resturant that serve 1000 customer per/day, you are passionate about coocking but you having trooubles track which meal is ready to go to the oven, which meal is ready to get out of the oven, which meal is burnt and should be thrown.
        So you hire someone let's call him select(), mr select his job is to solve your meal organizing problem and let you focus on preparing meals.
        Mr select() will have 3 sets of meals, one set for meals beign prepared, one set for meals inside the oven, one set for burunt meals to through, so select() will keep track of the whole 3 sets, whenever meal preparation is done he will put it on tray in front of the oven to let you put it inside the oven,
        he will keep track of the meals inside the oven, if any meal is ready he will teake it outside the oven and put it in tray of ready meals,
        also select() will keep track of errored meals and through it in trash.
        Now you can spend happy time serving your customers and focus on what matters.

        So slecet will take fd_sets as above, will keep watching for the ready file descriptros, then will modify the set to keep the ready file descriptors only.
        Select is will wait till file descriptor change, you can wait forever or wait for the timeout as you like by setting the timeout struct.

        for the use and declaration see above.


connect():\
    Typically used by client to connect to server.
    how?
    Step 1:
    create socket for you as client.
    step 2:
    fill address with server ip and port.
    step 3:
    call conect() using your socket and address as filled above
    step 4:
    Enjoy the connection with server you can send and recieve messages with the server.

    PSIS
     #include <sys/types.h>
     #include <sys/socket.h>

     int
     connect(int socket, const struct sockaddr *address, socklen_t address_len);
    
    See example use case with send() below. 


send():\
    Used to send message to another socket, it only works when it's connected, the address already set in the address in connect();
N.B:sendto(), sendmsg() can use to send at any time but not allowed to use, sendto() takes the address of the server.
N.B2:send() can send less than the message you give however it will return number of bytes sent, so you can handle to send the unsent bytes
    Delcaration:
N.B3: the difference bettween send() and write is the flag in send that can be set to non block, also send is more portable
N.B: if send() or recv() return is 0 it means that the connection is closed

    ssize_t
    send(int socket, const void *buffer, size_t length, int flags);

Send can cuase SIGPIPE if the recieving end closed the connection to handle it by MSG_NOSIGNAL
logic to handle send incompletness:

    #include <sys/types.h>
    #include <sys/socket.h>

    int send_all(int sock, char *buff, int *len)
    {
        int total = 0;
        int bytes_left = *len;
        int n;
        while(total < bytes_left)
        {
            n = send(sock, buff + total, bytes_left, 0);
            if (n == -1)
                break ;
            bytes_left -= n;
            total += n;
        }
        *len = total; //keep track of number of sent bytes
        return (n); //will return -1 on failure or 0 on success
    }
ssize_t: is singned size_t
buffer:message to send
flags:
     #define MSG_OOB        0x1  /* process out-of-band data */ will use highr priority packets
    #define MSG_DONTROUTE  0x4  /* bypass routing, use direct interface */ Diagnostic,
    will use NULL as I believe
return 0 on success or -1 on failure 
Example use case:

    #include <stdio.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <netinet/in.h>
    #include <string.h>
    #include <arpa/inet.h>
    #include <readline/readline.h>
    #include <readline/history.h>

    #define PORT 8080

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
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) // pton ---> presentation to network, to convert the ip other way around use function inet_ntop() which means network to presentation
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
            send(sock, hello, strlen(hello), 0);
            valread = read(sock, buffer, 1024);
            printf("%s\n", buffer);
            close (sock);
        }
        return (0);
    }

recv():\
     #include <sys/socket.h>

     ssize_t
     recv(int socket, void *buffer, size_t length, int flags);
     same as above recieve a message from a connected socket;
flag:

           MSG_OOB        process out-of-band data // recieve some data out of normal data stream, used by some protocols
           MSG_PEEK       peek at incoming message//keep the old buffer
           MSG_WAITALL    wait for full request or error// recieve the whole message, some portions of the message might be lost if program or message is intruppted

htons, htonl, ntohs, ntohl,:\
Convert bytes bettween network byte order to host byte order
Why do we need this?
    Because chips can differ some has littele endian like Intel chips, other can have big endian like motorolla, so the those functions saves you the hassle of the conversions

     uint16_t
     htons(uint16_t hostshort); // host to network short

    uint32_t
    htonl(uint32_t hostlong); // host to network long
    htons(PORT);

    uint16_t
     ntohs(uint16_t netshort); // network to host short

    uint32_t
    ntohl(uint32_t netlong); // network to host long


getaddrinfo():\
    
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    int
    getaddrinfo(const char *hostname, const char *servname, const struct addrinfo *hints, struct addrinfo **res);
    
hostneme: domain name or ip as string
    server name : port or service name 
    hints: provide some useful hints for getaddrinfo to bring you the required address data
    res:linked list of addrinfo struct that has the required information about your ip
    
gets information about specific domain name or IP, those information you can fill up the struct addr_in address in bind and connect, or you can use it in enumeration like dig command.


freeadrinfo():\
    free the res addrinfor void
     freeaddrinfo(struct addrinfo *ai);

    Example use case for getaddrinfo() and freeaddrinfo() on this repo :
    https://github.com/Salem-1/WebServe_42_Abu_Dhabi/tree/main/sandbox/dig_addresses

setsockopt():\
     int
     setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);

    //to avoid being bind engaged by the system
    int yes=1;
    //char yes='1'; // Solaris people use this
    // lose the pesky "Address already in use" error message
    if (setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) { perror("setsockopt");
    exit(1);
    }

    Allow us to set specific options for our socket like:
    
    SO_REUSEADDR: Allows reusing the local address, which is useful for binding to a port that may still have connections in the TIME_WAIT state.
    
    SO_KEEPALIVE: Enables sending keep-alive messages on the socket to detect if the connection is still alive.
    
    SO_RCVBUF and SO_SNDBUF: Set the receive and send buffer sizes for the socket, respectively.
    
    TCP_NODELAY: Disables Nagle's algorithm, which can improve the performance of certain applications.

         int
     setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);

         sockfd: The file descriptor of the socket on which the option should be set.
        level: The protocol level at which the option resides. It specifies the protocol family of the socket.
        optname: The specific option to set.
        optval: A pointer to the value to be set for the option.
        optlen: The size of the option value.
    if you want to know more about it do your own research, for now it doesn't seem a good use as I didn't start the project yet
getsockname():\
    #include <sys/socket.h>

    int
    getsockname(int socket, struct sockaddr *restrict address, socklen_t *restrict  address_len);
    
retrive the current address information linked to specific socket.


poll():\

     #include <poll.h>

     int
     poll(struct pollfd fds[], nfds_t nfds, int timeout);
    
Do the same job as select, managing file descriptors in read and write.
    Imagine poll as if you have a call center, and you want whoever call to be recieved so you need a device to manage the incoming and outcoming calls from the same call center number, poll do this job

     #include <poll.h>

     int
     poll(struct pollfd fds[], nfds_t nfds, int timeout);
    fds: the struc that holds fds and events
    struct pollfd {
         int    fd;       /* file descriptor */
         short  events;   /* events to look for you set this one, for example for read PULLIN flag used */
         short  revents;  /* events returned , you will check this one if event is triggered*/
     };

     nfds: number of tracked file descriptors

     timeout: -1 to wait forever, 0 to be non-blocking no wait, > 0 if you need set specific waiting time in millisecond

     example use cases:
     https://github.com/Salem-1/WebServe_42_Abu_Dhabi/tree/main/sandbox/try_poll/*

     N.B:poll require much less effort from you as programmer to keep track of fds than select, so will go with it inshalla, also to get myself familiar with event programming

epoll():\
epoll_create():\
epoll_ctl():\
epoll_wait():\
    like poll but with diffrent macros and syntax, it's highly scalable for web servers but unfortunately works on linux systems only

    #include <sys/epoll.h>

Another alternative to poll() and selecet(), it' more scalable and provide more control over handling events, however it's non portable, which means that it work only on Linux kernel

#include <sys/epoll.h>

int epoll_create(int size); //create epoll instance
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event); //control epoll event
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout); //wait for epoll event

use case (unfotunately only works on linux):
     https://github.com/Salem-1/WebServe_42_Abu_Dhabi/tree/main/sandbox/try_epoll/*
N.B: will not dig deep in epoll now, unless decided to use it in any reason.
kqueue():\
    Think of kqueue as if you are running busy resturant and you want to keep track of incoming orders and finished orders, so you bought new device that uses tokens and has screen, you tell the device which order to watch for and what event to watch for(incoming order or outgoing order), then the device will notify you on the screen whenever you recieve an order or an order was finished, so you can take the appropriate action.

Declarations:

    #include <sys/event.h>
    #include <sys/types.h>
    #include <sys/time.h>
    #define MAX_EVENTS = 10 //example for number of events you want to track
    int                 kq;
    struct kevent       event;
    struct kevent       events[MAX_EVENTS];

    int kqueue(void);
    //initialize kqueue, reuturn -1 on failure

    EV_SET(&kev, ident, filter, flags, fflags, data, udata);
    //macro to initialize event structure event
    
     int
     kevent(int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout);

     int
     kevent64(int kq, const struct kevent64_s *changelist, int nchanges, struct kevent64_s *eventlist, int nevents, unsigned int flags, const struct timespec *timeout);

     int
     kevent_qos(int kq, const struct kevent_qos_s *changelist, int nchanges, struct kevent_qos_s *eventlist, int nevents, void *data_out, size_t *data_available, unsigned int flags);


     EV_SET64(&kev, ident, filter, flags, fflags, data, udata, ext[0], ext[1]);

     EV_SET_QOS(&kev, ident, filter, flags, qos, udata, fflags, xflags, data, ext[0], ext[1], ext[2], ext[3]);

kqueue steps:
    1-create kqueue object
    Think of it as if you are setting up the orders tracking system in your resutrant
    
        int kq = kqueue();
        //create new kqueue to keep track of kernel event that you want, if fials return -1; 
    
    2-attch file/socket descriptor
    Think of it as making usb flash memory with the required setup for your  resturant new device stating kind of events you want to keep track of like incoming orders.
    Then you put the this usb flash memory in your machine to be ready for your morning shift orders
     
    EV_SET(&event, server_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    //initilizing event struct using EV_SET macro
    // &envent      : the address of event struct you want to keep track of 
    //server_socket : the file descriptor you want to monitor for the event
    //EVFILT        : event filter for the type of event to watch for, here is read
    //EV_ADD        : the action we want to take with this file descriptor, here we need to add it to the event
    //0             : the flag a

    kevent(kq, &event, 1, NULL, 0, NULL);
    //registering the event in event queue
    kq     : file descriptor representing event queue
    &event : address of the event structure we want to register
    1      : number of the envents in the change list 
    NULL   : pointer to the event structure where the triggered event will be stored(not used in this example)
    0      : Number of events we want to retrieve not used here
    NULL   : time out of the function (not used here) 
    3-wiat for incoming event from kqueue

    n = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
    //waiting for incoming connections, will recieve it on the events sturct, it returns number of events n , then will through it to handle it.

    fd  = events[i].ident;
    //assing fd to event struct
    filter = events[i].filter
    //assign the event to watch for like read ofr rwrite , use the corressponding macro

    4-destroy kqueue object
    //deleting socket from event watch list
    EV_SET(&event, client_socket, 
            EVFILT_READ, EV_DELETE, 0, 0, NULL);
    //using the macro of EV_SET() explicitly state that will delete specific fd using EV_DELETE that was monitored for EVFILT_READ that is watched read

    //then will call kevent to add this event to the kq "kernel event"
        if(kevent(kq, &event, 1, NULL, 0, NULL) == -1)
        {
            perror("kevent failed in disconencting client");
            exit(1);
        }
        close(fd);

kevent():\
any equivelant to pool select kqueue epoll:\
fcntl():\
    file control, can be used to set to socket to non blocking as requested by the subject.
      #include <fcntl.h>

     int
     fcntl(int fildes, int cmd, ...);

Example use case:

    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <fcntl.h>
    
    int main() {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("socket");
            exit(1);
        }
    
        // Set the socket to non-blocking mode
        int flags = fcntl(sockfd, F_GETFL, 0);
        if (flags < 0) {
            perror("fcntl");
            exit(1);
        }
        if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
            perror("fcntl");
            exit(1);
        }
    
        printf("Socket set to non-blocking mode\n");
    
        // Further code...
    
        return 0;
    }

getprotobyname():\

    #include <netdb.h>
    struct protoent *getprotobyname(const char *name);

Used to get the protocol number, for example "tcp" number is 6.
    why to use?\
    To make the code more readable and maintainable.

    #include <netdb.h>
    struct protoent *getprotobyname(const char *name);

name: protocol name like "tcp" , "udp"

struct  protoent {
        char    *p_name;        /* official name of protocol */
        char    **p_aliases;    /* alias NULL terminated list, means those are other names of the protocol, if your name is ahmed, your alias list is {"ahmed", "hamada", "abo hmied", NULL}*/
        int     p_proto;        /* protocol number which we need to use for our webserve*/
};


Example:

    #include <netdb.h>
    #include <stdio.h>

    int main() {
        struct protoent *proto = getprotobyname("tcp");
        if (proto != NULL) {
            printf("Protocol name: %s\n", proto->p_name);
            printf("Protocol aliases:\n");
            char **alias = proto->p_aliases;
            while (*alias != NULL) {
                printf("- %s\n", *alias);
                alias++;
            }
            printf("Protocol number: %d\n", proto->p_proto);
        } else {
            perror("getprotobyname");
        }
        return (0);
    }

Real Example use case in web server:

    https://github.com/Salem-1/WebServe_42_Abu_Dhabi/tree/main/sandbox/try_get_protobyname/kqueue_w_get_proto_server.cpp


fcntl(): for macos, only as follows fcntl(fd, F_SETFL, O_NONBLOCK);
Any defined macro like FD_SET, FD_CLR, FD_ISSET, FD_ZERO:
C++98
std::system: 
     int
     system(const char *command);

     take command as string or char * and execute it as if you have written it in the terminal

access():\
#include <unistd.h>

int access(const char *path, int amode);
What it does?
Check the existance of a file and check the allowed permission.
how?
give:
const char *path = "/path/to/the/desired/file";
int amode = W_OK 	check the writing permission
			R_OK 	check the reading permission
			X_OK	check the execution permission, gives success if the user has privilage
			to execute the file even if the file doesn't has executino permission.  '
			F_OK 	check for the file existance
return value? 0 upon_success : else -1;
and erronum change according to the kind of the error 

opendir():
    
    //Open directory and return pointer to directory stream
    #include <dirent.h>

    DIR *
    opendir(const char *filename);

readdir():
    Used for read-directory entries used to retriev information about files in directory, retur pointer to next directory entry.\

closedir():


Example for opendir readdir closedir:\

    #include <stdio.h>
    #include <dirent.h>
    #include <unistd.h>
    int main()
    {
        DIR *dir;
        struct dirent *entry;
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd");
            return 1;
        }
        printf("cwd is %s \n", cwd);
        dir = opendir(cwd);
        if (dir == NULL)
        {
            perror("opendir");
            return 1;
        }
    
        while ((entry = readdir(dir)) != NULL)
        {
            printf("%s\n", entry->d_name);
        }
    
        closedir(dir);
        return 0;
    }

execve():\
    used to execute programs, it replaces the current process with new process:

    int execve(const char *pathname, char *const argv[], char *const envp[]);
    //se minishell

dup():\
   
    #include <unistd.h>

    int
    dup(int fildes);

Duplicate existing value of file descriptor, can be used with sockets


dup2():
    duplicate file descriptor to another file descruptir: see minishell
pipe():
    open int fd[2]; fd[1] for writing, fd[0] for reading
strerror():
    take error number and return pointer to the corresponding message string
    
gai_strerror():
     
     #include <sys/types.h>
     #include <sys/socket.h>
     #include <netdb.h>

     const char *
     gai_strerror(int ecode);
Returns message corresponding to the errorno
errno():
    global variable indicates various errors during running the program, used with strerror, or you can print the errno as int directly
fork():
    fork the parent process to child process, see minishell
close():
    close opened file descriptor
read():
    read from opened file descriptor to a buffer with defined number of bytes to read
write():
    write to file descriptor form buffer number of bytes 
waitpid():
    wait for specfic proccess "child" to exit before complete execution, and store the exit status if needed: see minishell
kill():
    send specific signal to specific process id: see minitalk
signal():
    set signal handler for specific signal


# ------------------------------------------------------------------------

# RFC:



# RFC 9112
HTTP client and server are communicating through sending messages
message format
        start-line
Header  zero or more header fields

\n empty line indicating the end of header fields

Body message (optional)

How HTTP message is paresed?
Read each header field into hash table
-> each header field in to map, 
std::map<string, string> http_message;
if the header indicates there is a body then the body is read in stream the message body length or till the connection is closed

end of start line is CR: carrige return followed by LF line feed
so it's CRLF, or sometimes LF is enough
HTTP version: must be sepecified to get a correct parsing
(the one who will take this part should do deep dive in the rfc and summurize it to us)
Request line:
METHOD single_space    request target single_space protocol version
method SP request-target SP HTTP-version

example:
    GET /api/products/123 HTTP/1.1
the breakdown below:

Method: GET POST DELETE PUT

request target are weather:

    origin-form : that has "?" in it 
        /where?q=now
    absolute-form : which gives the absolute path
         http://www.example.org/pub/WWW/TheProject.html

    
    authority-form: only used for CONNECT request and it has ":", used for tunnels in proxies, cleint send the host and the port number of the tunnel destination as request target
         www.example.com:80

    asterisk-form: only used for server OPTION method
        OPTIONS * HTTP/1.1

Don't accept obsolete line folding:
    ;
    continue rquest
instead send bad request 400
Content-Length:
    determines the message body
Message body: 
    is identical to the content unless transfer coding is applied


Transfer-Encoding: gzip, chunked

    Transfer encoding is primary intended for dynamically generated content, transfer coding is very important when the content size is not known in advance, 
    Transfer-Encoding is the property of the message unlike Content-Encoding
    Server should never send transfer encoding in 1xx 204(no content) or 2xx response to CONNECT 

N.B: should never send Content-Length in any message that has transfer encoding
Content-Length: applied for non Transfer-Encoding

Chunked Transfer Coding:
    divide file to chunks to be sent in packets

    HTTP/1.1 200 OK
    Content-Type: text/plain
    Transfer-Encoding: chunked
    
    25
    This is the first chunk of data.
    1A
    Here is the second chunk.
    0
Chunked trailer section:
    chunk can have metadata section after the chunk ends carry important information
The bottom line th chunking topic needs deep dive as it's non trivial part, it seems that chunked worked as follows line instructions and line data

N.B:HTTP messaging is independent of underlying transport or session-layer connection protocol.

HTTP implementations are expected to engage in connection management, which includes:
    -Maintaining the state of current connections
    -Establishing a new connection
    -Reusing an existing connection
    -Processing messages received on a connection
    -Detecting connection failures
    -Closing each connection


Connection: close
    mean that the server will close the connection after responding to this message
TLS:
    starts with TLS handshake then act as normal HTTP
# Response

HTTP-Version Space status_code soace [ reason-sphere ]

each field consists of case insenstive name followed by ":"






# notes from 7232 for your info only the implementations needed is in 9112 above
Statless application: means it doesn't store any data about session, HTTP doesn't keep any data of previous requests, each request is proccessed independently and the serever doesn't know about the previous request.
This statelessness make http more scalable (load balancing),better caching, but increase the payload, also rely on tokens for authentication.

URI: uniform resource identifier 

Each HTTP message is either request or response, steps for implementation:\
1-Server listen on connection for request or response\
2-Parse each message recieved\
3-interpret message sematics
4-respond to request with one or more response message

from client side:
1-Construct request message with specific intention
2-Examine recieved request to see if intentions were carried out
------
resource: target of http request
One of the goals of HTTP is to seprate what you request from what are you gonna do with it\

payload purpose defined by the request method and status code\

Payloda: is the data that is sent back purpose depends on request method and response status code \

HTTP is just a messanger that doesn't interfere how your server work or take decision or whatever algorithm you decide to implement, it's all yours./

Proactive negtiation: the user agent tell the server it's prefrences in the request itself 
unfortunately can make things slower, have privacy concerns, and might not fullfill useragent full needs due to lack of information like: is the user request page to show on screen or to print on paper

Reactive negotiation: the useragent receive a response with several options provided in the response metadata, if not satisfied with the response can send another GET request based on the useragent or user choice\
This reactive representation is just different presentation of the resource (URI: like www.example.com/imgaes/cat.jpeg), like page format or language choice, useful for laod balancing , unfortunately might increase latency if user need to do another GET request to obtain the needed representation

Distributed objects, are objects that can be accessed remotly(objects here are in sense of object oriented programmin), those obejects might be available on different address spaces or processes on the same computer, or on different computers, the sender send message to an object to do some tasks then the result is sent back to the calling object, HTTP was orginally designed to be usable interface for distriputed object system
\


Methods are case senstive.

GET: retrieve data from server 
HEAD: like GET but without body
POST: send data to the server
PUT: update data already exist
DELETE: remove specific data permenantly
CONNECT: establish connection typically used in proxies
TRACE: used for debugging allowing you to see how your data handled by intermediate servers
CONNECT: establish tunnel service for proxies
GET with body will be rejected

N.B: all general purpose servers must support GET and HEAD, all other methods are optional
# -----------------
# Request
Header breakdown:

Expect: 100-continue
The client tells the server what he expects, to optimise performance, so server cannot fullfil the request he can reply with 4xx 
usually used with large messages

Max-Forwards:
Max-Forwards = 1*DIGIT 
used with Trace to limit number of times forwarding happens by the proxy, in case trace appears to be failing or looping mid-chain
This Max-Forward will be ignored with any other request methods as our case here


if recieved something not 100-continue reply with 417 (Expectation Failed), 

5.3.1.  Quality Values:
can assgin weight bet 0.000 and 1.000 to something that I don't understand, here is the syntax
weight = OWS ";" OWS "q=" qvalue
     qvalue = ( "0" [ "." 0*3DIGIT ] )
            / ( "1" [ "." 0*3("0") ] )

Accept:
    specifies the media type the client willing to accept

     Accept: audio/*; q=0.2, audio/basic
is interpreted as "I prefer audio/basic, but send me any audio type
if it is the best available after an 80% markdown in quality".

Accept-Encoding:

Accept-Language:
    which language do you prefer

Authorization:

Proxy-Autherization:

From:

     From: webmaster@example.org
Address of human user, can be used by the machine, often sent by robotic user agent

Referer:
    Where the user came from, the page he visits that refered him here,it might reveal senstive information and used in cross site scripting.
user-Agent:
    Shouldn't reveal more information that needed, and should be honest.

Staus Code:
    1xx Infomational, request was recieved continue proccessing 
    2xx Successful, request successfully recieved, understood and accepted
    3xx Redirection, further action needed to complete the request
    4xx Client Error, the request contains bad syntax cannot be fullfilled
    5xx Server Error, server failed to fullfill an apparently vaild request

# Errors:

501 Method not implemented
405 Method not allowed
303 POST request already satisfied before, no need to implement this request
415 unsupported media type as sending .jpeg to .txt data field
# -------------------------------------------------------------------------

# telnet:
    used for testing of the webserve, especially the stress test one.

# NGINX:
Notes on the configuration file:\
Configuration file consist of:
    -Sections
    -Directives
    -Blocks
Breakdown:
1-Main context:

    user nginx;
    worker_processes auto;
    pid /var/run/nginx.pid;
2-Events:

    events {
      worker_connections 1024;
    }

Maximum number of simultanous connections that the server can handle
3-HTTP context

    http {
      # HTTP-related directives go here

      server {
        # Server block for example.com
        #the port we are listening on
        listen 80;
        #website name or ip
        server_name example.com;
        #where the root of the website files on my machine (server)
        root /var/www/example;

        #what should happen when the user visit the home page
        location / {
          # Directives for handling requests to the root location
          index index.html;
        }

        #I believe there is where it should search when visit www.example.com/api/
        location /api {
          # Directives for handling requests to the /api location
          proxy_pass http://backend_servers;
        }
      }

    #other servers that we can do load balancing with
    #will not implement this too 
      upstream backend_servers {
        server backend1.example.com;
        server backend2.example.com;
      }
    }

3-Stream and mail context will not handle
4-Include allow you to split your configuration  directive into multiple files
    include /etc/nginx/conf.d/*.conf;
    #it's like #inlude "other_header_file.h"


ngninx -s reload

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
can I do man-in-the-middle with telnet/myserver?
what is CGI?
what is the meaning of session managment?
what are coockies?
how my server can handle cookies?


--------------------------------
Hassan Sharhan and Ellen tips:
----------
Hey man, sure. So here are a few things to get you started on webserv:
 Read this RFC in full: https://www.rfc-editor.org/rfc/rfc9112.html.  There are a few others that go in more detail, but I find this one is the most succinct in giving a overview of how an http server works.  Your server should aim to be compliant with this RFC.
 Split webserv into its component objects and divide it among your team. This is how we and many others did it: https://docs.google.com/document/d/1SBlVWD4AKpSdg6GnfPASktrNkL211isQFdex2iRDtRg/edit
 For configuration, remember you don't have to exactly follow nginx.  Just stick with what the subject guide asks you to include in config.
 One thing to remember is that read/recv and write/send don't necessarily transfer all the data in one system call.  They have limits depending on which computer you run it in.  So when reading a request, you have to ensure that you got a complete request; if not, you'll need to another read when ready.  Same logic with writing/sending a response: check whether all of it was sent; if not, resend when ready.  This was something I wish I knew early on cause not accounting for this caused us to fail the intra tester for a long time.
 Now when I say "when ready", I mean when select/poll says so.  These basically tell you when a particular fd can be written to and/or read from.  Select() is easier to use but it can only handle 1024 fds max, which is fine cause macs/pcs can only handle that many.  But I would suggest to go for poll, as that's actually used in production servers and so will be more useful.  I think Alia/Ameer/Gerard used poll for ft_irc, while my group and Moatasem/Anastasia used Select.
 To pass the intra tester, you have to handle more types of request than the subject asks for, which we did.
 When you guys start, figure out who depends on who for their objects/classes to work and prioritize on creating draft .hpp files.  That way, person A who depends on Person B for an object knows exactly the structure of the object person B sends.
 There are different ways a Request's body can be encoded, which your server can decode.  But the intratester only tests an encoding type called "chunking".  We only accounted for this type of encoding, and ignored all others mentioned in the RFC, which is what everyone who's done webserv has also done.
Alright, so this is all that comes to mind so far.  If you have any questions on the request, server, or server manager objects, feel free to ask me.  For response, you can go to Bassam.  For CGI and Config, you can go to Patel.

rfc-editor.orgrfc-editor.org
RFC 9112: HTTP/1.1
The Hypertext Transfer Protocol (HTTP) is a stateless application-level
protocol for distributed, collaborative, hypertext information systems.
This document specifies the HTTP/1.1 message syntax, message parsing,
connection management, and related security concerns.


This document obsoletes portions of RFC 7230.


Nice tutorial to start with :
https://beej.us/guide/bgnet/html/

# Beej tutorial guide:

IP : think of it like your home address or hotel address (if you own one)
PORT number: think of it as room number in that hotel, so one computer that has same ip can have an open port for http and another different port for email services 
You can find more info about ports you use on your unix OS in /etc/services

//nice quote about NAT
3.4.1 Private (Or Disconnected) Networks
Lots of places have a firewall that hides the network from the rest of the world for their own protection. And often times, the firewall translates “internal” IP addresses to “external” (that everyone else in the world knows) IP addresses using a process called Network Address Translation, or NAT.

NAT used to manage the ips of pcs in local network and seprate it from public IP

10.x.x.x  ips used in fully disconnected networks or networks behind the firewall
92.168.x.x also commonly used in private network


for clients use getaddrinfo() instead of fill addresses by hand
Whenever you are hard coding filling the ip addresses use helper functions
To handle ipv6 you will just change few macros and struct names AF_INETtoAF_INET6. PF_INETtoPF_INET6. struct sockaddr_in sa; struct sockaddr_in6 sa6; sa.sin_addr.s_addr = INADDR_ANY; // use my IPv4 address sa6.sin6_addr = in6addr_any; // use my IPv6 address


    //another neat way of calling socket and bind using getadddrinfo
    struct addrinfo hints, *res; int sockfd;
    // first, load up address structs with getaddrinfo():
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    getaddrinfo(NULL, "3490", &hints, &res);
    // make a socket:
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // bind it to the port we passed in to getaddrinfo():
    bind(sockfd, res->ai_addr, res->ai_addrlen);


//for data packing and unpacking  RFC 4506