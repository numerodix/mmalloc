#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mmalloc.h"
#include "mtesting.h"


#define NUM_THREADS 3

pthread_t tids[NUM_THREADS];

// more human readable thread ids
int ids[NUM_THREADS];


void *worker(void *arg) {
    int *id = (int *) arg;

    int min_size = 1 << 4;
    int max_size = 10 << 20;

    for (int i=0; i < 10; i++) {
        int size = get_random_int(min_size, max_size);
        printf("[thread %d] allocating buffer of size   : %d bytes\n", *id, size);

        void *ptr = mmalloc(size);
        assert(ptr != NULL);

        // sleep to encourage a context switch
        usleep(1000);

        size = get_random_int(min_size, max_size);
        printf("[thread %d] re-allocating buffer of size: %d bytes\n", *id, size);

        ptr = mrealloc(ptr, size);
        assert(ptr != NULL);

        // sleep to encourage a context switch
        usleep(1000);

        mfree(ptr);
    }

    return NULL;
}


int main() {
    // populate ids
    for (int i=0; i < NUM_THREADS; i++) {
        ids[i] = i;
    }

    long seed_val = set_rand48_seed();
    printf("Using seed value: %ld\n", seed_val);

    // spawn all the threads
    for (int i=0; i < NUM_THREADS; i++) {
        int err = pthread_create(&tids[i], NULL, &worker, &ids[i]);
        if (err) {
            printf("failed to created thread %d: %s\n", i, strerror(err));
            return 1;
        }
    }

    // join all the threads
    for (int i=0; i < NUM_THREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    return 0;
}