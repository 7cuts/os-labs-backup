#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"

static void *
start_routine (void *arg) {
   (void)arg;
   return NULL;
}

TEST(test_basic) {
    mythread_t thread;
    int r;
    r = mythread_create(&thread, start_routine, NULL);
    if (r != 0) {
        FAIL("mythread_create failed with error: %s", strerror(-r));
    }

    r = mythread_join(thread, NULL);

    if (r != 0) {
        FAIL("mythread_join failed with error: %s", strerror(-r));
    }
    OK;
}
