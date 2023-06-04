#include <stdio.h>
#include <poll.h>

int main()
{
    struct pollfd pfds[1];
    pfds[0].fd = 0;
    pfds[0].events =  POLLIN;
    int num_events= 0;
    printf("HIT  RETURN OR WAIT FOR 2.5 SEC FOR TIMEOUT\N");
    num_events = poll(pfds, 1, 2500);
    if (num_events == -1)
    {
        perror("poll failed");
        return (0);
    }
    if (num_events == 0)
    {
        printf("poll timed out\n");
        return (1);
    }
    int pollin_happened = pfds[0].revents & POLLIN;

    if ( pollin_happened)
    {
        printf("file descriptor opened for listening\n");
    }
    else
        printf("Unexpected erroror  occured\n");

    return (0);
}