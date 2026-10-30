#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/my_thread.h"
#include "../test_result.h"


TEST(test_join_self_v2) {

    int join_result = mythread_join(mythread_self(), NULL);

    if (join_result != -EDEADLK) {
        FAIL("mythread_join does not give us expected -EDAEDLK but gives: ", strerror(-join_result));
    }

    OK_MSG("got (expected) error: %s", strerror(-join_result));
}
