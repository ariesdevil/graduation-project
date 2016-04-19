#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define err_quit(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define BUFSIZE 2048
#define SOURCEADDR "192.168.1.105"
#define SOURCEPORT 8080
#define DESTADDR "192.168.1.100"
#define DESTPORT 8080

typedef struct Bishe {
    int source_socket_fd;
    struct sockaddr_in source_addr;
    struct sockaddr_in dest_addr;
    FILE* fp;
} Bishe;

Bishe* init_bishe()
{
    Bishe* bishe = (Bishe*)malloc(sizeof(Bishe));
    if (NULL == bishe) err_quit("malloc error");

    int source_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == source_socket_fd) err_quit("socket error");
    bishe->source_socket_fd = source_socket_fd;

    bzero(&(bishe->source_addr), sizeof(struct sockaddr_in));
    bishe->source_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SOURCEADDR, &(bishe->source_addr.sin_addr));
    bishe->source_addr.sin_port = htons(SOURCEPORT);

    bzero(&(bishe->dest_addr), sizeof(struct sockaddr_in));
    bishe->dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, DESTADDR, &(bishe->dest_addr.sin_addr));
    bishe->dest_addr.sin_port = htons(DESTPORT);

    if (-1 == bind(bishe->source_socket_fd, 
                (struct sockaddr*)&(bishe->source_addr), sizeof(struct sockaddr_in)))
        err_quit("bind error");

    if (NULL == (bishe->fp = fopen("video.h264", "wb")))
        err_quit("fopen error");

    return bishe;
}

void free_bishe(Bishe* bishe)
{
    if (EOF == fclose(bishe->fp)) err_quit("fclose error");
    free(bishe);
}

void forward_save(Bishe* bishe)
{
    ssize_t n;
    char buf[BUFSIZE];
    fd_set wset;
    FD_ZERO(&wset);
    int file_no = fileno(bishe->fp);

    for (;;) {
        if (-1 == (n = read(STDIN_FILENO, buf, BUFSIZE))) {
            err_quit("read error");
        } else if (0 == n) {
            return;
        } else {
            FD_SET(bishe->source_socket_fd, &wset);
            FD_SET(file_no, &wset);
        }

        if (-1 == select(fmax(bishe->source_socket_fd, file_no) + 1, NULL, &wset, NULL, NULL)) 
            err_quit("select error");

        if (FD_ISSET(bishe->source_socket_fd, &wset)) {
            if (BUFSIZE != sendto(bishe->source_socket_fd, buf, BUFSIZE, 0, 
                        (struct sockaddr*)&(bishe->dest_addr), sizeof(struct sockaddr_in)))
                err_quit("sendto error");
        }

        if (FD_ISSET(file_no, &wset)) {
            if (BUFSIZE != fwrite(buf, 1, BUFSIZE, bishe->fp))
                err_quit("fwrite error");
        }
    }
}

void save(Bishe* bishe)
{
    bishe->fp = fopen("video.h264", "wb");
    if (NULL == bishe->fp) err_quit("fopen error");
}


int main(void)
{
    Bishe* bishe = init_bishe();
    forward_save(bishe);
    free_bishe(bishe);
    return 0;
}
