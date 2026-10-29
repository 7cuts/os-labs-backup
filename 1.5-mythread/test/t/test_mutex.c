#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"
#include "../../lib/my_mutex.h"

#define NUM_THREADS 200
#define NUM_ITERS 10000

static mythread_mutex_t mutex;
static long x;
static _Atomic(long) x_atomic;

static void *
start_routine (void *arg) {
    for (int i = 0; i < NUM_ITERS; i++) {
        mythread_mutex_lock(&mutex);
        x++;
        mythread_mutex_unlock(&mutex);
        x_atomic++;
    }
    return NULL;
}

TEST(test_mutex) {
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

    long expected = NUM_THREADS * NUM_ITERS;
    if (x_atomic != expected) {
        FAIL("atomic x (%ld) != expected (%ld)", x_atomic, expected);
    }
    if (x != expected) {
        FAIL("x (%ld) != expected (%ld)", x_atomic, expected);
    }


    OK_MSG("x (with mutex) = %ld, x_atomic = %ld", x, x_atomic);
}
