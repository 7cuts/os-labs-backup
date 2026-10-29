
#include <asm-generic/errno-base.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"

static void *
start_routine (void *arg) {
   (void)arg;
   sleep(10);
   mythread_testcancel();
   return NULL;
}

TEST(test_cancel_wrong_id) {
    mythread_t thread;
    int r, r1, r2;
    r = mythread_create(&thread, start_routine, NULL);
    if (r != 0) {
        FAIL("mythread_create failed with error: %s", strerror(-r));
    }

    r1 = mythread_cancel(thread - 10);

    if (r1 != -ESRCH) {
        FAIL("expected mythread_cancel failed with error -ESRCH but got: %s", strerror(-r));
    }

    r2 = mythread_cancel(thread - 10);

    if (r2 != -ESRCH) {
        FAIL("expected mythread_cancel_async failed with error -ESRCH but got: %s", strerror(-r));
    }

    OK_MSG("got (expected) errors: %d, %d",-r1, -r2);
}
