
OSI: open system intercommunication model, standardize the communication regardless the underlying technology.
Layer 4: Ensure data transferred from one point to another reliably and without errors (ex: TCP, UDP, SPX)
            -> Provide flow control
            -> Error handling
TCP: usually use port 80,however you can use other ports if you want
Socket: Mechanism that gives prgrams access to network, allows message send
and recieving bettween application of unrelated procceses
N.B:Socket is independant from any specific type of network or IP

Steps to program TCP/IP socket:
-------------------------------
    1-Create the socket
    2-Identify the socket
    3-Wait for incoming connection on server side
    4-Send and recieve messages
    5-Close the socket

A socket is like requesting a phone line from the telephone company, it generates a phone number, then the next step is assign this phone number to you;
the phone number is the file descriptor where you can send and recieve data from other applications on the network, then assigning the phone number to you is binding step, it bind this socket to specific port.
Now you have phone number when you can reciecve or call you friends, the same here you have an open socket where you can send and recieve data on it.

# 1-Creating the socket:


    int server_fd = socket(domain, type, protocol);
    
int domain, or address family:
    is it:
          IPV4                              AF_INET
          IPv6                              AF_INET6 
          local channel, similar to pipes   AF_UNIX 
          ISO protocols                     AF_ISO 
          Xerox Network Systems protocols   AF_NS 

int type:
    type of service according to the properties required by the application:
    virtual circuit service         SOCK_STREAM  -->there is only one type of virtual circuit service, no variations
    datagran service                SOCK_DGRAM
    direct IP service               SOCK_RAW

int protocol:
    indicate use of specific protocol to support socket operation( ^ type) 
                                                                   | 
    this is useful as some families may have more than one protocol, it returnin a file descriptor.

So here is the server_fd resulted from socket creation:
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    AF_INET     ---> IPV4
    SOCK_STREAM ---> virtual circuit service
    0           --->  only one type of virtual circuit service supported,that's why we put 0 "No variation"

Implementation example:

    if (server_fd = socket(AF_INET, SOCK_STREAM, 0) < 0)
    {
        perror("cannot create a socket");
        return (0);
    }
# 2-Identify (name) a socket
    int bind(int sock, const struct sockaddr *address, socklen_t address_len)
    Naming in this context means assigning transport address to the socket (a port number in IP networking)
    This operation here called binding an address
    We do this through bind system call
    Becuase there are different type of socket address structure whe use sockeaddr struct,
    sockaddd  -->determines: address family(type of network : UNIX domain socket, ...)

    bind creates an file in the filesystem 

    int bind(
        int socket,
        const struct sockaddr *address,
        socklen_t address_len)

    socket  --> the socket that was created above
    sockaddr --> Identify address family by allowing the OS to read the first couple bytes


    for IP networking we use the following address family (struct sockaddr_in) defined in:
    # include <netinet/in.h>
   
    as follows:
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
        AF_INET : this is the address family we use to setup our socket (this is for the TCP/IP as I remember)

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
    address.sin_addr.s_addr = htonl(INADDR_ANY) //convert to ip bits
    address.sin_port = htons(PORT)

    if (bind(server_fd, (struct sockaddrs *)& address, sizeof(address)) < 0)
    {
        perror("bind fialed);
        return (0);
    }

# 3-Server side wait for incoming connections:
    Before a client connect to a server, there must be open socket waiting for connection
    listen() do this job (also a function that do system call)
    int listen(int socket, int backlog);

    int backlog: define maximum number of connection that can be quered up before connections are refused

    int accept(int socket, struct sockaddr *restrict address, socketlen_t *restricted address_len)
    the accept connection take the first connection in the queue and create socket to serve it 
    
    N.B: The original socket is set up for listening for only accepting connections NOT FOR EXCHANGIN DATA.
    By default socket operations are  blocking (synchronous) and accept will block until a connection is present on the queue 
   
    int accept(int socket, struct sockaddr *restrict address, socketlen_t *restricted address_len)
    int socket   ----> the socket set to accept the connection
    address     -----> filed with the address and the port of the client, this allow us to examine ip and port
    address_len ------> size of address as each address family has different size

Example code:

    if (listen(server_fd, 3) < 0)
    {
        perror("In lishten error");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *) & address, (socklen_t *)& addrlen)) < 0)
    {
        perror("accept() Error!");
        exit(EXIT_FAILURE);
    }

# 4 -Send and recieve messages
    At this moment we successfully connected sockets bettween clinet and server, 
    Communication is happen through simply reading and writing file descriptors

Example code:

    char *buffer[1024 = {0};

    if (read(new_socket, buffer, 1024) < 0)
        printf("No bytes to read");
    printf("%s\n", buffer);

    //this hello is very important, the work of HTTP is based on it, will tell you later why
    char *hello = "Hello from the server";
    write(new_socket, hello, strlen(hello));

# 5-Close the scoket

    this is the easiest part, just close the socket with close()

    close(new_socket);

