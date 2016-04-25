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

    struct sockaddr_in receiver_addr;
    bzero(&receiver_addr, sizeof(struct sockaddr_in));
    receiver_addr.sin_family = AF_INET;
    inet_pton(AF_INET, RECEIVER_ADDR, &(receiver_addr.sin_addr));
    receiver_addr.sin_port = htons(RECEIVER_PORT);

    if (-1 == bind(sock_fd, (struct sockaddr*)&(receiver_addr), sizeof(struct sockaddr_in))) {
        err_quit("bind error");
    }

    int n;
    socklen_t* len = (socklen_t*)malloc(sizeof(socklen_t));
    char buf[PACKET_LENGTH];
    Packet packets[DROPS];
    int8_t last_index; //上一滴所属原数据的索引
    int8_t this_index; //这一滴所属原数据的索引
    bool isfirstloop = true;
    int i = 0;
    bzero(buf, PACKET_LENGTH);
    while (true) {
        n = recvfrom(sock_fd, buf, PACKET_LENGTH, 0, (struct sockaddr*)&(sender_addr), len);
        if (-1 == n) {
            err_quit("recvfrom error");
        } else if (PACKET_LENGTH != n) {
            break;
        } else {
            this_index = buf[0];
            if (isfirstloop || this_index == last_index) {
                memcpy(packets[i].data, buf, PACKET_LENGTH);
                i++;
                isfirstloop = false;
            } else {
                RawDataBlock* rdb = RawDataBlock_FromPackets(packets, i);
                if (rdb) {
                    //fprintf(stderr, "index: %d\tlength: %d\n", *(int8_t*)(rdb->data), *(int32_t*)(rdb->data + 1));
                    fwrite(rdb->data + 5, 1, *(int32_t*)(rdb->data + 1), stdout);
                    }
                free(rdb);
                i = 0;
                memcpy(packets[i].data, buf, PACKET_LENGTH);
            }
            last_index = this_index;
        }
    }
    free(len);
    return 0;
}
