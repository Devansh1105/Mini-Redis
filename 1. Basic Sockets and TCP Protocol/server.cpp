#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}


static void do_something(int conn_fd){
    char rbuf[64] = {};
    ssize_t n = read(conn_fd, rbuf, sizeof(rbuf)-1);
    if(n < 0){
        die("read()\n");
        return;
    }
    printf("client says: %s\n", rbuf);
    char wbuf[] = "world from server\n";
    write(conn_fd, wbuf, strlen(wbuf)); 
}

int main(){
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(0);
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if(rv){die("bind()");}

    rv = listen(fd, SOMAXCONN);
    if (rv) { die("listen()"); }

    while(1){
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int conn_fd = accept(fd, (struct sockaddr *) &client_addr, &addrlen);
        if(conn_fd < 0){
            continue;
        }
        do_something(conn_fd);
        close(conn_fd);
    }

}

