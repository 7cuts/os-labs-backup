#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <stdbool.h>
#include <stdint.h>

/*
 * mythreads - A limited implementation of a wrapper for kernel threads,
 * inspired by pthreads (including function descriptions and error codes).
 */

typedef uintptr_t mythread_t;

#define MYTHREAD_CANCELED ((void*)-1)

/** Create thread and immediately run `start_routine` in it.
 *
 * The new thread terminates in one of the folowwing ways:
 *    - It calls `mythread_exit()` specifying the exit value
 *      that is available for another thread in same process
 *      that calls `mythread_join()`
 *
 *    - It returns from `start_routine()`.
 *
 *    - It is canceled via `mythread_cancel()`.
 *
 *    - Any of threads call `exit(3)` or main thread returns
 *      from main. TODO: what about detached threads?
 *
 * Returns 0 on success. On error returns error code. Constants
 * can be found in <errno.h> but mythread functions do not
 * set errno. (if i wait to support it, i need to touch libc things...
 * i realy don't want to. Moreover, pthreds not setting errno too).
 *
 *  ERRORS:
 *    EAGAIN
 *          Not enough resources to create another thread.
 */
int mythread_create(mythread_t* thread,
    void* (*start_routine)(void*),
    void* arg,
    bool async_cancelable);

/** Wait for `thread` to terminate.
 *
 * Returns 0 on success. On error returns error code. Constants
 * can be found in <errno.h> but mythread functions do not
 * set errno. (if i wait to support it, i need to touch libc things...
 * i realy don't want to. Moreover, pthreds not setting errno too).
 *
 * ERRORS:
 *  EDEADLK
 *      Two threads tried to join with each other
 *      or calling thread tries to join itself.
 *
 *  ESRCH
 *      No thread with the ID `thread` could be found.
 *
 *  EINVAL
 *      Another thread is already waiting for join this thread.
 *
 */
int mythread_join(mythread_t thread, void** retval);

/** Send a cancelation request to `thread`
 *
 * If asynchronous cancellation is enable then this function
 * will try to cancel thread immedeately. If it is disable
 * then thread will be canceled next time when it calls
 * `mythread_testcancel`.
 *
 * After a canceled thread has terminated, a join of this
 * thread obtains `MYTHREAD_CANCELED` as the thread's exit
 * status.
 *
 * Returns 0 on success. On error returns error code. Constants
 * can be found in <errno.h> but mythread functions do not
 * set errno. (if i wait to support it, i need to touch libc things...
 * i realy don't want to. Moreover, pthreds not setting errno too).
 *
 * ERRORS
 *   ESRCH
 *      No thread with ID `thread` could be found.
 */
int mythread_cancel(mythread_t thread);

/** Check if there any pending cancellation requests for calling thread
 *
 * If no cancellation request is pending, then this call has no effect.
 * Othrewise, calling thread will be canceled and function won't return.
 * This function always suceeds.
 */
void mythread_testcancel(void);
#endif
