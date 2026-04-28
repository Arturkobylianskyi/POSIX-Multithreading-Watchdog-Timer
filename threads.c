//PS IS1 321 LAB06
//Artur Kobylianskyi
//ka49465@zut.edu.pl

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "timer.h"

int MAX_THREADS = 64;
int N = 3;
int T = 10;

typedef struct{
    pthread_t tid;
    int lifetime;
    time_t start_time;
    int alive;
} Worker;

Worker *workers;

static void cleanup_handler(int sig){
    if(sig != SIGUSR1) return;


    long elapsed = stop();
    if (elapsed == -1) {
        fprintf(stderr, "stop() called without start()\n\n");
        pthread_exit(NULL);
    }

    pthread_t tid = pthread_self();

    printf("[TID %lu] operating time: %ld ms\n\n",
    tid, elapsed);

    pthread_exit(NULL);
}


static void *thread_func(void *arg){
    (void)arg;

    struct sigaction sa;
    sa.sa_handler = cleanup_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    // start pomiaru
    start();

    long n = 1;
    long factorial = 1;
    while (1){
        factorial *= n;
        n++;
        if(n > 20) n = 1;
    }

    return NULL;

}

int main(int argc, char *argv[]){
    int ret;
    opterr = 0;

    while((ret = getopt(argc, argv, "n:t:")) != -1){
        switch(ret) {
            case 'n': N = atoi(optarg); break;
            case 't': T = atoi(optarg); break;
            case '?':
            fprintf(stderr, "Unknown option '-%c'. \n\n", optopt);
                return 1;
            default: abort();
        }
    }

    if (N > MAX_THREADS){
        printf("Max threads allowed: %d\n\n", MAX_THREADS);
        printf("Set default value: 10");

        N = 10;
    }

    srand((unsigned) time(NULL));

    workers = malloc(N * sizeof(Worker));

    for (int i = 0; i<N; i++){
        workers[i].lifetime = (rand() % T) + 1;
        workers[i].start_time = time(NULL);
        workers[i].alive = 1;
        pthread_create(&workers[i].tid, NULL, thread_func, NULL);
        printf("[MAIN] thread id: %lu, selected lifetime: %d s\n\n", workers[i].tid, workers[i].lifetime);
    }

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    // watchdog
    while(1){
        sleep(1);
        time_t current_time = time(NULL);

        int all_dead = 1;
        for (int i = 0; i<N; i++){
            if(!workers[i].alive) continue;

            all_dead = 0;
            if(current_time - workers[i].start_time >= workers[i].lifetime){
                pthread_kill(workers[i].tid, SIGUSR1);
                printf("[MAIN] Sended SIGUSR1 to TID=%lu\n\n", workers[i].tid);
                workers[i].alive = 0;
            }
        }

        if(all_dead) break;
    }

    for (int i = 0; i < N; i++){
        pthread_join(workers[i].tid, NULL);
    }

    free(workers);
    return 0;
}