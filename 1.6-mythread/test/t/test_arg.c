#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"

static void *
start_routine (void *arg) {
    long a = (long)(intptr_t) arg;
    return (void*) (2*a);
}

TEST(test_arg) {
    mythread_t thread;
    int r = mythread_create(&thread, start_routine, (void*)2);
    if (r != 0) {
        FAIL("mythread_create failed with error: %s", strerror(-r));
    }

    long result;
    r = mythread_join(thread, (void*)&result);

    if (r != 0) {
        FAIL("mythread_join failed with error: %s", strerror(-r));
    }

    if (result != 4) {
        FAIL("expeted result == 4 but got %ld", result);
    }
    OK;
}
