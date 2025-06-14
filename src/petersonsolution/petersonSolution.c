#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_ITERATIONS 5

// Shared variables
volatile bool flag[2] = {false, false}; // determines whether this thread is ready
volatile int turn;  // show whose turn next time

// Thread function for each process
void* process(void* arg) {
    int this = *(int*) arg, that = 1 - this;

    for (int i = 0; i < NUM_ITERATIONS; i++) {

        // This thread is in entry section.
        flag[this] = true;
        turn = that;    
        while (flag[that] && turn == that);

        // Now this thread is in critical section.
        printf("Process %d started the %dth critical section.\n", this, i + 1);
        sleep(1);
        printf("Process %d terminated the %dth critical section.\n", this, i + 1);
        flag[this] = false;

        // Now this thread is in remainder section.
        printf("Process %d is in the %dth remainder section.\n", this, i + 1);

    }
    return NULL;
}

int main() {
    pthread_t t0, t1;
    int id0 = 0, id1 = 1;

    pthread_create(&t0, NULL, process, &id0);
    pthread_create(&t1, NULL, process, &id1);

    pthread_join(t0, NULL); // waiting for thread id0
    pthread_join(t1, NULL); // waiting for thread id1

    return 0;
}