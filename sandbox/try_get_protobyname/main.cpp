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