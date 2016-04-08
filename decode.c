#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define err_quit(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define MAXLEN 2048
#define RECVADDR "192.168.1.100"
#define RECVPORT 8081
#define SENDADDR "127.0.0.1"
#define SENDPORT 8081
#define DESTADDR "127.0.0.1"
#define DESTPORT 8080

int main(void)
{
    int recv_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int send_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in recv_addr, send_addr, dest_addr;
    bzero(&recv_addr, sizeof(recv_addr));
    bzero(&send_addr, sizeof(send_addr));
    bzero(&dest_addr, sizeof(dest_addr));
    recv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, RECVADDR, &recv_addr.sin_addr);
    recv_addr.sin_port = htons(RECVPORT);
    send_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SENDADDR, &send_addr.sin_addr);
    send_addr.sin_port = htons(SENDPORT);
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, DESTADDR, &dest_addr.sin_addr);
    dest_addr.sin_port = htons(DESTPORT);
    char buf[MAXLEN];

    if (-1 == recv_sockfd) err_quit("recv socket error");
    if (-1 == send_sockfd) err_quit("send socket error");
    if (-1 == bind(recv_sockfd, (struct sockaddr *)&recv_addr, sizeof(recv_addr))) 
        err_quit("recv bind error");
    if (-1 == bind(send_sockfd, (struct sockaddr *)&send_addr, sizeof(send_addr)))
        err_quit("send bind error");

    for (;;) {
        ssize_t n = recvfrom(recv_sockfd, &buf, MAXLEN, 0, NULL, NULL);
        if (-1 == n) err_quit("recvfrom error");
        if (-1 == sendto(send_sockfd, &buf, n, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)))
            err_quit("sendto error");
    }

    return 0;
}
