# Webserve project journey documentation:
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
execve():
dup():
dup2():
pipe():
strerror():
gai_strerror():
errno():
fork():
htons():
htonls():
ntohs():
ntohl():
select():
poll():
epoll():
epoll_create():
epoll_ctl():
epoll_wait():
kqueue():
kevent():
any equivelant to pool select kqueue epoll:
socket():
accept():
listen():
send():
revc():
bind():
connect():
getaddrinfo():
freeadrinfo():
setsockopt():
getsockname():
getprotobyname():
fcntl():
close():
read():
write():
waitpid():
kill():
signal():
access():
opendir():
readdir():
closedir():
fcntl(): for macos, only as follows fcntl(fd, F_SETFL, O_NONBLOCK);
Any defined macro like D_SET, FD_CLR, FD_ISSET, FD_ZERO:
C++98

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
what is CGI: