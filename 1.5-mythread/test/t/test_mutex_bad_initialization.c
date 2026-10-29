

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

TEST(test_mutex_bad_initialization) {
    mythread_mutex_t mutex = {.owner = 12345};

    int r;
    r = mythread_mutex_lock(&mutex);
    if (r != -EINVAL) {
        FAIL("second mythread_mutex_lock NOT failed with -EINVAL, instead got %s", strerror(-r));
    }
    OK_MSG("got expected error: %s", strerror(-r));
}
