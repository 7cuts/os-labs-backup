#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"

#define NUM_THREADS 200
#define NUM_ITERS 10000

static long x;
static _Atomic(long) x_atomic;

static void *
start_routine (void *arg) {
    for (int i = 0; i < NUM_ITERS; i++) {
        x++;
        x_atomic++;
    }
    return NULL;
}

TEST(test_data_race) {
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

    if (x == expected) {
        FAIL("no data race happend: x = %ld", (x));
    }

    OK_MSG("x = %ld, x_atomic = %ld", x, x_atomic);
}
