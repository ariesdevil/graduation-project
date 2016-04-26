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
    static int8_t index = 1;
    RawDataBlock* rdb = (RawDataBlock*)malloc(sizeof(RawDataBlock));
    if (rdb) {
        bzero(rdb->data, TOTAL_LENGTH);
        rdb->data[0] = index;
        *(int32_t*)(rdb->data + 1) = size;
        memcpy(rdb->data + 5, data, size);

        if (127 > index) {
            index++;
        } else {
            index = 1;
        }
    } else {
        err_quit("malloc error");
    }
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
                    int k = *(int16_t*)(packets[j].data + 2);
                    for (int m = 0; m < drops; m++) {
                        int n = Packet_contain(packets + m, k);
                        if (-1 != n) {
                            Packet_delete(packets + m , n);
                            if (m == j) {
                                memcpy(rdb->data + k * SLICE_LENGTH, packets[m].data + 22, SLICE_LENGTH);
                            } else {
                                slice_xor(packets[m].data + 22, packets[j].data + 22, SLICE_LENGTH);
                            }
                        }
                    }
                    s.insert(k);
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
    if (p) {
        bzero(p->data, PACKET_LENGTH);
        p->data[0] = rdb->data[0];
        int8_t d = gen_deg();
        p->data[1] = d;
        choose_slice(p->data + 2, d, SLICES);
        for (int8_t i = 0; i < d; i++) {
            slice_xor(p->data + 22, rdb->data + *(int16_t*)(p->data + 2 + i * 2) * SLICE_LENGTH, SLICE_LENGTH);
        }
    } else {
        err_quit("malloc error");
    }
    return p;
}

int
Packet_contain(Packet* packet, int16_t k) {
    assert(packet);
    assert(k >= 0 && k < SLICES);
    int d = packet->data[1];
    for (int i = 0; i < d; i++)
        if (k == *(int16_t*)(packet->data + 2 + i * 2))
            return 2 + i * 2;
    return -1;
}

int16_t
Packet_delete(Packet* packet, int i) {
    assert(packet);
    assert(i >= 2 && i < 22);
    int d = packet->data[1];
    int16_t index = *(int16_t*)(packet->data + i);
    int end = 2 + 2 * d;
    int start = i;
    memmove(packet->data + start, packet->data + start + 2, end - start - 2);
    packet->data[1]--;
    return index;
}

void
Packet_print(Packet* packet)
{
    int degree = packet->data[1];
    int index = packet->data[0];
    fprintf(stderr, "index: %d\tdegree: %d\t", index, degree);
    for (int i = 0; i < degree; i++)
        fprintf(stderr, "%d ", *(int16_t*)(packet->data + 2 + i * 2));
    fprintf(stderr, "\n");
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
    bzero(chosen, M);
    int select = M, remaining = N;
    for (int i = 0, k = 0; i < N; i++) {
        if (rand() % remaining < select) {
            *(int16_t*)(chosen + k) = i;
            k += 2;
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
