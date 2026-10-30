#include <stdint.h>

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
 * Returns 0 if success else -ERROR.
 *
 *  ERRORS:
 *    - -EAGAIN
 *          Not enough resources to create another thread.
 */
int mythread_create(mythread_t* thread,
    void* (*start_routine)(void*),
    void* arg);



/** Wait for `thread` to terminate.
 * Returns 0 if success else -ERROR.
 *
 * ERRORS:
 *    - -EDEADLK
 *      Calling thread tries to join itself.
 *
 *    - -ESRCH
 *      No thread with the ID `thread` could be found.
 *
 *
 */
int mythread_join(mythread_t thread, void** retval);

/** Send a cancelation request to `thread`
 *
 * After a canceled thread has terminated, a join of this
 * thread obtains `MYTHREAD_CANCELED` as the thread's exit
 * status.
 *
 * Returns 0 if success else -ERROR.
 *
 * ERRORS
 *    - `-ESRCH`
 *      No thread with ID `thread` could be found.
 */
int mythread_cancel(mythread_t thread);


/** Send a cancelation signal to `thread`
 *
 * This function will cancel `thread` asynchronously
 * but there is no guarantee that it happens immediately
 *
 * Returns 0 if success else -ERROR.
 *
 * ERRORS
 *    - `-ESRCH`
 *      No thread with ID `thread` could be found.
 */
int mythread_cancel_async(mythread_t thread);


/** Check if there any pending cancellation requests for calling thread
 *
 * If no cancellation request is pending, then this call has no effect.
 * Othrewise, calling thread will be canceled and function won't return.
 * This function always suceeds.
 */
void mythread_testcancel(void);



/** Returns self mythread_t handler.
 *
 *  This function always succeed.
 */
mythread_t mythread_self();

/** Terminate thread and return `retval` to the joining thread.
  *
  * This function never returns.
  */
void mythread_exit(void *retval);
