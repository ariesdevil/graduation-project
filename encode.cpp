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
#define SENDER_PORT 8080
#define RECEIVER_ADDR "192.168.1.100"
#define RECEIVER_PORT 8080

int
main(void) {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sock_fd) {
        err_quit("socket error");
    }

    struct sockaddr_in sender_addr;
    bzero(&sender_addr, sizeof(struct sockaddr_in));
    sender_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SENDER_ADDR, &(sender_addr.sin_addr));
    sender_addr.sin_port = htons(SENDER_PORT);
    if (-1 == bind(sock_fd, (struct sockaddr*)&(sender_addr), sizeof(struct sockaddr_in))) {
        err_quit("bind error");
    }

    struct sockaddr_in receiver_addr;
    bzero(&receiver_addr, sizeof(struct sockaddr_in));
    receiver_addr.sin_family = AF_INET;
    inet_pton(AF_INET, RECEIVER_ADDR, &(receiver_addr.sin_addr));
    receiver_addr.sin_port = htons(RECEIVER_PORT);

    FILE* file_fp = fopen("video.h264", "wb");
    if (NULL == file_fp) {
        err_quit("fopen error");
    }

    int size = TOTAL_LENGTH - 5;
    char buf[size];
    int t = 0;
    int n = 0;
    for (int psize = size; (-1 != (t = read(STDIN_FILENO, buf + n, psize))); ) {
        n += t;
        psize /= 2;
        if (n + psize > size) {
            printf("%d\n", n);
            if (n != fwrite(buf, 1, n, file_fp)) {
                err_quit("fwrite error");
            }
            RawDataBlock* rdb = RawDataBlock_FromRawData(buf, n);
            for (int i = 0; i < DROPS; i++) {
                Packet* p = Packet_FromRawDataBlock(rdb);
                if (PACKET_LENGTH != sendto(sock_fd, p->data, PACKET_LENGTH, 0, (struct sockaddr*)&(receiver_addr), sizeof(struct sockaddr_in))) {
                    err_quit("sendto error");
                }

                free(p);
            }
            free(rdb);
            n = 0;
            psize = size;
        }
    }

    return 0;
}
