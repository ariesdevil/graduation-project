#include <stdio.h>
#include "bishe.h"

int main(void)
{
    const int M = 4;
    const int N = 10;
    const int times = 1000000;
    int selected[M];
    int c[10] = {0};
    for (int i = 0; i < times; i++) {
        random_select(selected, M, N);
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                if (selected[i] == j)
                    c[j]++;
    }

    for (int i = 0; i < N; i++) 
        printf("%lf\n", c[i] / (4.0 * times));
    return 0;
}
