#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void printTime(int i, struct timeval T1, struct timeval T2){
    if(i != 49){
        return;
    }
    
    gettimeofday(&T2, NULL);
    printf("%ld ", 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000);
}

void swap(double *xp, double *yp) 
{ 
    double temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
}

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
        for (int j = 0; j < N; j++)
        {
            M1[j] = 1 + (rand_r(&seed) % A);
        }
        for (int j = 0; j < n; j++)
        {
            M2[j] = A + (rand_r(&seed) % (A * 10));
        }

        //map
        for (int j = 0; j < N; j++)
        {
            M1[j] = 1.0 / tan(sqrt(M1[j]));
        }

        for (int j = 0; j < n; j++)
        {
            double result = tan(M2[j]);

            if(result < 0.0)
            {
                result *= -1.0;
            }

            M2[j] = result;
        }

        //merge
        for (int j = 0; j < n; j++)
        {
            M2[j] = M1[j] * M2[j];
        }

        //sort
        int min_idx; 
        for (int i = 0; i < n-1; i++) 
        { 
            min_idx = i; 
            for (int j = i+1; j < n; j++) 
            if (M2[j] < M2[min_idx]) 
                min_idx = j; 
    
            swap(&M2[min_idx], &M2[i]); 
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

        result = sum;
    }
    gettimeofday(&T2, NULL);

    deltaMs = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("%d\t%f\t%ld\n", N, result, deltaMs);
}