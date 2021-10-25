#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define NUMTHRDS 4

pthread_t tid[NUMTHRDS];
pthread_mutex_t mutexsum;

int count = 0;

int _checkArea(float x, float y)
{
    int check = 0;
    float r;

    r = sqrt(x * x + y * y);
    if (r <= 1)
    {
        return 1;
    }
    return 0;
}

double _calculateArea(int _calcPoints)
{
    double _newArea;
    _newArea = 4.0 * (double)_calcPoints / 4000000;
    printf("%d .... %f \n", _calcPoints, _newArea);

    return _newArea;
}

// each thread should generate 10 random points between -1 to 1
void *calculatePoints(void *arg)
{
    int threadSum = 0;
    unsigned long i = 0;
    pthread_t id = pthread_self();

    for (i = 0; i < 1000000; i++)
    {
        float num1 = (-1) + (2.0 * rand() / RAND_MAX);
        float num2 = (-1) + (2.0 * rand() / RAND_MAX);

        if (_checkArea(num1, num2))
        {
            // printf("%s \n", "bingo !!");
            threadSum++;
            // printf("%d \n", threadSum);
        };
    }
    printf("Total points created in this thread now -  %d \n ", threadSum);

    pthread_mutex_lock(&mutexsum);
    count += threadSum;
    printf("Total points until now in all threads ran -  %d \n ", count);
    pthread_mutex_unlock(&mutexsum);

    pthread_exit((void *)0);

    // for (i = 0; i < (0xFFFFFFFF); i++)
    //     ;

    return NULL;
}

int main(void)
{
    int i = 0;
    int err;
    void *status;

    pthread_mutex_init(&mutexsum, NULL);

    while (i < 4)
    {
        err = pthread_create(&(tid[i]), NULL, &calculatePoints, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n Thread created successfully\n");

        i++;
    }

    /* Wait on the other threads */
    for (i = 0; i < NUMTHRDS; i++)
    {
        pthread_join(tid[i], &status);
    }

    printf("Sum =  %d \n", count);
    printf("Area =  %f \n", _calculateArea(count));

    pthread_mutex_destroy(&mutexsum);
    pthread_exit(NULL);

    sleep(5);
    return 0;
}
