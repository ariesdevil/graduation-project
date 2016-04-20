#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

#define err_quit(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define BUFSIZE 10240
#define TOTAL_LENGTH BUFSIZE
#define LENGTH_PER_SLICE 1024
#define DEG_MAX 10

typedef struct LT {
    char encoded_data[LENGTH_PER_SLICE];
    int deg;
    int slices[DEG_MAX]; //被随机选中的deg个片段，至多10个片段
} LT;


/*理想孤子分布的度发生器*/
int random_deg();

/*N个数[0 ~ N-1]均匀选取M个数*/
void random_select(int* selected, ssize_t M , ssize_t N);

/*喷泉码编码一次，喷一滴*/
void encode(char* buf, ssize_t length, LT* ltp);

/*N个M长的切片做异或运算，结果保存到dst*/
void xor_slices(char* data, ssize_t M, ssize_t N, char* dst);

#endif
