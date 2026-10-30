
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"

static void *
start_routine (void *arg) {
    mythread_t *self = arg;
    return (void*)(intptr_t) mythread_join(*self, NULL);
}


TEST(test_join_self) {
    mythread_t thread;
    int r;
    r = mythread_create(&thread, start_routine, &thread);
    if (r != 0) {
        FAIL("mythread_create failed with error: %s", strerror(-r));
    }

    int join_result;
    r = mythread_join(thread, (void*)&join_result);
    if (r != 0) {
        FAIL("mythread_join failed with error: %s", strerror(-r));
    }

    if (join_result != -EDEADLK) {
        FAIL("mythread_join does not give us expected -EDAEDLK but gives: ", strerror(-join_result));
    }

    OK_MSG("got (expected) error: %s", strerror(-join_result));
}
