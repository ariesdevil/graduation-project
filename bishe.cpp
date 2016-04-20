#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bishe.h"

int random_deg()
{
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

void random_select(int* selected, ssize_t M , ssize_t N)
{
    int select = M, remaining = N;
    for (int i = 0, j = 0; i < N && j < M; i++) {
        if (rand() % remaining < select) {
            selected[j++] = i;
            select--;
        }
        remaining--;
    }
}

void xor_slices(char* data, ssize_t M, ssize_t N, char* dst)
{
    bzero(dst, M);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            dst[j] ^= data[i * M + j];
        }
    }
}

void encode(char* buf, ssize_t length, LT* ltp)
{
    srand(time(NULL));
    ltp->deg = random_deg();
    bzero(ltp->slices, sizeof(char) * DEG_MAX);
    random_select(ltp->slices, ltp->deg, length / LENGTH_PER_SLICE);
    xor_slices(buf, LENGTH_PER_SLICE, length / LENGTH_PER_SLICE, ltp->encoded_data);
}
