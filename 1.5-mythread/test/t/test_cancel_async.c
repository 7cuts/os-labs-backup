
#include <asm-generic/errno-base.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"

static void *
start_routine (void *arg) {
    for (;;) {/* busy loop */}

   assert(false && "unreachable");
}

TEST(test_cancel_async) {
    mythread_t thread;
    int r;
    r = mythread_create(&thread, start_routine, NULL);
    if (r != 0) {
        FAIL("mythread_create failed with error: %s", strerror(-r));
    }

    r = mythread_cancel_async(thread);

    if (r != 0) {
        FAIL("mythread_cancel_async failed with error: %s", strerror(-r));
    }

    void *retval;
    r = mythread_join(thread, &retval);

    if (r != 0) {
        FAIL("mythread_join failed with error: %s", strerror(-r));
    }

    if (retval != MYTHREAD_CANCELED) {
        FAIL("expected retval = MYTHREAD_CANCELED but got: %p", retval);
    }

    OK_MSG("got retval = %p (MYTHREAD_CANCELED)", retval);
}
