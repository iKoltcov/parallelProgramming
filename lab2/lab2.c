#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <FW_1.3.1_Lin64/fwBase.h>
#include <FW_1.3.1_Lin64/fwImage.h>
#include <FW_1.3.1_Lin64/fwSignal.h>

# define M_E		2.7182818284590452354	/* e */

int main(int argc, char *argv[])
{
    struct timeval T1, T2;
    long deltaMs;

    int N = atoi(argv[1]);
    int n = N / 2;
    unsigned int seed = 0;

    // 6, 3, 3, 7
    int A = 7 * 4 * 11;
    double result = 0.0;

    int M = atoi(argv[2]);
    fwSetNumThreads(M);

    gettimeofday(&T1, NULL);
    for (int i = 0; i < 50; i++)
    {
        //generate
        double *M1 = malloc(sizeof(double) * N);
        double *M2 = malloc(sizeof(double) * n);
        for (int j = 0; j < N; j++)
        {
            M1[j] = 1 + (rand_r(&seed) % A);
        }
        for (int j = 0; j < n; j++)
        {
            M2[j] = A + (rand_r(&seed) % (A * 10));
        }

        //map1 = 6
        Fw64f *buffer1 = malloc(sizeof(Fw64f) * N);
        Fw64f *buffer2 = malloc(sizeof(Fw64f) * N);
        Fw64f power = 1.0 / 3.0;

        fwsSet_64f(M_E, buffer2, N);
        fwsDiv_64f_A53(M1, buffer2, buffer1, N);
        fwsSet_64f(power, buffer2, N);
        fwsPow_64f_A53(buffer1, buffer2, M1, N);

        free(buffer1);
        free(buffer2);

        for (int j = 0; j < n; j++)
        {
            M2[j] += j == 0 ? 0 : M2[j - 1];
        }

        //map2 = 3
        fwsTan_64f_A53(M2, M2, n);
        fwsAbs_64f_I(M2, n);

        //merge = 3
        fwsMul_64f(M1, M2, M2, n);

        //sort = 7
        int min_idx; 
        for (int k = 0; k < n-1; k++) 
        { 
            min_idx = k; 
            for (int j = k+1; j < n; j++)
            {
                if (M2[j] < M2[min_idx])
                {
                    min_idx = j; 
                }
            }

            double tmp = M2[min_idx];
            M2[min_idx] = M2[k];
            M2[k] = tmp;
        } 

        //reduce
        double min = 0.0;
        for (int j = 0; j < n; j++)
        {
            if(M2[j] > 0.0 && min < M2[j])
            {
                min = M2[j];
            }
        }

        double sum = 0.0;
        for(int j = 0; j < n; j++)
        {
            if(((int)(M2[j] / min)) % 2 == 0 )
            {
                sum += sin(M2[j]);
            }
        }

        result += sum;

        free(M1);
        free(M2);
    }
    gettimeofday(&T2, NULL);

    deltaMs = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("%d\t%f\t%ld\n", N, result / 50.0, deltaMs);
}