#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "bishe.h"

#define SENDER_ADDR "192.168.1.105"
#define SENDER_PORT_E 8080
#define SENDER_PORT_R 8081
#define RECEIVER_ADDR "192.168.1.100"
#define RECEIVER_PORT_E 8080
#define RECEIVER_PORT_R 8081

int
main(void) {
    int sock_fd_e = socket(AF_INET, SOCK_DGRAM, 0);
    int sock_fd_r = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sock_fd_e || -1 == sock_fd_r) {
        err_quit("socket error");
    }

    struct sockaddr_in sender_addr_e;
    bzero(&sender_addr_e, sizeof(struct sockaddr_in));
    sender_addr_e.sin_family = AF_INET;
    inet_pton(AF_INET, SENDER_ADDR, &(sender_addr_e.sin_addr));
    sender_addr_e.sin_port = htons(SENDER_PORT_E);
    if (-1 == bind(sock_fd_e, (struct sockaddr*)&(sender_addr_e), sizeof(struct sockaddr_in))) {
        err_quit("bind error");
    }
    struct sockaddr_in receiver_addr_e;
    bzero(&receiver_addr_e, sizeof(struct sockaddr_in));
    receiver_addr_e.sin_family = AF_INET;
    inet_pton(AF_INET, RECEIVER_ADDR, &(receiver_addr_e.sin_addr));
    receiver_addr_e.sin_port = htons(RECEIVER_PORT_E);

    struct sockaddr_in sender_addr_r;
    bzero(&sender_addr_r, sizeof(struct sockaddr_in));
    sender_addr_r.sin_family = AF_INET;
    inet_pton(AF_INET, SENDER_ADDR, &(sender_addr_r.sin_addr));
    sender_addr_r.sin_port = htons(SENDER_PORT_R);
    if (-1 == bind(sock_fd_r, (struct sockaddr*)&(sender_addr_r), sizeof(struct sockaddr_in))) {
        err_quit("bind error");
    }
    struct sockaddr_in receiver_addr_r;
    bzero(&receiver_addr_r, sizeof(struct sockaddr_in));
    receiver_addr_r.sin_family = AF_INET;
    inet_pton(AF_INET, RECEIVER_ADDR, &(receiver_addr_r.sin_addr));
    receiver_addr_r.sin_port = htons(RECEIVER_PORT_R);

    FILE* file_fp = fopen("video.h264", "wb");
    if (NULL == file_fp) {
        err_quit("fopen error");
    }

    int size = TOTAL_LENGTH - 5;
    char buf[size];
    int t = 0;
    int n = 0;
    int pipsize = 4096;
    for (int psize = pipsize; (-1 != (t = read(STDIN_FILENO, buf + n, psize))); ) {
        n += t;
        if (n + psize > size) {
            if (n != fwrite(buf, 1, n, file_fp)) {
                err_quit("fwrite error");
            }

            //sendto(sock_fd_r, buf, n, 0, (struct sockaddr*)&(receiver_addr_r), sizeof(struct sockaddr_in));

            RawDataBlock* rdb = RawDataBlock_FromRawData(buf, n);
            for (int i = 0; i < DROPS; i++) {
                Packet* p = Packet_FromRawDataBlock(rdb);
                if (PACKET_LENGTH != sendto(sock_fd_e, p->data, PACKET_LENGTH, 0, (struct sockaddr*)&(receiver_addr_e), sizeof(struct sockaddr_in))) {
                    err_quit("sendto error");
                }
                free(p);
            }
            free(rdb);
            n = 0;
        }
    }

    return 0;
}
