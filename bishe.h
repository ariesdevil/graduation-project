#ifndef BISHE_H
#define BISHE_H

#include <stdint.h>
#include <stddef.h>

#define err_quit(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while(0)

/*一滴数据的长度，或者一个未编码的数据块的切片的长度，这是一个重要的约定参数*/
#define SLICE_LENGTH 1024

/*待编码的数据块的切片数目*/
#define SLICES 10

/*待编码的数据块的长度*/
#define TOTAL_LENGTH (SLICE_LENGTH * SLICES)

/*可能出现的度的种类，或度的数目，可能的度有[1, DEG_MAX]*/
#define DEG_MAX SLICES

/*对于一个数据块，喷泉码编码器喷的滴数*/
#define DROPS 60

#define PACKET_LENGTH 1 + 1 + 10 + SLICE_LENGTH

/******************编码后的数据块的最小单位****************/
typedef struct Packet {
    char data[PACKET_LENGTH];//第1个byte表示RawDataBlock的索引，第2个byte表示度数，后10个字节表示切片组合，置为'1'的被选中做异或，置为'0'的没有被选中
} Packet;

/******************补零的原始数据块****************/
typedef struct RawDataBlock {
    char data[TOTAL_LENGTH]; //第1个byte表示索引，再4byte表示原始长度，后续表示数据
} RawDataBlock;

RawDataBlock* RawDataBlock_FromRawData(char* data, int size);

RawDataBlock* RawDataBlock_FromPackets(Packet* packets, int drops);

//喷1滴
Packet* Packet_FromRawDataBlock(RawDataBlock* rdb);


//度发生器
int8_t gen_deg();

//随机选择切片
void choose_slice(char* chosen, int M, int N);

//切片做异或运算，存入left
void slice_xor(char* left, char* right, int N);

#endif
