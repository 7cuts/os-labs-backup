#include <asm-generic/errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"

static void *
start_routine (void *arg) {
    return (void*)(intptr_t) mythread_self();
}


TEST(test_self) {
    mythread_t thread;
    int r;
    r = mythread_create(&thread, start_routine, NULL);
    if (r != 0) {
        FAIL("mythread_create failed with error: %s", strerror(-r));
    }

    mythread_t thread_actual;
    r = mythread_join(thread, (void*)&thread_actual);
    if (r != 0) {
        FAIL("mythread_join failed with error: %s", strerror(-r));
    }

    if (thread != thread_actual) {
        FAIL("expected mythread_self = %ld, but got %ld", thread, thread_actual);
    }

    OK;
}
