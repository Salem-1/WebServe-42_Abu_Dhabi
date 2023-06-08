#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h> 
#include <netdb.h>

int main()
{
char hostname[128];
gethostname(hostname, sizeof hostname); 
printf("My hostname: %s\n", hostname);
}
