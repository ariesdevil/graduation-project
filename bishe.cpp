#include "bishe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <set>

using std::set;

RawDataBlock*
RawDataBlock_FromRawData(char* data, int size) {
    assert(data);
    assert(TOTAL_LENGTH >= size + 5);
    static int8_t index = 0;
    if (127 > index) {
        index++;
    } else {
        index = 0;
    }
    RawDataBlock* rdb = (RawDataBlock*)malloc(sizeof(RawDataBlock));
    int32_t* p4 = (int32_t*)malloc(sizeof(4));
    if (rdb && p4) {
        bzero(rdb->data, TOTAL_LENGTH);
        rdb->data[0] = index;
        *p4 = size;
        memcpy(rdb->data + 1, p4, 4);
        memcpy(rdb->data + 1 + 4, data, size);
    } else {
        err_quit("malloc error");
    }
    free(p4);
    return rdb;
}

RawDataBlock*
RawDataBlock_FromPackets(Packet* packets, int drops)
{
    assert(packets);
    assert(DROPS >= drops);
    bool success = false;
    set<int8_t> s;
    RawDataBlock* rdb = (RawDataBlock*)malloc(sizeof(RawDataBlock));
    if (rdb) {
        bzero(rdb->data, TOTAL_LENGTH);
        for (int i = 0; i < drops; i++) {
            for (int j = 0; j < drops; j++) {
                if (1 == packets[j].data[1]) {
                    int k;
                    for (k = 2; k < 2 + DEG_MAX; k++) {
                        if ('1' == packets[j].data[k]) {
                            break;
                        }
                    }
                    for (int m = 0; m < drops; m++) {
                        if ('1' == packets[m].data[k]) {
                            packets[m].data[k] = '0';
                            packets[m].data[1]--;
                            if (m == j) {
                                memcpy(rdb->data + SLICE_LENGTH* (k - 2), packets[m].data + 12, SLICE_LENGTH);
                            } else {
                                slice_xor(packets[m].data + 12, packets[j].data + 12, SLICE_LENGTH);
                            }
                        }
                    }
                    s.insert(k - 2);
                    if (SLICES == s.size()) {
                        success = true;
                        goto success;
                    }
                }
            }
        }
    } else {
        err_quit("malloc error");
    }
success:
    if (success) {
        fprintf(stderr, "1\n");
    } else {
        fprintf(stderr, "0\n");
    }
    return rdb;
}

Packet*
Packet_FromRawDataBlock(RawDataBlock* rdb) {
    assert(rdb);
    Packet* p = (Packet*)malloc(sizeof(Packet));
    bzero(p->data, sizeof(p->data));
    p->data[0] = rdb->data[0];
    int8_t d = gen_deg();
    p->data[1] = d;
    choose_slice(p->data + 2, d, SLICES);
    for (int8_t i = 0; i < SLICES; i++) {
        if ('1' == *(p->data + 2 + i))
            slice_xor(p->data + 12, rdb->data + i * SLICE_LENGTH, SLICE_LENGTH);
    }
    return p;
}

int8_t
gen_deg() {
    /*度的概率分布：理想孤子分布
    static double p[10] = {
        0.1,
        0.5,
        0.1667,
        0.0833,
        0.05,
        0.0333,
        0.0238,
        0.0179,
        0.0139,
        0.0111
    };
    */
    static double P[10] = { // P是p的前i项的累加
        0.1,
        0.6,
        0.7667,
        0.85,
        0.9,
        0.9333,
        0.9571,
        0.975,
        0.9889,
        1,
    };

    double d = drand48();
    int i = 0;
    while (i < 10 && d > P[i])
        i++;
    return i + 1;
}

void
choose_slice(char* chosen, int M, int N) {
    assert(chosen);
    assert(M <= N && M > 0);
    memset(chosen, '0', N);
    int select = M, remaining = N;
    for (int i = 0; i < N; i++) {
        if (rand() % remaining < select) {
            chosen[i] = '1';
            select--;
        }
        remaining--;
    }
}

void
slice_xor(char* left, char* right, int N) {
    assert(left);
    assert(right);
    assert(N > 0);
    for (int i = 0; i < N; i++) {
        left[i] ^= right[i];
    }
}
