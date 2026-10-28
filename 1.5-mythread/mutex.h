#ifndef MUTEX_H
#define MUTEX_H

#include "mythread.h"
#include <stdint.h>
#include <stdatomic.h>

typedef struct mutex_t {
    _Atomic(uint32_t) lock; /* 0 - unlocked, 1 - locked */
    mythread_t owner;
} mutex_t;

/**
 * Errors
 *  - EINVAL
 *      The mutex has not been properly initialized.
 *  - EDEADLK
 *      The mutex is already locked by the calling thread
 */
int mythread_mutex_lock(mutex_t *mu);

/**
 * Errors
 *  - EINVAL
 *      The mutex has not been properly initialized.
 *
 *  - EPERM
 *      Calling thread doesn't own the mutex.
 */
int mythread_mutex_unlock(mutex_t *mu);

void mythread_mutex_init(mutex_t *mu);

/* idk do i really neeed mythread_mutex_destroy() */

#endif
