#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>


const long long npoints = 3*3*5*7*11*13*0x800;
pthread_t tid[10];

double func(double x)
{
    return 1;
}

double get_time()
{
    struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec + currentTime.tv_usec * 1e-6;
}

void* thread_func(int arg)
{
    struct drand48_data d;
    srand48_r(rand(), &d);
    long long counter = 0;
    for (long long i = 0; i < npoints/arg; i++)
    {
        double x;
        drand48_r(&d, &x);
        double y;
        drand48_r(&d, &y);
        if (y <= func(x))
            counter++;
    }
    long long* ret = malloc(sizeof(long long));
    *ret = counter;
    return ret;
}

int main() {
    for (int y = 1; y < 100; y++)
    {
        long long int t = get_time();
        int i = 0, acc = 0;
        int* k;
        for (i; i < y; i++)
            pthread_create(&tid[i], NULL, &thread_func, y);
        for (i = 0; i < y; i++)
        {
            pthread_join(tid[i], (void**)&k);
            acc+=*k;
        }
        printf("%d\t%.05lf\t%lf\n", i, get_time() - t, ((double)acc/(npoints)));
    }
    return 0;
}