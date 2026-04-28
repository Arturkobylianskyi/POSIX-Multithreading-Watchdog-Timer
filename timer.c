//PS IS1 321 LAB06
//Artur Kobylianskyi
//ka49465@zut.edu.pl

#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"


static pthread_key_t timer_key;
static pthread_once_t timer_once = PTHREAD_ONCE_INIT;


static void freeMemory(void *buffer)
{
    free(buffer);
}


static void create_key(void){
    pthread_key_create(&timer_key, freeMemory);
}

void start(void){
    pthread_once(&timer_once, create_key);

    struct timespec *ts = pthread_getspecific(timer_key);

    if(ts == NULL){
        ts = malloc(sizeof(struct timespec));
        pthread_setspecific(timer_key, ts);
    }

    clock_gettime(CLOCK_REALTIME, ts);
}

long stop(void){
    pthread_once(&timer_once, create_key);

    struct timespec *ts_start = pthread_getspecific(timer_key);
    if(ts_start == NULL){
        return -1;
    }

    struct timespec ts_end;

    clock_gettime(CLOCK_REALTIME, &ts_end);

  
    long sec = ts_end.tv_sec - ts_start->tv_sec;
    long nsec = ts_end.tv_nsec - ts_start->tv_nsec;

    long ms = sec * 1000L + nsec / 1000000L;

    return ms;
}