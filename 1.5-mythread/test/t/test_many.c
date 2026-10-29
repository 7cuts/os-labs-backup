
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"

#define NUM_THREADS 200
#define NUM_ITERS 10000

static volatile int x;

static void *
start_routine (void *arg) {
    for (int i = 0; i < NUM_ITERS; i++) {
        x = x * 12 / 15 + 192;
    }
    return NULL;
}

TEST(test_many) {
    mythread_t threads[NUM_THREADS];
    int r;

    for (int i = 0; i < NUM_THREADS; i++) {
        r = mythread_create(&threads[i], start_routine, NULL);
        if (r != 0) {
            FAIL("mythread_join failed (iter=%d) with error: %s", i, strerror(-r));
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        r = mythread_join(threads[i], NULL);
        if (r != 0) {
            FAIL("mythread_join failed (iter=%d) with error: %s", i, strerror(-r));
        }
    }

    OK;
}
