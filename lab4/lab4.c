#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

# define M_E		2.7182818284590452354	/* e */

#include <omp.h>
#ifdef _OPENMP
    #include <omp.h>
#else
    int omp_get_num_procs() { return 1; }
    double omp_get_wtime() {
        struct timeval T11;
        gettimeofday(&T11, NULL);
        return (1000.0 * T11.tv_sec + T11.tv_usec / 1000.0) / 1000.0;
    }
#endif

int percent = 0;
double lastPrintSec = 0;

void selectionSort(double* array, int start, int n)
{
    for (int i = start; i < n - 1; i++) 
    { 
        int min_idx = i; 
        for (int j = i + 1; j < n; j++)
        {
            if (array[j] < array[min_idx])
            {
                min_idx = j; 
            }
        }
        
        double tmp = array[min_idx];
        array[min_idx] = array[i];
        array[i] = tmp;
    }
}

void workProcess(int N, int n)
{
    int A = 7 * 4 * 11;
    double result = 0.0;

    int runCount = 10;
    long* deltasMs = (long*) malloc(sizeof(long) * runCount);
    long sumDeltaMs = 0;
    long minDeltaMs = __LONG_MAX__;

    for(int run = 0; run < runCount; run++)
    {
        double T1, T2;
        result = 0.0;
        percent = (100.0 / (double)runCount) * run;

        T1 = omp_get_wtime();
        for (int i = 0; i < 50; i++)
        {
            //generate
            double *M1 = malloc(sizeof(double) * N);
            double *M2 = malloc(sizeof(double) * n);

            #pragma omp parallel for default(none) shared(M1, N, A, i)
            for (int j = 0; j < N; j++)
            {         
                unsigned int seed = i;
                M1[j] = 1 + (rand_r(&seed) % A);
            }

            #pragma omp parallel for default(none) shared(M2, n, A, i)
            for (int j = 0; j < n; j++)
            {
                unsigned int seed = i;
                M2[j] = A + (rand_r(&seed) % (A * 10));
            }

            //map1 = 6
            #pragma omp parallel for default(none) shared(M1, N)
            for (int j = 0; j < N; j++)
            {
                M1[j] = pow(M1[j] / M_E, 1.0 / 3.0);
            }

            //map2
            #pragma omp parallel for default(none) shared(M2, n) ordered
            for (int j = 1; j < n; j++)
            {
                #pragma omp ordered
                M2[j] += M2[j - 1];
            }

            //map3 = 3
            #pragma omp parallel for default(none) shared(M2, n)
            for (int j = 0; j < n; j++)
            {
                M2[j] = fabs(tan(M2[j]));
            }

            //merge = 3
            #pragma omp parallel for default(none) shared(M1, M2, n)
            for (int j = 0; j < n; j++)
            {
                M2[j] = M1[j] * M2[j];
            }

            //sort = 7
            int threadsCount = omp_get_num_procs();
            #pragma omp parallel for default(none) shared(M2, n, threadsCount)
            for(int thread = 0; thread < threadsCount; thread++)
            {
                int chunkSize = n / threadsCount;
                selectionSort(M2, chunkSize * thread, chunkSize * (thread + 1));
            }

            //reduce
            double min = 0.0;
            #pragma omp parallel for default(none) shared(M2, min, n)
            for (int j = 0; j < n; j++)
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
            #pragma omp parallel for default(none) shared(M2, min, n, sum)
            for(int j = 0; j < n; j++)
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
        T2 = omp_get_wtime();

        long deltaMs = (T2 - T1) * 1000;
        if(minDeltaMs > deltaMs)
        {
            minDeltaMs = deltaMs;
        }
        sumDeltaMs += deltaMs;
        deltasMs[run] = deltaMs;
    }

    double meanDeltaMs = sumDeltaMs / (double)runCount;
    double stDev = 0;
    for(int run = 0; run < runCount; run++)
    {
        stDev += pow(deltasMs[run] - meanDeltaMs, 2);
    }
    stDev = sqrt(stDev / (double)(runCount - 1));
    double confInterval = 2.26 * (stDev / sqrt(runCount));

    printf("%d\t%f\t%ld\t%f ± %f\n", N, result / 50.0, minDeltaMs, meanDeltaMs, confInterval);
    percent = 100;
}

void statisticProcess()
{
    while(1)
    {
        double nowSec = omp_get_wtime();
        if(nowSec - lastPrintSec >= 1)
        {
            lastPrintSec = nowSec;
            printf("[%d%%]\n", percent);
            fflush(stdout);
        }

        if(percent >= 100)
        {
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    int N = atoi(argv[1]);
    int n = N / 2;

#ifdef _OPENMP
    #pragma omp parallel sections
    {
        // #pragma omp section
        // {
        //     statisticProcess();
        // }
        #pragma omp section
        {
            workProcess(N, n);
        }
    }
#else
    workProcess(N, n);
#endif

    return 0;
}