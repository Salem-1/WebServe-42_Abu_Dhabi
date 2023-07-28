#include <sys/types.h>
#include <sys/socket.h>

int sendAll(int sock, char *buff, int *len)
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