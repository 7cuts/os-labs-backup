
#include <asm-generic/errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"
#include "../../lib/my_mutex.h"

static void *
start_routine (void *arg) {
   (void)arg;
   return NULL;
}

TEST(test_mutex_double_lock) {
    mythread_mutex_t mutex = {0};

    int r;
    r = mythread_mutex_lock(&mutex);

    if (r != 0) {
        FAIL("first mythread_mutex_lock failed: %s", strerror(-r));
    }

    r = mythread_mutex_lock(&mutex);
    if (r != -EDEADLK) {
        FAIL("second mythread_mutex_lock NOT failed with -EDEADLK, instead got %s", strerror(-r));
    }
    OK_MSG("got expected error: %s", strerror(-r));
}
