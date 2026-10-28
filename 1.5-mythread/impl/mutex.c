#include "../mythread.h"
#include "../mutex.h"

 /* for E* constatns */
#include "../myerrno.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stddef.h>
#include <stdatomic.h>
#include <linux/futex.h>

/* Syscall wrapper to avoiding use of libc */
#include "mysyscall.h"

#define _MUTEX_UNLOCKED 0
#define _MUTEX_LOCKED 1

const uint32_t MUTEX_UNLOCKED = _MUTEX_UNLOCKED;
const uint32_t MUTEX_LOCKED = _MUTEX_LOCKED;

static int
futex(uint32_t *uaddr, int op, uint32_t expected_val)
{
    return my_syscall6(SYS_futex, uaddr, op, expected_val,
                    NULL, NULL, 0);
}


int
mythread_mutex_lock(mutex_t *mu)
{
    int err;

    while(1) {

        if (mu->owner == mythread_self()) {
            return -EDEADLK;
        }

        if (atomic_compare_exchange_strong(&mu->lock, &MUTEX_UNLOCKED, MUTEX_LOCKED)) {
            /* we have locked the mutex */
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
mythread_mutex_unlock(mutex_t *mu)
{
    int err;

    if (mu->owner != mythread_self()) {
        return -EPERM;
    }

    if (mu->lock == MUTEX_UNLOCKED) {
        return -EINVAL;
    }

    mu->owner = 0;
    mu->lock = MUTEX_UNLOCKED;

    /* FUTEX_WAIT returns only -EINVAL error */
    return futex((uint32_t *)&mu->lock, FUTEX_WAKE, 1 /* wake only one waiting thread */);
}


void
mythread_mutex_init(mutex_t *mu)
{
    *mu = (mutex_t){0};
}

void mythread_mutex_destroy(mutex_t *mu) {
    *mu = (mutex_t){0};
}
