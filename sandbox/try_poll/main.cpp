#include <poll.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

//      int
//      poll(struct pollfd fds[], nfds_t nfds, int timeout);

// int main()
// {
//     struct pollfd fds[2];
//     int fd[2];
//     pipe(fd);
//     fds[0].fd = fd[0];
//     fds[0].events = POLLIN; 
//     fds[1].fd = fd[1];
//     fds[1].events = POLLOUT; 
//     printf("pipe opend with %d, %d\n", fd[0], fd[1]);
//     for (int i = 0; i < 2; i++)
//     {
//         if (poll(fds, 2, 1) < 0)
//             perror("poll fialed");

//     }
//     return (0);
    
// }

void    visualize_fds(struct pollfd *fds, int i);

#include <stdio.h>
#include <poll.h>

int main() {
    struct pollfd fds[2];
    int timeout = 5000; // 5 seconds
    int fd[2] = {0};
    pipe(fd);
    // File descriptor 1
    fds[0].fd = fd[0]; // Set the file descriptor to monitor
    fds[0].events = POLLIN; // Monitor for input events

    // File descriptor 2
    fds[1].fd = fd[1]; // Set the file descriptor to monitor
    fds[1].events = POLLOUT; // Monitor for output events

    int result = poll(fds, 2, timeout);
    if (result == -1) {
        // Error occurred
        perror("poll");
    } else if (result == 0) {
        // Timeout expired
        printf("No events occurred within the timeout.\n");
    } else {
        // Events occurred on the file descriptors
        for (int i = 0; i < 2; i++) {
            if (fds[i].revents & POLLIN) {
                printf("Input event occurred on file descriptor %d\n", fds[i].fd);
            }
            if (fds[i].revents & POLLOUT) {
                printf("Output event occurred on file descriptor %d\n", fds[i].fd);
            }
            // Check for other event types as needed
        }
    }
    visualize_fds(fds, 2);
    return 0;
}

void    visualize_fds(struct pollfd *fds, int i)
{
    while (i > 0)
    {
        printf("fd = %d\n", fds[i - 1].fd);
        printf("events = %d\n", fds[i - 1].events);
        printf("re_events = %d\n", fds[i - 1].revents);
        printf("\n-----------------------------\n");
        i--;
    }
}