#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "queue.h"

#define THREADS 4
pthread_t threads[THREADS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
queue_t * que;
int args[THREADS], data[THREADS];
 
void * safe_pop();
void safe_push(void *);

void * handler(void * arg) {
    while(1) {
        void * data = (int*)safe_pop();
        *(int*)data += 1;
        safe_push((void*) data);
    }
}

int main(int argc, char * argv[]) {
    int c = 0;
   
    init_queue(&que);
    for(c = 0; c < THREADS; ++c) {
        data[c] = c;
        queue_push(que, (void*)&data[c]);
    }

    for(c = 0; c < THREADS; ++c) {
        args[c] = c;
        if(pthread_create(&threads[c], NULL, handler, (void *) &args[c])) {
            printf("%d failed\n", c);
        }
    }
    
    while(1) {
        usleep(1000 * 1000);
        for(c = 0; c < THREADS; ++c) {
            printf("%d ", data[c]);
        }
        puts("");
    }

    return 0;
}

void * safe_pop() {
    pthread_mutex_lock(&mutex);
    void * data = queue_pop(que);
    pthread_mutex_unlock(&mutex);
    return data;
}

void safe_push(void * data) {
    pthread_mutex_lock(&mutex);
    queue_push(que, data);
    pthread_mutex_unlock(&mutex);
}
