#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>


int main(){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0){
        printf("error in socket()");
        return 0;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int rv = connect(fd, (const struct sockaddr *) &addr, sizeof(addr));
    if(rv<0){
        printf("error in connect()");
        return 0;
    }

    char msg[] = "world from client";
    write(fd, msg, strlen(msg));

    char rbuf[64] = {};
    ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
    if(n <0){
        printf("error in read");
        return 0;
    }
    printf("server says: %s", rbuf);
    close(fd);

}