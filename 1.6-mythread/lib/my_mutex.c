#define _GNU_SOURCE
#include <unistd.h>

#include "my_mutex.h"
#include "my_syscall.h"

#include <asm-generic/errno-base.h>
#include <errno.h>
#include <linux/futex.h>
#include <stdatomic.h>
#include <stdbool.h>

#include <stdio.h>
#undef errno // do not use errno! included only for constants


#define MUTEX_UNLOCKED 0
#define MUTEX_LOCKED 1

static bool
a_cas(_Atomic(uint32_t) *ptr, uint32_t expected, uint32_t desired)
{
    return atomic_compare_exchange_strong(ptr, &expected, desired);
}

int
mythread_mutex_lock(mythread_mutex_t *mu)
{
    int err;

    while(1) {

        if (mu->owner == gettid()) {
            return -EDEADLK;
        }

        if (a_cas(&mu->lock, MUTEX_UNLOCKED, MUTEX_LOCKED)) {
            if (mu->owner != 0) {
                return -EINVAL;
            }
            /* we have locked the mutex */
            mu->owner = gettid();
            break;
        }

        /* mutex is locked by someone else */
        err = futex((uint32_t *)&mu->lock, FUTEX_WAIT, MUTEX_LOCKED);

        if (err < 0 && err != -EAGAIN) {
            /*
             * err == -EAGAIN means that mutex has been unlocked after CAS
             * and we can try to lock it on the next iteration.
             */
            return err;
        }
    }
    return 0;
}


int
mythread_mutex_unlock(mythread_mutex_t *mu)
{
    int err;

    if (mu->owner != gettid()) {
        return -EPERM;
    }

    if (mu->lock == MUTEX_UNLOCKED) {
        return -EINVAL;
    }

    mu->owner = 0;
    mu->lock = MUTEX_UNLOCKED;

    /* FUTEX_WAIT returns only -EINVAL error or number of woken threads */
    err = futex((uint32_t *)&mu->lock, FUTEX_WAKE, 1 /* wake only one waiting thread */);
    if (err < 0) {
        return err;
    }
    return 0;
}
