#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bishe.h"

int main(void)
{
    int N = 100000000;
    double d[10] = {0};
    int t;
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        t = random_deg();
        if (t == 1) d[0]++;
        else if (t == 2) d[1]++;
        else if (t == 3) d[2]++;
        else if (t == 4) d[3]++;
        else if (t == 5) d[4]++;
        else if (t == 6) d[5]++;
        else if (t == 7) d[6]++;
        else if (t == 8) d[7]++;
        else if (t == 9) d[8]++;
        else if (t == 10) d[9]++;
        else {
            printf("error");
            return -1;
        }
    }
    for (int i = 0; i < 10; i++)
        printf("%lf\n", d[i] / N);

    return 0;
}
