#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

# define M_E		2.7182818284590452354	/* e */

int main(int argc, char *argv[])
{
    struct timeval T1, T2;
    long deltaMs;

    int N = atoi(argv[1]);
    int n = N / 2;
    unsigned int seed = 0;

    int A = 7 * 4 * 11;
    double result = 0.0;

    gettimeofday(&T1, NULL);
    for (int i = 0; i < 50; i++)
    {
        //generate
        double *M1 = malloc(sizeof(double) * N);
        double *M2 = malloc(sizeof(double) * n);
        int j = 0;

        #pragma omp parallel for default(none) private(j) shared(A, M1, N, seed) schedule(static, 16) ordered
        for (j = 0; j < N; j++)
        {         
            #pragma omp ordered
            M1[j] = 1 + (rand_r(&seed) % A);
        }

        #pragma omp parallel for default(none) private(j) shared(A, M2, n, seed) schedule(static, 16) ordered
        for (j = 0; j < n; j++)
        {
            #pragma omp ordered
            M2[j] = A + (rand_r(&seed) % (A * 10));
        }

        //map1 = 6
        #pragma omp parallel for default(none) private(j) shared(M1, N) schedule(static, 16)
        for (j = 0; j < N; j++)
        {
            M1[j] = pow(M1[j] / M_E, 1.0 / 3.0);
        }

        //map2
        #pragma omp parallel for default(none) private(j) shared(M2, n) schedule(static, 16) ordered
        for (j = 1; j < n; j++)
        {
            #pragma omp ordered
            M2[j] += M2[j - 1];
        }

        //map3 = 3
        #pragma omp parallel for default(none) private(j) shared(M2, n) schedule(static, 16)
        for (j = 0; j < n; j++)
        {
            M2[j] = fabs(tan(M2[j]));
        }

        //merge = 3
        #pragma omp parallel for default(none) private(j) shared(M1, M2, n) schedule(static, 16)
        for (j = 0; j < n; j++)
        {
            M2[j] = M1[j] * M2[j];
        }

        //sort = 7
        int k = 0;
        #pragma omp parallel for default(none) private(k) shared(M2, n) schedule(static, 16)
        for (k = 0; k < n-1; k++) 
        { 
            int min_idx = k; 
            for (int j = k+1; j < n; j++)
            {
                if (M2[j] < M2[min_idx])
                {
                    min_idx = j; 
                }
            }

            #pragma omp critical
            {
                double tmp = M2[min_idx];
                M2[min_idx] = M2[k];
                M2[k] = tmp;
            }
        } 

        //reduce
        double min = 0.0;
        #pragma omp parallel for default(none) private(j) shared(M2, min, n) schedule(static, 16)
        for (j = 0; j < n; j++)
        {
            if(M2[j] > 0.0)
            {
                #pragma omp critical
                {
                    if(min < M2[j])
                    {
                        min = M2[j];
                    }
                }
            }
        }

        double sum = 0.0;
        #pragma omp parallel for default(none) private(j) shared(M2, min, n, sum) schedule(static, 16)
        for(j = 0; j < n; j++)
        {
            if(((int)(M2[j] / min)) % 2 == 0 )
            {
                #pragma omp atomic
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