#include <stdint.h>
#include <sys/types.h>
typedef struct mutex_t {
    _Atomic(uint32_t) lock; /* 0 - unlocked, 1 - locked */
    pid_t owner; /* tid */
} mythread_mutex_t;

/**
 * Errors:
 *  - `-EINVAL`
 *      The mutex has not been properly initialized.
 *  - `-EDEADLK`
 *      The mutex is already locked by the calling thread
 */
int mythread_mutex_lock(mythread_mutex_t *mu);

/**
 * Errors:
 *  - `-EINVAL`
 *      The mutex has not been properly initialized.
 *
 *  - `-EPERM`
 *      Calling thread doesn't own the mutex.
 */
int mythread_mutex_unlock(mythread_mutex_t *mu);
